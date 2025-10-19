
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

QStringList LoadCameraLinks();

template<> constexpr int TypeRegister<CommandSetPosScanator    >::ID() { return 1; }; int ID1 = TypeRegister<CommandSetPosScanator    >::RegisterType();
template<> constexpr int TypeRegister<CommandSetPosRotary      >::ID() { return 2; }; int ID2 = TypeRegister<CommandSetPosRotary      >::RegisterType();
template<> constexpr int TypeRegister<CommandDeviceController  >::ID() { return 3; }; int ID3 = TypeRegister<CommandDeviceController  >::RegisterType();
template<> constexpr int TypeRegister<CommandDeviceLaserPower  >::ID() { return 4; }; int ID4 = TypeRegister<CommandDeviceLaserPower  >::RegisterType();
template<> constexpr int TypeRegister<CommandDeviceLaserPointer>::ID() { return 5; }; int ID5 = TypeRegister<CommandDeviceLaserPointer>::RegisterType();
template<> constexpr int TypeRegister<CommandDeviceFocusator   >::ID() { return 6; }; int ID6 = TypeRegister<CommandDeviceFocusator   >::RegisterType();
template<> constexpr int TypeRegister<MessageDeviceController  >::ID() { return 7; }; int ID7 = TypeRegister<MessageDeviceController  >::RegisterType();
template<> constexpr int TypeRegister<MessageDeviceLaserPower  >::ID() { return 8; }; int ID8 = TypeRegister<MessageDeviceLaserPower  >::RegisterType();
template<> constexpr int TypeRegister<MessageDeviceLaserPointer>::ID() { return 9; }; int ID9 = TypeRegister<MessageDeviceLaserPointer>::RegisterType();
template<> constexpr int TypeRegister<MessageDeviceFocusator   >::ID() { return 10;}; int ID10 = TypeRegister<CommandSetPosScanator   >::RegisterType();
template<> constexpr int TypeRegister<CommandCalibration       >::ID() { return 11;}; int ID11 = TypeRegister<CommandSetPosScanator   >::RegisterType();
template<> constexpr int TypeRegister<MessagePositionState<0>  >::ID() { return 12;}; int ID12 = TypeRegister<MessagePositionState<0> >::RegisterType();
template<> constexpr int TypeRegister<MessagePositionState<1>  >::ID() { return 20;}; int ID13 = TypeRegister<MessagePositionState<1> >::RegisterType();
template<> constexpr int TypeRegister<CommandCheckConnection   >::ID() { return 24;}; int ID14 = TypeRegister<CommandCheckConnection  >::RegisterType();
template<> constexpr int TypeRegister<CommandCloseConnection   >::ID() { return 25;}; int ID15 = TypeRegister<CommandCloseConnection  >::RegisterType();

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

  QThread* threadCamera1 = new QThread;
  QThread* threadCamera2 = new QThread;
  QByteArray array; array.resize(20);

  //CommandDispatcherGeneric<TypeRegister<CommandSetPosRotary>::ID()>::dispatchCommand(array);
  CommandDispatcherGeneric<TypeRegister<CommandSetPosRotary>::TYPE_ID>::dispatchCommand(array);
  qDebug() << "TYPE COUNT: " << TypeRegister<>::GetTypeCount();
  qDebug() << "TYPE MAX  : " << TypeRegister<>::GetTypeMax();

  int val = 2;
  switch(val)
  {
    case TypeRegister<CommandSetPosRotary>::TYPE_ID: 
    qDebug() << "GET COMMAND ROTARY: " <<TypeRegister<CommandSetPosRotary>::TYPE_ID;
    break;
    qDebug() << "GET COMMAND : " << val;
  };


  //ControlPTZCamera PTZDevice;
  //                 PTZDevice.connectToCamera("192.168.1.11", "8899", "admin", "admin");

  WidgetComplexInterface* WindowInterface = new WidgetComplexInterface;

  TCPConnectionEngine ConnectionInterface;

  DeviceLaserInterface<TCPConnectionEngine,CommandDevice<1>,MessageDevice<1>> ControlLaserPower  {&ConnectionInterface , "[ LPOWER]"};
  DeviceLaserInterface<TCPConnectionEngine,CommandDevice<2>,MessageDevice<2>> ControlLaserPointer{&ConnectionInterface , "[ LPOINTER ]"};
  DeviceFocusRangerInterface<TCPConnectionEngine,CommandDevice<3>,MessageDevice<3>> ControlFocus{&ConnectionInterface , "[ FOCUSATOR ]"};

  DeviceRotaryInterface<TCPConnectionEngine,CommandSetPos<0>,MessageMoveState<0>> ControlRotary  {&ConnectionInterface , "[ ROTARY ]"};
  DeviceRotaryInterface<TCPConnectionEngine,CommandSetPos<1>,MessageMoveState<1>> ControlScanator{&ConnectionInterface , "[ SCANATOR ]"};

  //std::shared_ptr<DeviceRotaryGenericInterface> ptr = std::make_shared<DeviceRotaryInterface<TCPConnectionEngine,CommandSetPos<1>,MessageMoveState<1>>>(nullptr);

    ControlRotary.linkTo(WindowInterface->ControlRotary);
    ControlRotary.linkTo(WindowInterface->ControlRotaryPanel);
  ControlScanator.linkTo(WindowInterface->ControlScanatorPanel);

  //==================================================================================================================
  QStringList links; 
            //links = LoadCameraLinks();
  
  links.resize(3);
  links[0] = "rtsp://192.168.1.11:554/user=admin_password=_channel=1_stream=0.sdp";
  links[1] = "rtsp://192.168.1.75:8554/test";

  //CameraInterfaceUniversal* Camera1 = new CameraInterfaceUniversal(links[0].toStdString(), "[CAMERA1]");
  //CameraInterfaceUniversal* Camera2 = new CameraInterfaceUniversal(links[1].toStdString(), "[CAMERA2]");

  //WindowInterface->outputVideo1->linkToSource(Camera1); WindowInterface->outputVideo1->slotActivate(true);
  //WindowInterface->outputVideo2->linkToSource(Camera2); WindowInterface->outputVideo2->slotActivate(true);

  //WindowInterface->outputVideo1Control->linkToSource(Camera1);
  //WindowInterface->outputVideo2Control->linkToSource(Camera2);

  //WindowInterface->outputVideo1Mini->linkToSource(Camera1);
  //WindowInterface->outputVideo2Mini->linkToSource(Camera2);
  //  WindowInterface->outputVideoBig->linkToSource(Camera1);

  //Camera1->moveToThread(threadCamera1);
  //Camera2->moveToThread(threadCamera2);

  //QObject::connect(threadCamera1, &QThread::started, Camera1, &CameraInterfaceUniversal::slotStartStream);
  //QObject::connect(threadCamera2, &QThread::started, Camera2, &CameraInterfaceUniversal::slotStartStream);
  //QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, Camera1, &CameraInterfaceUniversal::slotEndWork, Qt::QueuedConnection);
  //QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, Camera2, &CameraInterfaceUniversal::slotEndWork, Qt::QueuedConnection);

  QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, threadCamera1, &QThread::quit, Qt::QueuedConnection);
  QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, threadCamera2, &QThread::quit, Qt::QueuedConnection);

  //WindowInterface->showFullScreen();
  WindowInterface->showMaximized();


  //threadCamera1->start();
  //threadCamera2->start();
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

  //EnginePort = new UDPEngineInterface("192.168.0.178",2323); 
  //EnginePort->BindTo(QHostAddress::Any,1313);