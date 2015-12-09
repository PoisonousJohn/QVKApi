#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include "VKResponse.h"

VKResponse::VKResponse(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NetworkError::NoError)
    {
        error.type = Error::Type::NETWORK;
        error.description = reply->errorString();
        error.code = reply->error();
        return;
    }

    QJsonDocument jsonDoc;
    QJsonParseError parseErr;
    auto data = reply->readAll();
    jsonDoc = QJsonDocument::fromJson(data , &parseErr);

    if (parseErr.error != QJsonParseError::NoError) {
        error.type = Error::Type::JSON;
        error.description = parseErr.errorString() + data;
        error.code = parseErr.error;
        qDebug() << "Failed to parse json:\n" << data;
    } else if (!jsonDoc.object().contains("response")) {
        qDebug() << "bad json.\n";
        error.type = Error::Type::BADCONTENT;
    } else if (jsonDoc.object().contains("error")) {
        error.type = Error::Type::API;
//        auto jsonErr = jsonDoc.object().val
//        if (jsonDoc.object().contains("")
//        error.description =
    } else {
        json = QJsonDocument( jsonDoc.object().value( "response" ).toArray().at(0).toObject() );
    }
}


QDebug operator<<(QDebug dbg, const VKResponse *resp)
{
    QDebugStateSaver saver(dbg);
    if (resp->isError()) {
        dbg.nospace() << "VKResponse:"
            << "\nerror: " << resp->getError().description
            << "\ncode: " << static_cast<int>(resp->getError().type);
    } else {
        dbg.nospace() << "VKResponse:\n"
            << resp->json.toJson();

    }

    return dbg.space();
}
