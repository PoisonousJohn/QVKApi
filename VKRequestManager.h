#ifndef VKREQUESTLIMITER_H
#define VKREQUESTLIMITER_H

#include <functional>

#include <QObject>
#include <QNetworkRequest>
#include <QQueue>
#include <QTimer>
#include <QPair>
#include <QUrlQuery>
#include "VKRequest.h"

class QNetworkAccessManager;
class QNetworkReply;

class VKRequestManager : public QObject
{

    Q_OBJECT
public:
    explicit VKRequestManager(QNetworkAccessManager *nm, QObject *parent = 0);
    /**
     * @brief limiter won't exceed specified limit per second
     * @param max count of reqests per second
     * @default default value of the limit is 3
     */
    void setReqLimitPerSec(char value);
    /**
     * @brief if no limits are exceeded, request will be started instantly,
     * otherwise request will be delayed according to specified limit
     *
     * VKRequest is automatically deleted after completed.
     * @param req
     */
    void sendRequest(VKRequest *req);

private: // members
    QNetworkAccessManager* net;
    char reqLimit;
    QQueue<VKRequest*> reqQueue;
    QTimer timer;
    char reqStarted;

private: // methods
    void tryToSendReq();

signals:

private slots:
    void onTimerFire();
    void onReqFinish();
};

#endif // VKREQUESTLIMITER_H
