#include "device_zoom_rotation.h"
#include "./services_onvif/onvif_device_service.h"
#include "./services_onvif/onvif_media_service.h"

#include <QString>

#include <chrono>
using namespace std::chrono_literals;

QList<QString> DeviceZoomRotation::onvif_schemas
{
    "xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\"",
    "xmlns:d=\"http://www.w3.org/2001/XMLSchema\"",
    "xmlns:c=\"http://www.w3.org/2003/05/soap-encoding\""
};


DeviceZoomRotation::DeviceZoomRotation(QString user, QString pass, QString ip, QString port, QObject* parent): QObject(parent)
{
  QTimer::singleShot(6000,[this,user,pass,ip, port](){connectToDevice(user,pass,ip,port);});
}

DeviceZoomRotation::DeviceZoomRotation() { }

void DeviceZoomRotation::connectToDevice(QString user, QString pass, QString ip, QString port)
{
qDebug() << "[PTZ CONNECT]" << user << pass << ip << port;
auto service_address = tr("http://%1:%2/onvif/device_service").arg(ip).arg(port); 
DeviceCapabilities deviceCapabilities = OnvifDeviceService::GetCapabilities(service_address, user, pass);
      QList<MediaProfile> listProfile = OnvifMediaService::GetProfiles(deviceCapabilities.addressMediaService,  user, pass);

                  if(listProfile.isEmpty())  qDebug() << "[ CONNECT FAIL    ]" << deviceCapabilities.addressMediaService; 
                  if(listProfile.isEmpty())  return; 

    qDebug() << "[ CONNECT SUCCESS  ]" << deviceCapabilities.addressPTZService;

    profile_token = listProfile[0].token; //ONVIF SERVICES TOKENS
    //============================================================================
    requestStop.address = deviceCapabilities.addressPTZService;
    requestStop.username = user;
    requestStop.password = pass;
    requestStop.action = "http://www.onvif.org/ver20/ptz/wsdl/Stop";
    requestStop.XMLNs = onvif_schemas;
    QString body("<Stop xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile_token);
    body.push_back("</ProfileToken>");
    body.push_back("<PanTilt>true</PanTilt><Zoom>true</Zoom>");
    body.push_back("</Stop>");
    requestStop.body = body;

    //============================================================================
    requestMove.address = deviceCapabilities.addressPTZService;
    requestMove.username = user;
    requestMove.password = pass;
    requestMove.action = "http://www.onvif.org/ver20/ptz/wsdl/ContinousMove";
    requestMove.XMLNs = onvif_schemas;
    body = QString("<Stop xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile_token);
    body.push_back("</ProfileToken>");
    body.push_back("<PanTilt>true</PanTilt><Zoom>true</Zoom>");
    body.push_back("</Stop>");
    requestMove.body = body;

    //============================================================================
    requestSetHome.address = deviceCapabilities.addressPTZService;
    requestSetHome.username = user;
    requestSetHome.password = pass;
    requestSetHome.action = "http://www.onvif.org/ver20/ptz/wsdl/SetHomePosition";
    requestSetHome.XMLNs = onvif_schemas;
    body = QString("<SetHomePosition xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile_token);
    body.push_back("</ProfileToken>");
    body.push_back("<PanTilt>true</PanTilt><Zoom>true</Zoom>");
    body.push_back("</SetHomePosition>");
    requestSetHome.body = body;


    //============================================================================
    requestSetPreset.address = deviceCapabilities.addressPTZService;
    requestSetPreset.username = user;
    requestSetPreset.password = pass;
    requestSetPreset.action = "http://www.onvif.org/ver20/ptz/wsdl/SetPreset";
    requestSetPreset.XMLNs = onvif_schemas;
    body = QString("<SetPreset xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile_token);
    body.push_back("</ProfileToken>");
//    body.push_back("<PresetName>");
//    body.push_back("qwe");
//    body.push_back("</PresetName>");
//    body.push_back("<PresetToken>");
//    body.push_back("123");
//    body.push_back("</PresetToken>");
    body.push_back("</SetPreset>");
    requestSetPreset.body = body;

    //============================================================================
    requestGoPreset.address = deviceCapabilities.addressPTZService;
    requestGoPreset.username = user;
    requestGoPreset.password = pass;
    requestGoPreset.action = "http://www.onvif.org/ver20/ptz/wsdl/GotoPreset";
    requestGoPreset.XMLNs = onvif_schemas;
    body = QString("<GotoPreset xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile_token);
    body.push_back("</ProfileToken>");
    body.push_back("<PresetToken>");
    body.push_back("0");
    body.push_back("</PresetToken>");
    body.push_back("</GotoPreset>");
    requestGoPreset.body = body;

    //============================================================================
    requestGetPreset.address = deviceCapabilities.addressPTZService;
    requestGetPreset.username = user;
    requestGetPreset.password = pass;
    requestGetPreset.action = "http://www.onvif.org/ver20/ptz/wsdl/GetPresets";
    requestGetPreset.XMLNs = onvif_schemas;
    body = QString("<GetPresets xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile_token);
    body.push_back("</ProfileToken>");
    body.push_back("</GetPresets>");
    requestGetPreset.body = body;

    //============================================================================
    requestGetStatus.address = deviceCapabilities.addressPTZService;
    requestGetStatus.username = user;
    requestGetStatus.password = pass;
    requestGetStatus.action = "http://www.onvif.org/ver20/ptz/wsdl/GetStatus";
    requestGetStatus.XMLNs = onvif_schemas;
    body = QString("<GetStatus xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile_token);
    body.push_back("</ProfileToken>");
    body.push_back("</GetStatus>");

    requestGetStatus.body = body;

    //============================================================================
    requestGoHome.address = deviceCapabilities.addressPTZService;
    requestGoHome.username = user;
    requestGoHome.password = pass;
    requestGoHome.action = "http://www.onvif.org/ver20/ptz/wsdl/GotoHomePosition";
    requestGoHome.XMLNs = onvif_schemas;
    body = QString("<GotoHomePosition xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile_token);
    body.push_back("</ProfileToken>");
    body.push_back("</GotoHomePosition>");
    requestGoHome.body = body;

    slotMoveLimit();
}

void DeviceZoomRotation::slotContinuousMove(double x, double y, double z)
{
    QString body("<ContinuousMove xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile_token);
    body.push_back("</ProfileToken>");
    body.push_back("<Velocity>");
    body.push_back("<PanTilt xmlns=\"http://www.onvif.org/ver10/schema\" x=\"" + QString::number(x) + "\" y=\""+ QString::number(y) +"\"/>");
    body.push_back("<Zoom xmlns=\"http://www.onvif.org/ver10/schema\" x=\"" + QString::number(z) + "\"/>");
    body.push_back("</Velocity></ContinuousMove>");
    requestMove.body = body;
    requestMove.sendRequest(response);
}

void DeviceZoomRotation::slotZoomIn()  { slotContinuousMove(0,0, 1); }
void DeviceZoomRotation::slotZoomOut() { slotContinuousMove(0,0,-1); }

void DeviceZoomRotation::slotStop()            { requestStop.sendRequest(response); }
void DeviceZoomRotation::slotSetHomePosition() { requestSetHome.sendRequest(response); }

void DeviceZoomRotation::slotGoToHomePosition() {    requestGoHome.sendRequest(response); }
void DeviceZoomRotation::slotSetPreset()        { requestSetPreset.sendRequest(response); }
void DeviceZoomRotation::slotGotoPreset(){  requestGoPreset.sendRequest(response); }
void DeviceZoomRotation::slotGetPreset() { requestGetPreset.sendRequest(response); }
void DeviceZoomRotation::slotGetStatus() { requestGetStatus.sendRequest(response); }



void DeviceZoomRotation::setParam(uint16_t CommandID, float    CommandParam)  
{
  qDebug() << "[ZOOM]" << CommandID << "PARAM:" << CommandParam;
  switch(CommandID)
  {
    case 0:
      if(CommandParam > 0 ) slotZoomOut();
      if(CommandParam < 0 ) slotZoomIn();
      if(CommandParam == 0) slotStop();
    qDebug() << "MOVE ZOOM: "  << CommandParam;
    break;
    case 1:
    qDebug() << "SET ZOOM: "  << CommandParam;
    slotMovePos(CommandParam);
    break;

  }
}


void DeviceZoomRotation::slotMovePos(int pos)
{

  int steps_count = std::abs(pos - posStep);
  int dir = 1; if(pos - posStep < 0) dir = -1; 

  if(dir ==  1) slotZoomOut(); 
  if(dir == -1) slotZoomIn(); 

  QTimer::singleShot(steps_count*timePeriodStep,[this,pos]() 
                         { 
                          slotStop(); posStep = pos; 
                          qDebug() << TAG_NAME << "[MOVE STEP]" << posStep;
                         }); 

  qDebug() << "MOVE TO POS: " << pos << "STEPS: " << steps_count << "DIR: " << dir;
}

void DeviceZoomRotation::slotMoveStep()
{
  slotZoomOut(); QTimer::singleShot(timePeriodStep,[this]() 
                         { 
                          slotStop(); posStep++; 
                          qDebug() << TAG_NAME << "[MOVE STEP]" << posStep;
                         }); 
}

void DeviceZoomRotation::slotMoveLimit()
{
  slotZoomIn(); QTimer::singleShot(8000,[this]() { slotStop(); qDebug() << TAG_NAME << "AT LIMIT"; });
}

















