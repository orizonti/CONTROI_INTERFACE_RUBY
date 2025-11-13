
#include <QDebug>
#include <QApplication>
#include <QImage>

#include <QThread>

#include <opencv2/videoio.hpp>
#include <qnamespace.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <QDebug>
#include "interface_camera_rtsp.h"
#include <QThread>
#include "control_ptz_camera.h"
#include <QStringList>
#include <QFile>
#include "widget_complex_interface.h"
#include "message_command_structures.h"
#include "device_rotary_interface.h"
#include "device_laser_interface.h"
#include "device_focus_ranger_interface.h"
#include "engine_type_register.h"
#include <memory>
#include "widget_laser_control.h"
#include "device_lid_interface.h"
#include <QRegularExpression>

QStringList LoadCameraLinks();

#include "message_command_id.h"

int ID1 = TypeRegister<CommandSetPosScanator    >::RegisterType();
int ID2 = TypeRegister<CommandSetPosRotary      >::RegisterType();
int ID3 = TypeRegister<CommandDeviceController  >::RegisterType();
int ID4 = TypeRegister<CommandDeviceLaserPower  >::RegisterType();
int ID5 = TypeRegister<CommandDeviceLaserPointer>::RegisterType();
int ID6 = TypeRegister<CommandDeviceFocusator   >::RegisterType();
int ID7 = TypeRegister<MessageDeviceController  >::RegisterType();
int ID8 = TypeRegister<MessageDeviceLaserPower  >::RegisterType();
int ID9 = TypeRegister<MessageDeviceLaserPointer>::RegisterType();
int ID10 = TypeRegister<CommandSetPosScanator   >::RegisterType();
int ID11 = TypeRegister<CommandSetPosScanator   >::RegisterType();
int ID12 = TypeRegister<MessagePositionState<0> >::RegisterType();
int ID13 = TypeRegister<MessagePositionState<1> >::RegisterType();
int ID14 = TypeRegister<CommandCheckConnection  >::RegisterType();
int ID15 = TypeRegister<CommandCloseConnection  >::RegisterType();

void printRegisteredTypes()
{
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandSetPos<0>>::TYPE_ID).arg(TypeRegister<CommandSetPosScanator>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandSetPos<1>>::TYPE_ID).arg(TypeRegister<CommandSetPosRotary>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandDevice<0>>::TYPE_ID).arg(TypeRegister<CommandDevice<0>>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandDevice<1>>::TYPE_ID).arg(TypeRegister<CommandDevice<1>>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandDevice<2>>::TYPE_ID).arg(TypeRegister<CommandDevice<2>>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandDevice<3>>::TYPE_ID).arg(TypeRegister<CommandDevice<3>>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<MessageDevice<0>>::TYPE_ID).arg(TypeRegister<MessageDevice<0>>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<MessageDevice<1>>::TYPE_ID).arg(TypeRegister<MessageDevice<1>>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<MessageDevice<2>>::TYPE_ID).arg(TypeRegister<MessageDevice<2>>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<MessageDevice<3>>::TYPE_ID).arg(TypeRegister<MessageDevice<3>>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandCalibration>::TYPE_ID).arg(TypeRegister<CommandCalibration>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<MessagePositionState<0>>::TYPE_ID).arg(TypeRegister<MessagePositionState<0>>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<MessagePositionState<1>>::TYPE_ID).arg(TypeRegister<MessagePositionState<0>>::GetTypeSize());
  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandCheckConnection>::TYPE_ID).arg(TypeRegister<CommandCheckConnection>::GetTypeSize());
}

template<> void CommandDispatcherGeneric<TypeRegister<CommandDevice<0>>::ID()>::dispatchCommand(const QByteArray& Command) 
{
  qDebug() << "DISPATCH COMMAND: " << TypeRegister<CommandDevice<0>>::TYPE_ID << " DEV 0";
};

template<> void CommandDispatcherGeneric<TypeRegister<CommandSetPosRotary>::ID()>::dispatchCommand(const QByteArray& Command) 
{
  qDebug() << "DISPATCH COMMAND: " << TypeRegister<CommandSetPosRotary>::TYPE_ID << " POS ROTARY";
};



