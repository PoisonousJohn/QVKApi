#ifndef VKAPI_H
#define VKAPI_H

#include <functional>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QWebView>
#include <QPointer>

#include "VKRequestManager.h"
#include "VKResponse.h"

class QWebView;
class QSettings;

namespace poison {
    class User : public QObject
    {
        Q_OBJECT
    public:

        Q_PROPERTY(QString name MEMBER _name)
        Q_PROPERTY(QString uid MEMBER _uid)

        QString _name;
        QString _uid;
    };

    class VKApi : public QObject {
        Q_OBJECT
    signals:
        void loginStatusChanged();
    public: // properties
        Q_PROPERTY(bool authorized READ isLoggedIn NOTIFY loginStatusChanged)
    public:
        VKApi();

        QUrl getLoginUrl() const;
        Q_INVOKABLE void init(const QString& appId, const QString &scope);
        Q_INVOKABLE void login();
        Q_INVOKABLE void logout();
        Q_INVOKABLE bool isLoggedIn() const;
        Q_INVOKABLE QVariant getUser() const { return QVariant::fromValue(user.data()); }
        void method(
                const QString& method,
                const QMap<QString, QString> params = QMap<QString, QString>(),
                std::function<void(const VKResponse&) > callback = 0
        );
    protected:
        void getCurrentUserInfo(std::function<void(QPointer<User>)> onComplete);
        void setCurrentUser(const QPointer<User>& currUser);

    protected: // members
        QString appId;
        QString scope;
        QString accessToken;
        QWebView* loginView;
        QNetworkAccessManager net;
        VKRequestManager reqManager;
        QSettings* settings;
        QPointer<User> user;


    protected slots:
        void loginUrlChanged(const QUrl&);

    };

}

#endif // VKAPI_H
