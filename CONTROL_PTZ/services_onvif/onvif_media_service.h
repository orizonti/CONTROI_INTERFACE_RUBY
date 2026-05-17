#ifndef ONVIFMEDIASERVICE_H
#define ONVIFMEDIASERVICE_H

#include <QList>

class MediaProfile 
{
public:
    QString name;
    QString token;
};

class OnvifMediaService
{
    public:
    static QList<QString> onvif_schemas;
    static QList<MediaProfile> GetProfiles(QString addressMedia, QString username, QString password);
};

#endif // ONVIFMEDIASERVICE_H
