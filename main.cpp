
#include <QDebug>
#include <QApplication>
#include <QImage>
#include "widget_rotary_control.h"

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
#include "device_aiming_interface.h"
#include "device_focus_ranger_interface.h"
#include "engine_type_register.h"
#include <memory>
#include "device_lid_interface.h"
#include <QRegularExpression>

#include "message_struct_generic_ext.h"
#include "message_dispatcher_generic.h"
#include "engine_ring_buffer_generic.h"
#include "message_header_generic_ext.h"
#include "engine_uart_interface.h"
#include "engine_can_interface.h"
#include <QPainter>

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

int ID16 = TypeRegister<SystemState  >::RegisterType();
int ID17 = TypeRegister<ControlRX  >::RegisterType();

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

#define MESSAGE_TYPE_1 0
#define MESSAGE_TYPE_2 1
char DataArray[10];

void testArduinoJson()
{
//  JsonDocument doc;
//  const char* input = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
//
//  QString str_input{"{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}"};
//  qDebug() << "TEST1: "<< str_input.toUtf8().toHex();
//
//  QByteArray array((char*)input,str_input.toUtf8().size()); qDebug() << "TEST2: " << array.toHex();

 //QString str = QString::fromUtf8(array);
//  // This MessagePack document contains:
//  // {
//  //   "sensor": "gps",
//  //   "time": 1351824120,
//  //   "data": [48.75608, 2.302038]
//  // }
//
//  DeserializationError error = deserializeJson(doc, str_input.toStdString());
//
//  if (error) { qDebug() << "deserializeMsgPack() failed: " << error.c_str(); }
//
//  const char* sensor = doc["sensor"];
//  long time = doc["time"];
//  double latitude = doc["data"][0];
//  double longitude = doc["data"][1];
//
//  qDebug() << "MSG SIZE: " << doc.size();
//  qDebug() << "SENSOR: " << sensor << "TIME: " << time << "LATITUDE: " << latitude << "LONGITUDE: " << longitude;
//
//  qDebug() << "================================================" << Qt::endl;
//  qDebug() << "================================================" << Qt::endl;

MessageRotaryStateJson MessageRotary;
MessageRotaryStateJson MessageRotary2;
MessageRotaryStateJson MessageRotary3;

QString str1("state:OPERATIONAL, class:TDrive, name:Motor1, status:0, position:799, velocity:23, torque:0, voltage:24590, analogIn:130, digitalIn:4128768");
QString str2("state:OPERATIONAL, class:TDrive, name:Motor2, status:0, position:899, velocity:222, torque:0, voltage:24590, analogIn:130, digitalIn:4128768");
MessageRotary.setParams(str1,str2);

str1 = QString("state:WORK, class:TDrive, name:Motor1, status:1, position:9246, velocity:223, torque:0, voltage:290, analogIn:13, digitalIn:4128");
str2 = QString("state:WORK, class:TDrive, name:Motor2, status:0, position:46, velocity:123, torque:0, voltage:245, analogIn:13, digitalIn:1128768");
MessageRotary2.setParams(str1,str2);

qDebug() << "MESSAGE ROTARY1 SIZE: " << MessageRotary.toByteArray().size();
qDebug() << "MESSAGE ROTARY2 SIZE: " << MessageRotary2.toByteArray().size();

QByteArray array = MessageRotary.toByteArray() + MessageRotary2.toByteArray();
qDebug() << "MESSAGE : " << MessageRotary.toByteArray().toHex();
qDebug() << "MESSAGES: " << array.toHex();

qDebug() << "======================================================" ;
qDebug() << "MESSAGE ROTARY JSON MAX SIZE: " << MessageRotaryStateJson::getMaxSize();
qDebug() << "======================================================" ;

JsonDocument doc_test;

  //DeserializationError error = deserializeJson(doc_test, (uint8_t*)MessageRotary.toByteArray().data());
  DeserializationError error = deserializeJson(doc_test, array.data());

  if (error) { qDebug() << "deserializeMsgPack() failed: " << error.c_str(); }

  const char* pos = doc_test["slaves"][0]["position"];
  long vel        = doc_test["slaves"][1]["velocity"];
  long volt        = doc_test["slaves"][1]["voltage"];

  qDebug() << "MSG SIZE: " << doc_test["slaves"].size();
  qDebug() << "POS     : " << pos << "VEL: " << vel << "VOLT: " << volt;


}


