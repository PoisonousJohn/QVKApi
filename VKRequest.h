#ifndef VKREQUEST_H
#define VKREQUEST_H

#include <QDebug>
#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include "VKResponse.h"

/**
 * @brief The VKRequest class is automatically deleted after completed
 */
class VKRequest : public QObject
{
    Q_OBJECT

public: // methods

    /**
     * @brief Creates GET request
     * @param url
     */
    VKRequest(QUrl url);
    /**
     * @brief Creates POST request
     * @param url
     * @param postData
     */
    VKRequest(QUrl url, QUrlQuery postData);


signals:
    /**
     * @brief after this signal is emitted, VKRequest will be automatically deleted
     * @param response
     */
    void finished(const VKResponse& response);

private: // sub types
    enum class Type
    {
        GET,
        POST,
    };

private: // members
    QUrl url;
    Type type = Type::GET;
    // fill with data if type == POST
    QUrlQuery postData;
    QNetworkReply* reply;


private: // methods

    friend class VKRequestManager;
    friend QDebug operator<<(QDebug dbg, const VKRequest* req);
    void send(QNetworkAccessManager* net);

private slots:
    void onReplyReceived();
};

extern QDebug& operator<<(QDebug& dbg, const VKRequest* req);

#endif // VKREQUEST_H
