
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

QStringList LoadCameraLinks();

int main(int argc, char* argv[])
{
  QApplication app(argc,argv);

  QThread* threadCamera1 = new QThread;
  QThread* threadCamera2 = new QThread;

  //ControlPTZCamera PTZDevice;
  //                 PTZDevice.ConnectToCamera("192.168.1.11", "8899", "admin", "admin");

  WidgetComplexInterface* WindowInterface = new WidgetComplexInterface;
  //==================================================================================================================
  QStringList links; 
            //links = LoadCameraLinks();
  
  links.resize(3);
  links[0] = "rtsp://192.168.1.11:554/user=admin_password=_channel=1_stream=0.sdp";
  links[1] = "rtsp://192.168.1.75:8554/test";

  CameraInterfaceUniversal* Camera1 = new CameraInterfaceUniversal(links[0].toStdString(), "[CAMERA1]");
  CameraInterfaceUniversal* Camera2 = new CameraInterfaceUniversal(links[1].toStdString(), "[CAMERA2]");

  WindowInterface->outputVideo1->linkToSource(Camera1); WindowInterface->outputVideo1->slotActivate(true);
  WindowInterface->outputVideo2->linkToSource(Camera2); WindowInterface->outputVideo2->slotActivate(true);

  WindowInterface->outputVideo1Control->linkToSource(Camera1);
  WindowInterface->outputVideo2Control->linkToSource(Camera2);

  WindowInterface->outputVideo1Mini->linkToSource(Camera1);
  WindowInterface->outputVideo2Mini->linkToSource(Camera2);
    WindowInterface->outputVideoBig->linkToSource(Camera1);

  Camera1->moveToThread(threadCamera1);
  Camera2->moveToThread(threadCamera2);

  QObject::connect(threadCamera1, &QThread::started, Camera1, &CameraInterfaceUniversal::slotStartStream);
  QObject::connect(threadCamera2, &QThread::started, Camera2, &CameraInterfaceUniversal::slotStartStream);
  QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, Camera1, &CameraInterfaceUniversal::slotEndWork, Qt::QueuedConnection);
  QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, Camera2, &CameraInterfaceUniversal::slotEndWork, Qt::QueuedConnection);

  QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, threadCamera1, &QThread::quit, Qt::QueuedConnection);
  QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, threadCamera2, &QThread::quit, Qt::QueuedConnection);

  //WindowInterface->showFullScreen();
  WindowInterface->showMaximized();

  threadCamera1->start();
  threadCamera2->start();
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