int main(int argc, char* argv[])
{
  QApplication app(argc,argv);

  printRegisteredTypes();

  QByteArray array; array.resize(20);

  //ControlPTZCamera PTZDevice;
  //                 PTZDevice.connectToCamera("192.168.1.11", "8899", "admin", "admin");

  WidgetComplexInterface* WindowInterface = new WidgetComplexInterface;

  //=====================================================================================================
  //CONNECTIONS
  std::shared_ptr<UDPConnectionEngine> ConnectionInterface = std::make_shared<UDPConnectionEngine>();
  ConnectionInterface->listenTo("192.168.0.36",2323);
  ConnectionInterface->connectTo("192.168.0.57",2525);

  std::shared_ptr<UDPConnectionEngine> ConnectionInterface2 = std::make_shared<UDPConnectionEngine>();
  ConnectionInterface2->listenTo("192.168.0.36",40805);
  ConnectionInterface2->connectTo("192.168.0.106",40804);

  std::shared_ptr<UDPConnectionEngine> ConnectionInterface3 = std::make_shared<UDPConnectionEngine>();
  ConnectionInterface3->listenTo("192.168.0.36",2323);
  ConnectionInterface3->connectTo("192.168.0.120",9000);

  //=====================================================================================================
  //LASERS
  using CommandLaserPointer = MessageGenericExt<CommandDeviceLaserPointer,MESSAGE_HEADER_EXT>;
  using RequestLaserPointer = MessageDevice<1>;

  using CommandLaserPower = MessageGenericExt<CommandDeviceLaserPower,MESSAGE_HEADER_EXT>;
  using RequestLaserPower = MessageDevice<2>;

  DeviceLaserInterface<UDPConnectionEngine,CommandLaserPower,RequestLaserPower> ControlLaserPower  
  {ConnectionInterface , "Силовой лазер"};

  DeviceLaserInterface<UDPConnectionEngine,CommandLaserPointer,RequestLaserPointer> ControlLaserPointer
  {ConnectionInterface , "Подсветчик"};

  //=====================================================================================================
  //LIDS
  QString open1  {"value: open, pin: 0, down: 2470, top: 1300, step: 50, interval: 0.03" };
  QString close1 {"value: close, pin: 0, down: 2470, top: 1300, step: 2, interval: 0.001"};
  QString open2  {"value: open, pin: 1, down: 530, top: 1750, step: 50, interval: 0.03"  };
  QString close2 {"value: close, pin: 1, down: 530, top: 1750, step: 2, interval: 0.001" };

  std::shared_ptr<DeviceLidInterface> LidControl 
  = std::make_shared<DeviceLidControl<UDPConnectionEngine>>(ConnectionInterface3, open1, close1);

  std::shared_ptr<DeviceLidInterface> LidControl2
  = std::make_shared<DeviceLidControl<UDPConnectionEngine>>(ConnectionInterface3, open2, close2);

  WindowInterface->ControlLid->linkTo(LidControl,LidControl2);

  //==================================================================================================================
  //ROTARY SCANATOR
  using CommandScanator = MessageGenericExt<CommandSetPosScanator,MESSAGE_HEADER_EXT>;
  using RequestScanator = CommandSetPosScanator;

  std::shared_ptr<DeviceRotaryInterface> ControlScanator 
  = std::make_shared<DeviceRotaryControl<UDPConnectionEngine,CommandScanator,RequestScanator>>(ConnectionInterface);

  std::shared_ptr<DeviceRotaryInterface> ControlRotary 
  = std::make_shared<DeviceRotaryControl<UDPConnectionEngine,ControlTX,ControlRX>>(ConnectionInterface2);
  //ControlRotary->moveWithVelocity(QPair<int,int>(0,0));

                                               WindowInterface->ControlRotary->linkToDevice(ControlRotary);
                                               WindowInterface->ControlScanatorPanel->linkToDevice(ControlScanator);

                                               WindowInterface->windowControlLaserPower->linkTo(&ControlLaserPower);
                                               WindowInterface->windowControlLaserIllum->linkTo(&ControlLaserPointer);
  //==================================================================================================================

  //DeviceFocusRangerInterface<TCPConnectionEngine,CommandDevice<3>,MessageDevice<3>> ControlFocus{&ConnectionInterface , "[ FOCUSATOR ]"};

  //==================================================================================================================
  //CAMERAS
  QStringList links; 
            //links = LoadCameraLinks();
  links.resize(3);
  links[0] = "rtsp://192.168.1.21:554/user=admin_password=_channel=1_stream=0.sdp";
  links[1] = "rtsp://192.168.1.75:8554/test";
  links[2] = "rtsp://192.168.1.59:8554/test";

  CameraInterfaceUniversal* Camera1 = new CameraInterfaceUniversal(links[0].toStdString(), "[CAMERA1]");
  CameraInterfaceUniversal* Camera2 = new CameraInterfaceUniversal(links[1].toStdString(), "[CAMERA2]");
  CameraInterfaceUniversal* Camera3 = new CameraInterfaceUniversal(links[2].toStdString(), "[CAMERA3]");

                                         QThread* threadCamera1 = new QThread;
                                         QThread* threadCamera2 = new QThread;
                                         QThread* threadCamera3 = new QThread;

                            Camera1->moveToThread(threadCamera1);
                            Camera2->moveToThread(threadCamera2);
                            Camera3->moveToThread(threadCamera2);

  QObject::connect(threadCamera1, &QThread::started, Camera1, &CameraInterfaceUniversal::slotStartStream);
  QObject::connect(threadCamera2, &QThread::started, Camera2, &CameraInterfaceUniversal::slotStartStream);
  QObject::connect(threadCamera3, &QThread::started, Camera3, &CameraInterfaceUniversal::slotStartStream);

  //QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, Camera1, &CameraInterfaceUniversal::slotEndWork, Qt::QueuedConnection);
  //QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, Camera2, &CameraInterfaceUniversal::slotEndWork, Qt::QueuedConnection);

  QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, threadCamera1, &QThread::quit, Qt::QueuedConnection);
  QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, threadCamera2, &QThread::quit, Qt::QueuedConnection);
  QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, threadCamera3, &QThread::quit, Qt::QueuedConnection);

                          WindowInterface->outputVideo1->linkToSource(Camera1); 
                          WindowInterface->outputVideo2->linkToSource(Camera2); 
                          WindowInterface->outputVideo3->linkToSource(Camera3); 

                          WindowInterface->outputVideo1Control->linkToSource(Camera1);
                          WindowInterface->outputVideo2Control->linkToSource(Camera2);
                          WindowInterface->outputVideo3Control->linkToSource(Camera3);

                          WindowInterface->outputVideo1Mini->linkToSource(Camera1);
                          WindowInterface->outputVideo2Mini->linkToSource(Camera2);
                          WindowInterface->outputVideo3Mini->linkToSource(Camera3);

                          WindowInterface->outputVideoBig->linkToSource(Camera1);

  if(Camera1->isCameraAccessable()) { threadCamera1->start(); WindowInterface->outputVideo1->slotActivate(true); }
  if(Camera2->isCameraAccessable()) { threadCamera2->start(); WindowInterface->outputVideo2->slotActivate(true); }
  if(Camera3->isCameraAccessable()) { threadCamera3->start(); WindowInterface->outputVideo3->slotActivate(true); }

                          WindowInterface->showMaximized();
                        //WindowInterface->showFullScreen();

  app.exec();
}


