#include <functional>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include "VKApi.h"

static const QString kApiBaseUrl = "https://api.vk.com/method/";
static const QString kAuthUrl = "https://oauth.vk.com/authorize";
static const QString kRedirectUrl = "http://oauth.vk.com/blank.html";
static const QString kAccessTokenSetting = "AccessToken";
static const QString kSettingsApplicationId = "QVKApi";

namespace poison
{


VKApi::VKApi()
    : loginView(0)
    , reqManager(&net)
    , settings(new QSettings(kSettingsApplicationId))
    , user(new User())
{

}

bool VKApi::isLoggedIn() const {
    return accessToken.length();
}

void VKApi::method(const QString &method,
        const QMap<QString, QString> params,
        std::function< void(const VKResponse&) > callback
)  {
    QUrl url( kApiBaseUrl + method );

    QUrlQuery q;

    if (!params.empty()) {
        for (const auto& key : params) {
            q.addQueryItem( key, params.value(key) );
        }
    }

    if (isLoggedIn())
        q.addQueryItem("access_token", accessToken);
    url.setQuery(q);

    auto req = new VKRequest(url);

    if (callback) {
        connect(req, &VKRequest::finished, callback);
    }

    reqManager.sendRequest(req);
}

QUrl VKApi::getLoginUrl() const {
    QUrl url(kAuthUrl);
    QUrlQuery q;
    q.addQueryItem("client_id", appId);
    q.addQueryItem("scope", scope);
    q.addQueryItem("display", "popup");
    q.addQueryItem("redirect_uri", kRedirectUrl);
    q.addQueryItem("response_type", "token");
    url.setQuery(q);

    return url;
}

void VKApi::init(const QString& appId, const QString& scope)
{
    this->appId = appId;
    this->scope = scope;
    qDebug() << "VK inited";

    accessToken = settings->value(kAccessTokenSetting).toString();
    if (!accessToken.isEmpty())
    {
        getCurrentUserInfo([this](QPointer<User> currUser) {
            setCurrentUser(currUser);
            emit loginStatusChanged();
        });
    }
}

void VKApi::login() {
    if (isLoggedIn() || loginView) {
        return;
    }

    auto webView = new QWebView();
    webView->setAttribute(Qt::WA_DeleteOnClose, true);
    auto url = getLoginUrl();
    webView->load( url );

    connect(webView, SIGNAL(urlChanged(QUrl)), this, SLOT(loginUrlChanged(const QUrl&)));
    connect(webView, &QWebView::destroyed, [this](){
        this->loginView = 0;
    });
    webView->show();

    loginView = webView;

}

void VKApi::getCurrentUserInfo(std::function<void(QPointer<User>)> onComplete)
{
    QMap<QString, QString> params;
    method("users.get", params, [this, onComplete]( const VKResponse& resp) {
        if (!resp.isError()) {
            auto obj = resp.getJson()->object();
            qDebug() << obj.keys().join(",");
            auto name = obj["first_name"].toString() + " " + obj["last_name"].toString();
            auto uid = QString::number( obj["uid"].toInt() );
            if (onComplete)
            {
                auto currUser = new User();
                currUser->_name = name;
                currUser->_uid = uid;
                onComplete(QPointer<User>(currUser));
            }
        } else if (onComplete) {
            onComplete(QPointer<User>());
        }

    });
}

void VKApi::setCurrentUser(const QPointer<User>& currUser)
{
    if (user)
    {
        user->deleteLater();
    }

    user = currUser;

    if (user.isNull())
    {
        logout();
        login();
    }
}

void VKApi::loginUrlChanged(const QUrl& url) {
    qDebug() << "login url filename: \n" << url.fileName() << "\n";
    if (url.fileName() == "blank.html") {
        QUrlQuery q(url.fragment());
        accessToken = q.queryItemValue("access_token");
        qDebug() << "got access token: " << accessToken << "\n";
        settings->setValue(kAccessTokenSetting, accessToken);

        getCurrentUserInfo([this] (QPointer<User> currUser) {
            loginView->close();
            loginView = 0;

            setCurrentUser(currUser);

            emit loginStatusChanged();
        });

        loginView->setVisible(false);
    }
}

void VKApi::logout() {
    accessToken.clear();
}



}
