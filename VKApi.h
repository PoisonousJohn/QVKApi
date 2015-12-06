#ifndef VKAPI_H
#define VKAPI_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QWebView>

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
    public  :
        VKApi(QSettings* settings);

        QUrl getLoginUrl() const;
        Q_INVOKABLE void init(const QString& appId);
        Q_INVOKABLE void login();
        Q_INVOKABLE void logout();
        Q_INVOKABLE bool isLoggedIn() const;
        Q_INVOKABLE QVariant getUser() const { return QVariant::fromValue(user); }
        void method(
                const QString& method,
                const QMap<QString, QString> params = QMap<QString, QString>(),
                std::function<void(const QJsonDocument*, QNetworkReply::NetworkError) > callback = 0
        );
    protected:
        QString appId;
        QString accessToken;
        QWebView* loginView;
        QNetworkAccessManager net;
        QSettings* settings;
        User* user;


    protected slots:
        void loginUrlChanged(const QUrl&);

    };

}

#endif // VKAPI_H