int main(int argc, char* argv[])
{
  QApplication app(argc,argv);

  //app.exec();
  //return 0;

  //printRegisteredTypes();
  //testArduinoJson();

  //ControlPTZCamera PTZDevice;
  //                 PTZDevice.connectToCamera("192.168.1.11", "8899", "admin", "admin");


  WidgetComplexInterface* WindowInterface = new WidgetComplexInterface;
  //=====================================================================================================
  //MESSAGES
  using MessageType1    = MessageGeneric<void*, MESSAGE_HEADER_GENERIC>;
  using MessageType2    = MessageGeneric<void*, MESSAGE_HEADER_ROTARY>;
  using MessageType3    = MessageGeneric<void*, MESSAGE_HEADER_ROTARY_JSON>;
  
  using MessageTypeRotary1    = MessageGeneric<CommandSetPos<0>      , MESSAGE_HEADER_GENERIC    >;
  using MessageTypeRotary2    = MessageGenericExt<ControlRX          , MESSAGE_HEADER_ROTARY     >;
  using MessageTypeRotary3    = MessageGeneric<MessageRotaryStateJson, MESSAGE_HEADER_ROTARY_JSON>;
  //==========================================================================================
  //BUFFERS
  using BufferType1=RingBufferGeneric<MESSAGE_HEADER_GENERIC, MessageTypeRotary1::GetSizeMessage(), 20,IteratorMode::Continous>; 
  using BufferType2=RingBufferGeneric<MESSAGE_HEADER_ROTARY , MessageTypeRotary2::GetSizeMessage(), 12,IteratorMode::Chunked>; 
  using BufferType3=RingBufferGeneric<MESSAGE_HEADER_ROTARY_JSON , 
                                      MESSAGE_HEADER_ROTARY_JSON::getMaxSizeStatic(), 12,IteratorMode::ChunkedContinous>; 

          std::shared_ptr<MessageStorageInterface> RingBuffer1_1 = std::make_shared<BufferType1>();
          std::shared_ptr<MessageStorageInterface> RingBuffer1_2 = std::make_shared<BufferType1>(); 
          std::shared_ptr<MessageStorageInterface> RingBuffer1_3 = std::make_shared<BufferType1>(); 

          std::shared_ptr<MessageStorageInterface> RingBuffer2_1 = std::make_shared<BufferType2>();
          std::shared_ptr<MessageStorageInterface> RingBuffer3_1 = std::make_shared<BufferType3>();
  
  //==========================================================================================
  //DISPATCHERS
  using DispatcherType1 = MessageDispatcher<MESSAGE_HEADER_GENERIC     ,BufferType1>;
  using DispatcherType2 = MessageDispatcher<MESSAGE_HEADER_ROTARY      ,BufferType2>;
  using DispatcherType3 = MessageDispatcher<MESSAGE_HEADER_ROTARY_JSON ,BufferType3>;

          std::shared_ptr<DispatcherType1> Dispatcher1_1 = std::make_shared<DispatcherType1>();
          std::shared_ptr<DispatcherType1> Dispatcher1_2 = std::make_shared<DispatcherType1>();
          std::shared_ptr<DispatcherType1> Dispatcher1_3 = std::make_shared<DispatcherType1>();

          std::shared_ptr<DispatcherType2> Dispatcher2_1 = std::make_shared<DispatcherType2>();
          std::shared_ptr<DispatcherType3> Dispatcher3_1 = std::make_shared<DispatcherType3>();

  //==========================================================================================
  //CONNECTIONS
  std::shared_ptr<UDPConnectionEngine> ConnectionInterface1 = std::make_shared<UDPConnectionEngine>();
                                      *ConnectionInterface1 | RingBuffer1_1 | Dispatcher1_1;

  std::shared_ptr<UDPConnectionEngine> ConnectionInterface2 = std::make_shared<UDPConnectionEngine>();
                                      *ConnectionInterface2 | RingBuffer2_1 | Dispatcher2_1;

  std::shared_ptr<UDPConnectionEngine> ConnectionInterface3 = std::make_shared<UDPConnectionEngine>();
                                      *ConnectionInterface3 | RingBuffer3_1 | Dispatcher3_1;

  std::shared_ptr<UDPConnectionEngine> ConnectionInterface4 = std::make_shared<UDPConnectionEngine>();
                                      *ConnectionInterface4 | RingBuffer2_1 | Dispatcher2_1;

  std::shared_ptr<UDPConnectionEngine> ConnectionInterface5 = std::make_shared<UDPConnectionEngine>();


  ConnectionInterface1->listenTo("192.168.1.200",2323);
  ConnectionInterface1->connectTo("192.168.1.59",2525);

  ConnectionInterface2->listenTo("192.168.1.200",2323);
  ConnectionInterface2->connectTo("192.168.1.58",2525);

  ConnectionInterface3->listenTo("192.168.1.200",2323);
  ConnectionInterface3->connectTo("192.168.1.57",2525);
 
  ConnectionInterface4->listenTo( "192.168.1.200",40805);
  ConnectionInterface4->connectTo("192.168.1.120",40804);

  ConnectionInterface5->listenTo( "192.168.1.200",55217);
  ConnectionInterface5->connectTo("192.168.1.120",9000);


  //==========================================================================================
  //MESSAGE PROCESSING
    Dispatcher1_1->AppendCallback<CommandSetPos<1>> ( [](MessageType1& Message)
    {
     auto data = DispatcherType1::ExtractData<CommandSetPos<1>>(&Message);
     qDebug() << "GET COMMAND: " << data->Param1 << data->Param2;
    });

    Dispatcher1_1->AppendCallback<CommandAiming> ( [WindowInterface](MessageType1& Message)
    {
     auto data = DispatcherType1::ExtractData<CommandAiming>(&Message);
     qDebug() << "GET AIMING STATE: " << data->PosAimingRelativeX << data->PosAimingRelativeY << "[CHANNEL]" << 1;
     WindowInterface->outputVideo1->setCoordPaint(std::pair<float,float>(data->PosAimingRelativeX, data->PosAimingRelativeY));
    });

    Dispatcher1_2->AppendCallback<CommandAiming> ( [WindowInterface](MessageType1& Message)
    {
     auto data = DispatcherType1::ExtractData<CommandAiming>(&Message);
     qDebug() << "GET AIMING STATE: " << data->PosAimingRelativeX << data->PosAimingRelativeY << "[CHANNEL]" << 2;
     WindowInterface->outputVideo2->setCoordPaint(std::pair<float,float>(data->PosAimingRelativeX, data->PosAimingRelativeY));
    });
    
           
    Dispatcher2_1->AppendCallback<ControlRX> ( [](MessageType2& Message) 
    {
        auto data = DispatcherType2::ExtractData<ControlRX>(&Message);
        qDebug() <<  OutputFilter::Filter(1000) << "[ GET CONTROL RX ]" << data->value0.position << data->value1.position;;
    });

                                  MessageRotaryStateJson receiver; 
    Dispatcher3_1->AppendCallback<MessageRotaryStateJson> ( [receiver](MessageType3& Message) mutable
    {
     receiver.loadData(Message.DATA); receiver.printMessage();
    });

  //=====================================================================================================
  //LASERS
  using CommandLaserPointer = MessageGenericExt<CommandDeviceLaserPointer,MESSAGE_HEADER_EXT>;
  using RequestLaserPointer = MessageDevice<1>;

  using CommandLaserPower   = MessageGenericExt<CommandDeviceLaserPower,MESSAGE_HEADER_EXT>;
  using RequestLaserPower   = MessageDevice<2>;

  using CommandFocusRanger   = MessageGenericExt<CommandDeviceFocusator,MESSAGE_HEADER_EXT>;
  using RequestFocusRanger   = MessageDevice<3>;

  DeviceLaserInterface<UDPConnectionEngine,CommandLaserPower,RequestLaserPower>     ControlLaserPower  
  {ConnectionInterface3 , "Силовой лазер"};

  DeviceLaserInterface<UDPConnectionEngine,CommandLaserPointer,RequestLaserPointer> ControlLaserPointer
  {ConnectionInterface3 , "Подсветчик"};

  DeviceFocusRangerInterface<UDPConnectionEngine,CommandFocusRanger,RequestFocusRanger> ControlFocusRanger
  {ConnectionInterface3 , "Фокусатор"};

  //WindowInterface->ControlFocusRanger->linkTo(&ControlFocusRanger);

  //=====================================================================================================
  //LIDS
  QString open1  {"id: 1, type: leftCapOpen"  };
  QString close1 {"id: 1, type: leftCapClose" };
  QString open2  {"id: 1, type: rightCapOpen" };
  QString close2 {"id: 1, type: rightCapClose"};

  std::shared_ptr<DeviceGenericHandleControl> LidControl 
  = std::make_shared<DeviceLidControl<UDPConnectionEngine>>(ConnectionInterface5, open1, close1);

  std::shared_ptr<DeviceGenericHandleControl> LidControl2
  = std::make_shared<DeviceLidControl<UDPConnectionEngine>>(ConnectionInterface5, open2, close2);

  //WindowInterface->ControlLid->linkTo(LidControl,LidControl2);

  //==================================================================================================================
  //ROTARY SCANATOR
  using CommandScanator = MessageGenericExt<CommandSetPosScanator,MESSAGE_HEADER_EXT   >;
  using CommandRotary   = MessageGenericExt<ControlTX            ,MESSAGE_HEADER_ROTARY>;

  auto ControlScanator = std::make_shared<DeviceRotaryControl<UDPConnectionEngine,
                                                                  CommandScanator, MessageStateScanator>>(ConnectionInterface1);

  auto ControlRotary = std::make_shared<DeviceRotaryControl<UDPConnectionEngine,
                                                                  CommandRotary  ,ControlRX>>(ConnectionInterface4);

  using CommandAimingControl = MessageGenericExt<CommandAiming   ,MESSAGE_HEADER_EXT>;
  std::shared_ptr<DeviceGenericHandleControl> ControlAiming1 = std::make_shared<DeviceAimingControl<UDPConnectionEngine, CommandAimingControl, 
                                                                                  CommandAimingControl>>(ConnectionInterface1);
  std::shared_ptr<DeviceGenericHandleControl> ControlAiming2 = std::make_shared<DeviceAimingControl<UDPConnectionEngine, CommandAimingControl, 
                                                                                  CommandAimingControl>>(ConnectionInterface2);
       //WindowInterface->outputVideo1->linkToDevice(ControlAiming1);                                                              
       //WindowInterface->outputVideo1->linkToDevice(ControlAiming2);                                                              


    ControlScanator->setLimits<CONTROL_PARAM::POS>(30000,30000);

    ControlRotary->setLimits<CONTROL_PARAM::POS>(180,180); ControlRotary->setMode(CONTROL_PARAM::POS);
    ControlRotary->setLimits<CONTROL_PARAM::VEL>(10,10 );
    ControlRotary->setToNull();

    //ControlRotary->ModuleMoveSinus.setFreq(0.2);
    //ControlRotary->ModuleMoveSinus.setAmplitude(40);
    //ControlRotary->ModuleMoveSinus.enableMove(true);

                                               //WindowInterface->ControlRotary->linkToDevice(ControlRotary);
                                               //WindowInterface->ControlRotaryPanel->linkToDevice(ControlRotary);

                                               //WindowInterface->ControlRotary->linkToDevice(ControlScanator);
                                               //WindowInterface->ControlScanatorPanel->linkToDevice(ControlScanator);

                                               //WindowInterface->windowControlLaserPower->linkTo(&ControlLaserPower);
                                               //WindowInterface->windowControlLaserIllum->linkTo(&ControlLaserPointer);
  //==================================================================================================================

  //==================================================================================================================
  //CAMERAS
  QStringList links; //links = LoadCameraLinks();
  links.resize(10);
  //links[0] = "rtsp://192.168.1.31:554/user=admin_password=_channel=1_stream=0.sdp";
  //links[0] = "rtspsrc location=rtsp://admin:123456@192.168.1.247/live/video is-live=true latency=1 buffer-mode=auto ! rtph264depay ! h264parse ! openh264dec ! videoconvert ! video/x-raw,format=RGB ! appsink name=sink_node drop=1 sync=0 max-buffers=1 async=1";
  links[0] = "udpsrc port=5000 ! application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96 ! rtph264depay ! h264parse ! openh264dec ! videoconvert n-threads=3 ! video/x-raw,format=RGB ! appsink name=sink_node drop=1 async=false sync=true max-buffers=1";

  //links[0] = "rtspsrc location=rtsp://192.168.1.31:554/user=admin_password=_channel=1_stream=0.sdp drop-on-latency=true is-live=1 latency=10 buffer-mode=auto ! queue ! rtph264depay ! h264parse ! openh264dec ! videoconvert n-threads=1 primaries-mode=fast ! video/x-raw,format=RGB ! appsink name=sink_node drop=1 sync=0 async=1 max-buffers=1";
  links[1] = "rtspsrc location=rtsp://192.168.1.108:554/stream3 latency=10 drop-on-latency=true is-live=true buffer-mode=auto! queue ! rtpjpegdepay ! jpegparse ! jpegdec ! videoconvert ! appsink name=sink_node drop=1 sync=0 max-buffers=1 async=1";
  links[2] = "rtspsrc location=rtsp://192.168.1.59:8554/test latency=10 drop-on-latency=true is-live=true buffer-mode=auto ! queue ! rtph264depay ! h264parse ! openh264dec ! videoconvert ! appsink name=sink_node drop=1 sync=0 max-buffers=1 async=1";
  //links[2] = "rtspsrc location=rtsp://192.168.1.31:554/user=admin_password=_channel=1_stream=0.sdp latency=1 ! queue ! rtph265depay ! h265parse ! d3d11h265dec ! videoconvert ! appsink";
  //links[2] = "rtsp://192.168.1.38:8554/test";

  //links[0] = "videotestsrc pattern=snow ! video/x-raw,format=BGR,width=640,height=480 ! queue ! videoconvert ! appsink name=sink_node";
  //links[1] = "videotestsrc pattern=ball ! video/x-raw,format=BGR,width=640,height=480 ! queue ! videoconvert ! appsink name=sink_node";
  //links[2] = "videotestsrc ! video/x-raw,format=BGR,width=640,height=480 ! queue ! videoconvert ! appsink name=sink_node";


  gst_init(&argc, &argv);
  CameraInterfaceUniversal* Camera1 = new CameraInterfaceUniversal(links[0], "[CAMERA1]");
  CameraInterfaceUniversal* Camera2 = new CameraInterfaceUniversal(links[1], "[CAMERA2]");
  CameraInterfaceUniversal* Camera3 = new CameraInterfaceUniversal(links[2], "[CAMERA3]");

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

  WindowInterface->show();

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

//  //==================================================================
//  QByteArray outputArray;
//  QByteArray outputArray2 = Command.castToByteArray();
//  CommandRotary Command; Command.DATA.setMode(0xBB);
//                Command >> outputArray;
// outputArray2 = Command.castToByteArray();
//  qDebug() << "COMMAND ROTARY DUMP: " << outputArray.toHex() << "SIZE: " << outputArray.size();
//  qDebug() << "========================" ;
//  qDebug() << "COMMAND ROTARY CAST: " << outputArray2.toHex() << "SIZE: " << Command.GetSize() << outputArray2.size();
//  //==================================================================
  //links[0] = "rtsp://192.168.1.21:554/user=admin_password=_channel=1_stream=0.sdp latency=1 buffer-mode=auto ! rtph265depay ! h265parse ! d3d12h265dec ! videoconvert ! appsink";
  //links[0] = "rtsp://192.168.1.108:554/user=admin_password=_channel=1_stream=0.sdp latency=1 buffer-mode=auto ! rtph264depay ! h264parse ! openh264dec ! videoconvert ! appsink";
  //links[0] = "rtspsrc location=rtsp://admin:admin@192.168.1.108:554/stream1 latency=10 drop-on-latency=true is-live=true ! queue ! rtph264depay ! h264parse ! openh264dec ! videoconvert ! appsink sync=false max-buffers=1 async=true";
  //links[0] = "rtsp://admin:123456@192.168.1.247/live/video";
  //links[0] = "rtspsrc location=rtsp://192.168.1.58:8554/test latency=1 ! queue ! rtph264depay ! h264parse ! openh264dec ! videoconvert ! appsrc";
  //links[0] = "rtspsrc location=rtsp://admin:admin@192.168.1.108:554/stream1 latency=1 ! queue ! rtph264depay ! h264parse ! openh264dec ! videoconvert ! appsrc";
  //links[0] = "rtspsrc location=rtsp://admin:admin@192.168.1.108:554/stream1 latency=1 ! queue ! rtph264depay ! h264parse ! openh264dec ! videoconvert ! appsink";
  //links[0] = "rtspsrc location=rtsp://192.168.1.58:8554/test latency=1 ! queue ! rtph264depay ! h264parse ! openh264dec ! videoconvert ! appsink";
  //links[0] = "rtsp://192.168.1.58:8554/test";

//  std::shared_ptr<UARTConnectionEngine> ConnectionInterface4 = std::make_shared<UARTConnectionEngine>();
//                                        ConnectionInterface4->connectTo("COM3",115200);

