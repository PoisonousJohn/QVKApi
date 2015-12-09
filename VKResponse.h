#ifndef VKRESPONSE_H
#define VKRESPONSE_H

#include <QDebug>
#include <QJsonDocument>

class QNetworkReply;

class VKResponse
{
public: // sub types
    struct Error
    {
        enum class Type
        {
            NOERROR,
            UNKNOWN,
            NETWORK,
            JSON,
            BADCONTENT, // when json is valid but some fields are missing
            API, // VK API error
        };

        Type type = Type::NOERROR;
        QString description;
        int code = 0;
    };

public: // methods

    explicit VKResponse(QNetworkReply* reply);

    bool isError() const { return error.type != Error::Type::NOERROR; }
    Error getError() const { return error; }
    const QJsonDocument* getJson() const { return &json; }

private:
    Error error;
    QJsonDocument json;
    friend QDebug operator<<(QDebug dbg, const VKResponse* req);
};

extern QDebug operator<<(QDebug dbg, const VKResponse* req);

#endif // VKRESPONSE_H
