#ifndef VKAPI_H
#define VKAPI_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <ServiceContainer/ServiceInterface.h>

class QWebView;
class QSettings;

namespace poison {

    class VKApi : public QObject {
        Q_OBJECT
    signals:
        void loginStatusChanged(bool);
    public:
        VKApi(const QString&& appId, QSettings* settings);
        QUrl getLoginUrl() const;
        void login();
        void logout();
        bool isLoggedIn() const;
        void method(
                const QString& method,
                const QMap<QString, QString> params = QMap<QString, QString>(),
                std::function< void(const QJsonDocument*, QNetworkReply::NetworkError) > callback = 0
        );
    protected:
        QString appId;
        QString accessToken;
        QWebView* loginView;
        QNetworkAccessManager net;
        QSettings* settings;


    protected slots:
        void loginUrlChanged(const QUrl&);

    };

}

#endif // VKAPI_H
