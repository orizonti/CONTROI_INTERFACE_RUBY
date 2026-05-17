#include "onvif_device_service.h"

#include "./request_soap/request_soap.h"
#include <QDomDocument>


QList<QString> OnvifDeviceService::onvif_schemas
{
    "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\"",
    "xmlns:tt=\"http://www.onvif.org/ver10/schema\""
};


DeviceCapabilities OnvifDeviceService::GetCapabilities(QString deviceXAddress, QString username, QString password)
{
    SoapRequest *soapRequest = new SoapRequest();
    soapRequest->address = deviceXAddress;
    soapRequest->username = username;
    soapRequest->password = password;
    soapRequest->XMLNs = onvif_schemas;
    QString body("<tds:GetCapabilities><tds:Category>All</tds:Category></tds:GetCapabilities>");
    soapRequest->body = body;

               QString response;
    DeviceCapabilities capabilities;

                    bool got_reply = soapRequest->sendRequest(response); delete soapRequest;
                    if (!got_reply) return capabilities;

//    qDebug() << "GET PROFILES";
//    qDebug() << "=================================";
//    qDebug() << response;
//    qDebug() << "=================================";

    
        QDomDocument doc;
        doc.setContent(response);
        QDomElement rootElement = doc.documentElement();
        for(QDomNode node = rootElement.firstChild(); !node.isNull(); node = node.nextSibling())
        {
            QDomElement elementBody = node.toElement();
            for(QDomNode node1 = elementBody.firstChild(); !node1.isNull(); node1 = node1.nextSibling())
            {
                QDomElement elementGetCapabilitiesResponse = node1.toElement();
                for(QDomNode node2 = elementGetCapabilitiesResponse.firstChild(); !node2.isNull(); node2 = node2.nextSibling())
                {
                    QDomElement elementCapabilities = node2.toElement();
                    for(QDomNode node3 = elementCapabilities.firstChild(); !node3.isNull(); node3 = node3.nextSibling())
                    {
                        if (node3.nodeName().toLower().endsWith("ptz"))
                        {
                            QDomElement elementPtz = node3.toElement();
                            for(QDomNode node4 = elementPtz.firstChild(); !node4.isNull(); node4 = node4.nextSibling())
                            {
                                if (node4.nodeName().toLower().endsWith("xaddr"))
                                {
                                    capabilities.addressPTZService = node4.toElement().text();
                                }
                            }
                        }
                        else if (node3.nodeName().toLower().endsWith("media"))
                        {
                            QDomElement elementPtz = node3.toElement();
                            for(QDomNode node4 = elementPtz.firstChild(); !node4.isNull(); node4 = node4.nextSibling())
                            {
                                if (node4.nodeName().toLower().endsWith("xaddr"))
                                {
                                    capabilities.addressMediaService = node4.toElement().text();
                                }
                            }
                        }
                    }
                }
            }
        }
    return capabilities;
}
