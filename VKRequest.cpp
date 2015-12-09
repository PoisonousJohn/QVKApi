#include <QNetworkRequest>
#include <QNetworkReply>
#include "VKRequest.h"

VKRequest::VKRequest(QUrl url)
    : url(url)
    , type(Type::GET)
    , reply(nullptr)
{
}

VKRequest::VKRequest(QUrl url, QUrlQuery postData)
    : url(url)
    , type(Type::POST)
    , postData(postData)
    , reply(nullptr)
{

}

void VKRequest::send(QNetworkAccessManager *net)
{
    QNetworkRequest req;
    req.setUrl(url);

    if (type == Type::GET)  {
        reply = net->get(req);
    } else {
        reply = net->post(req, postData.toString(QUrl::FullyEncoded).toUtf8());
    }

    qDebug() << this << "started";

    connect(reply, &QNetworkReply::finished, this, &VKRequest::onReplyReceived);

}

void VKRequest::onReplyReceived()
{
    qDebug() << this << "completed";
    VKResponse resp(reply);
    qDebug() << &resp;
    emit finished(resp);
    reply->deleteLater();
    deleteLater();
}


QDebug operator<<(QDebug dbg, const VKRequest *req)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "VKRequest<" << static_cast<const QObject*>(req) << "> [" << req->url.toDisplayString() << "]";
    return dbg.space();
}
