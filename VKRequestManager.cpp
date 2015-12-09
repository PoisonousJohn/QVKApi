#include <QDebug>
#include <QNetworkReply>
#include "VKRequestManager.h"

VKRequestManager::VKRequestManager(QNetworkAccessManager *nm, QObject *parent)
    : QObject(parent)
    , net(nm)
    // 3 is the value of VK API default limits
    , reqLimit(3)
    , reqStarted(0)
{
    connect(&timer, &QTimer::timeout, this, &VKRequestManager::onTimerFire);
    timer.start(1000);
}

void VKRequestManager::setReqLimitPerSec(char value)
{
    reqLimit = value;
}

void VKRequestManager::sendRequest(VKRequest* req)
{
    reqQueue.push_back(req);
    tryToSendReq();
}

void VKRequestManager::tryToSendReq()
{
    if (reqStarted >= reqLimit)
    {
        qDebug() << "Request limit exceeded. Waiting...";
        return;
    }

    if (reqQueue.empty())
    {
        return;
    }

    auto req = reqQueue.first();
    req->send(net);
    reqQueue.removeOne(req);

    ++reqStarted;
}

void VKRequestManager::onTimerFire()
{
    reqStarted = 0;
    tryToSendReq();
}

void VKRequestManager::onReqFinish()
{
    auto reply = sender();
    reply->deleteLater();
}
