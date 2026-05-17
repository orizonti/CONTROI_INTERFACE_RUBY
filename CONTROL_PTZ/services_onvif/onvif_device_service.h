#ifndef ONVIFDEVICESERVICE_H
#define ONVIFDEVICESERVICE_H

#include <QList>

class DeviceCapabilities 
{
public:
    QString addressMediaService;
    QString addressPTZService;
};

class OnvifDeviceService
{
private:
    static QList<QString> onvif_schemas;
public:
    static DeviceCapabilities GetCapabilities(QString deviceXAddress, QString username, QString password);
};

#endif // ONVIFDEVICESERVICE_H
