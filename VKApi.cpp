#include <boost/algorithm/string/join.hpp>
#include <boost/bind.hpp>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonArray>
#include "VKApi.h"

static const QString kApiBaseUrl = "https://api.vk.com/method/";
static const QString kAuthUrl = "https://oauth.vk.com/authorize";
static const QString kRedirectUrl = "http://oauth.vk.com/blank.html";

namespace poison
{


VKApi::VKApi()
    : loginView(0)
    , user(new User())
{

}

bool VKApi::isLoggedIn() const {
    return accessToken.length();
}

void VKApi::method(const QString &method,
        const QMap<QString, QString> params,
        std::function< void(const QJsonDocument*, QNetworkReply::NetworkError) > callback
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

    QNetworkRequest req;
    req.setUrl(url);

    qDebug() << "Sending request to: " << req.url().toString() << "\n";
    auto reply = net.get(req);
    connect(reply, &QNetworkReply::finished, [reply, callback]() {
        QJsonDocument* json = 0;
        QJsonDocument jsonDoc;
        QNetworkReply::NetworkError error = reply->error();
        qDebug() << "Finished reply: " << reply << "\n";
        if (reply->error() != QNetworkReply::NetworkError::NoError) {
            qDebug() << "Network error: " << reply->errorString() << "\n";
        } else {
            QJsonParseError parseErr;
            auto data = reply->readAll();
            jsonDoc = QJsonDocument::fromJson(data , &parseErr );

            qDebug() << "got response <" << reply->url() << ">\n" << data << "\n\n\n";

            if (parseErr.error != QJsonParseError::NoError) {
                qDebug() << "failed to parse json: " << parseErr.errorString() << "\n";
                error = QNetworkReply::NetworkError::UnknownContentError;
            } else if (!jsonDoc.object().contains("response")) {
                qDebug() << "bad json.\n";
                error = QNetworkReply::NetworkError::UnknownContentError;
            } else {
                jsonDoc = QJsonDocument( jsonDoc.object().value( "response" ).toArray().at(0).toObject() );
                json = &jsonDoc;
            }
        }

        if (callback)
            callback(json, error);

        reply->deleteLater();
    });
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

void VKApi::loginUrlChanged(const QUrl& url) {
    qDebug() << "login url filename: \n" << url.fileName() << "\n";
    if (url.fileName() == "blank.html") {
        QUrlQuery q(url.fragment());
        accessToken = q.queryItemValue("access_token");
        qDebug() << "got access token: " << accessToken << "\n";

        QMap<QString, QString> params;
        method("users.get", params, [this]( const QJsonDocument* json, QNetworkReply::NetworkError) {
            loginView->close();
            loginView = 0;

            if (json) {
                auto obj = json->object();
                qDebug() << obj.keys().join(",");
                user->_name = obj["first_name"].toString() + " " + obj["last_name"].toString();
                user->_uid = QString::number( obj["uid"].toInt() );
            } else {
                this->logout();
                this->login();
            }

            emit loginStatusChanged();
        });

        loginView->setVisible(false);
    }
}

void VKApi::logout() {
    accessToken.clear();
}



}
