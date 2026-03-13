
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

int ID1 = TypeRegister<CommandSetPair<0>  >::RegisterType();
int ID2 = TypeRegister<CommandSetPair<1>  >::RegisterType();
int ID10 = TypeRegister<CommandSetPair<2> >::RegisterType();
int ID11 = TypeRegister<CommandSetPair<3> >::RegisterType();
//int ID3 = TypeRegister<CommandDeviceController  >::RegisterType();
int ID4 = TypeRegister<CommandDeviceLaserPower  >::RegisterType();
int ID5 = TypeRegister<CommandDeviceLaserPointer>::RegisterType();
int ID6 = TypeRegister<CommandDeviceFocusator   >::RegisterType();
int ID7 = TypeRegister<RequestDeviceController  >::RegisterType();
int ID8 = TypeRegister<RequestDeviceLaserPower  >::RegisterType();
int ID9 = TypeRegister<RequestDeviceLaserPointer>::RegisterType();
int ID12 = TypeRegister<RequestPositionState<0> >::RegisterType();
int ID13 = TypeRegister<RequestPositionState<1> >::RegisterType();

int ID14 = TypeRegister<CommandCheckConnection  >::RegisterType();
int ID15 = TypeRegister<CommandCloseConnection  >::RegisterType();

int ID16 = TypeRegister<SystemState  >::RegisterType();
int ID17 = TypeRegister<ControlRX  >::RegisterType();

void printRegisteredTypes()
{
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandSetPosScanator>::TYPE_ID).arg(TypeRegister<CommandSetPosScanator>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandSetPosRotary>::TYPE_ID).arg(TypeRegister<CommandSetPosRotary>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandDevice<0>>::TYPE_ID).arg(TypeRegister<CommandDevice<0>>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandDevice<1>>::TYPE_ID).arg(TypeRegister<CommandDevice<1>>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandDevice<2>>::TYPE_ID).arg(TypeRegister<CommandDevice<2>>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandDevice<3>>::TYPE_ID).arg(TypeRegister<CommandDevice<3>>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<RequestDevice<0>>::TYPE_ID).arg(TypeRegister<RequestDevice<0>>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<RequestDevice<1>>::TYPE_ID).arg(TypeRegister<RequestDevice<1>>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<RequestDevice<2>>::TYPE_ID).arg(TypeRegister<RequestDevice<2>>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<RequestDevice<3>>::TYPE_ID).arg(TypeRegister<RequestDevice<3>>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandCalibration>::TYPE_ID).arg(TypeRegister<CommandCalibration>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<RequestPositionState<0>>::TYPE_ID).arg(TypeRegister<RequestPositionState<0>>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<RequestPositionState<1>>::TYPE_ID).arg(TypeRegister<RequestPositionState<0>>::GetTypeSize());
//  qDebug() << QString("REGISTER TYPE %1 SIZE %2").arg(TypeRegister<CommandCheckConnection>::TYPE_ID).arg(TypeRegister<CommandCheckConnection>::GetTypeSize());
}

template<> void CommandDispatcherGeneric<TypeRegister<CommandDevice<0>>::ID()>::dispatchCommand(const QByteArray& Command) 
{
  qDebug() << "DISPATCH COMMAND: " << TypeRegister<CommandDevice<0>>::TYPE_ID << " DEV 0";
};