QStringList LoadCameraLinks()
{
  QStringList DevicesList;
                                   QString settings_file = "./device_list.ini";
if(QFile::exists("../device_list.ini"))    settings_file = "../device_list.ini";
if(QFile::exists("../../device_list.ini")) settings_file = "../../device_list.ini";

if(QFile::exists("/home/broms/SETTINGS/device_list.ini")) settings_file = "/home/broms/SETTINGS/device_list.ini";
if(QFile::exists("D:/SETTINGS/device_list.ini"))          settings_file = "D:/SETTINGS/device_list.ini";

qDebug() << "LOAD CAMERA: " << settings_file;
QFile file(settings_file); file.open(QFile::ReadOnly); if(!file.isOpen()) return DevicesList;

QTextStream text_stream; 
            text_stream.setDevice(&file);

         auto Records = QString(file.readAll()).split("\r\n");
for(auto str: Records) if(str.contains("CAMERA")) DevicesList.append(str.split("|")[1]);


if(DevicesList.size() < 2)
{
  qDebug() << "[ SET DEFAULT CAMERA LINKS ]";
  DevicesList.resize(2);
  DevicesList[0] = "rtsp://192.168.1.11:554/user=admin_password=_channel=1_stream=0.sdp";
  DevicesList[1] = "rtsp://admin:123456@192.168.1.247/live/video";
  //DevicesList[1] = "rtsp://192.168.1.75:8554/test";
}

for(auto Device: DevicesList) qDebug() << "[ CAMERA ] " << Device; 

return DevicesList;
}

  //EnginePort = new UDPConnectionEngine("192.168.0.178",2323); 
  //EnginePort->BindTo(QHostAddress::Any,1313);