template<> void CommandDispatcherGeneric<TypeRegister<CommandSetPair<0>>::ID()>::dispatchCommand(const QByteArray& Command) 
{
  qDebug() << "DISPATCH COMMAND: " << TypeRegister<CommandSetPair<0>>::TYPE_ID << " POS ROTARY";
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

//  WidgetDeviceControl wid{"Device", Qt::Vertical};
//  wid.enableScheme(0,1,1,0,0,0); wid.setScheme(5,0,0);
//  wid.show();
//  app.exec();
//  return 0;

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
  
  using MessageTypeRotary1    = MessageGeneric<CommandSetPair<0>     , MESSAGE_HEADER_GENERIC    >;
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
    Dispatcher1_1->AppendCallback<CommandSetPair<1>> ( [](MessageType1& Message)
    {
     auto data = DispatcherType1::ExtractData<CommandSetPair<1>>(&Message);
     qDebug() << "GET COMMAND: " << data->Param1 << data->Param2;
    });

    Dispatcher1_1->AppendCallback<CommandSetPair<0>> ( [WindowInterface](MessageType1& Message)
    {
     auto data = DispatcherType1::ExtractData<CommandSetPair<0>>(&Message);
     qDebug() << "GET AIMING STATE: " << data->Param1 << data->Param2 << "[CHANNEL]" << 1;
     WindowInterface->outputVideo1->setCoordPaint(std::pair<float,float>(data->Param1, data->Param2));
    });

    Dispatcher1_2->AppendCallback<CommandSetPair<1>> ( [WindowInterface](MessageType1& Message)
    {
     auto data = DispatcherType1::ExtractData<CommandSetPair<1>>(&Message);
     qDebug() << "GET AIMING STATE: " << data->Param1 << data->Param2 << "[CHANNEL]" << 2;
     WindowInterface->outputVideo2->setCoordPaint(std::pair<float,float>(data->Param1, data->Param2));
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
  //DEVICES
  using DeviceAiming1 = DeviceGenericAiming<UDPConnectionEngine, 0>;
  using DeviceAiming2 = DeviceGenericAiming<UDPConnectionEngine, 1>;

  using DeviceLaserPower = DeviceLaserInterface      <UDPConnectionEngine,1>; 
  using DeviceLaserIllum = DeviceLaserInterface      <UDPConnectionEngine,2>; 
  using DeviceFocusator  = DeviceFocusRangerInterface<UDPConnectionEngine,3> ; 

                           using CommandScanator = MessageGenericExt<CommandSetPair<0>, MESSAGE_HEADER_EXT   >;
                           using CommandPlatform = MessageGenericExt<ControlTX        , MESSAGE_HEADER_ROTARY>;
  using DeviceScanator = DeviceRotaryControl<UDPConnectionEngine, CommandScanator, RequestStateScanator>;
  using DevicePlatform = DeviceRotaryControl<UDPConnectionEngine, CommandPlatform, ControlRX>;

  using DeviceLid        = DeviceLidControl<UDPConnectionEngine>;
  //=====================================================================================================

  //LASERS
  std::shared_ptr<DeviceGenericHandleControl> ControlLaserPower = std::make_shared<DeviceLaserPower>(ConnectionInterface3 , "Силовой лазер");
  std::shared_ptr<DeviceGenericHandleControl> ControlLaserIllum = std::make_shared<DeviceLaserIllum>(ConnectionInterface3 , "Подсветчик");
  //FOCUS CONTROL
  std::shared_ptr<DeviceGenericHandleControl> ControlFocusator  = std::make_shared<DeviceFocusator >(ConnectionInterface3 , "Фокусатор");
  //=====================================================================================================
  //LIDS
  std::shared_ptr<DeviceGenericHandleControl> ControlLid  = std::make_shared<DeviceLid>(ConnectionInterface5);
  //==================================================================================================================
  //ROTARY SCANATOR
  std::shared_ptr<DeviceScanator> ControlScanator = std::make_shared<DeviceScanator>(ConnectionInterface1, CONTROL_PARAM::POS);
  std::shared_ptr<DevicePlatform> ControlPlatform = std::make_shared<DevicePlatform>(ConnectionInterface4, CONTROL_PARAM::VEL);

  std::shared_ptr<DeviceGenericHandleControl> ControlAiming1 = std::make_shared<DeviceGenericAiming<UDPConnectionEngine, 0>>(ConnectionInterface1);
  std::shared_ptr<DeviceGenericHandleControl> ControlAiming2 = std::make_shared<DeviceGenericAiming<UDPConnectionEngine, 1>>(ConnectionInterface2);

    ControlScanator->setLimits<CONTROL_PARAM::POS>(30000,30000);
    ControlPlatform->setLimits<CONTROL_PARAM::POS>(180,180); ControlPlatform->setMode(CONTROL_PARAM::POS);
    ControlPlatform->setLimits<CONTROL_PARAM::VEL>(10,10 );
    ControlPlatform->setToNull();

  WindowInterface->widgetLidControl->linkToDevice(ControlLid);

  WindowInterface->widgetControlPlatform->linkToDevice(ControlPlatform->ControlRotaryVel);
  WindowInterface->widgetControlScanator->linkToDevice(ControlScanator->ControlRotaryVel);

  WindowInterface->widgetControlRotary1->linkToDevice(ControlPlatform->ControlRotaryPos);
  WindowInterface->widgetControlRotary2->linkToDevice(ControlScanator->ControlRotaryPos);

  WindowInterface->widgetControlLaserPower->linkToDevice(ControlLaserPower);
  WindowInterface->widgetControlLaserIllum->linkToDevice(ControlLaserIllum);

  WindowInterface->outputVideo1->linkToDevice(ControlAiming1);
  WindowInterface->outputVideo2->linkToDevice(ControlAiming2);
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
//  CommandPlatform Command; Command.DATA.setMode(0xBB);
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


    //ControlRotary->ModuleMoveSinus.setFreq(0.2);
    //ControlRotary->ModuleMoveSinus.setAmplitude(40);
    //ControlRotary->ModuleMoveSinus.enableMove(true);