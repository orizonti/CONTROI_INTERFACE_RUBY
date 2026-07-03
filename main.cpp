
#include <QDebug>
#include <QApplication>
#include <QImage>
#include "widget_rotary_control.h"

#include <QThread>
#include <tuple>

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

#include <opencv2/videoio.hpp>
#include <qnamespace.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aiming_monitoring_module.h"

#include <QDebug>
#include "interface_camera_rtsp.h"
#include "module_period_measure.h"
#include "nodes_track_approximation.h"
#include "nodes_track_approximation_test.h"

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
#include "widget_line_graph.h"
#include "engine_keyfilter.h"
#include "interface_node_signal_adapter.h"

QStringList LoadCameraLinks();
//#include "message_command_id.h"
#include "engine_statistics_track.h"
#include "widget_device_control2.h"
#include "engine_statistics_track.h"

//=================================================
//COMMAND TO INTERRACT WITH DEVICES VIA PROCESSOR MODULE 

template<> constinit const int TypeRegister<CommandSetPosRotary>      ::TYPE_ID{0 };
template<> constinit const int TypeRegister<CommandSetPosScanator>    ::TYPE_ID{1 };
template<> constinit const int TypeRegister<RequestPosRotary  >       ::TYPE_ID{2 };
template<> constinit const int TypeRegister<RequestPosScanator>       ::TYPE_ID{3 };
template<> constinit const int TypeRegister<CommandAiming1>           ::TYPE_ID{4 };
template<> constinit const int TypeRegister<CommandAiming2>           ::TYPE_ID{5 };
template<> constinit const int TypeRegister<RequestAiming >           ::TYPE_ID{6 };
template<> constinit const int TypeRegister<CommandCheckConnection   >::TYPE_ID{7 };

template<> constinit const int TypeRegister<CommandDevice<0>   >::TYPE_ID{20 };
template<> constinit const int TypeRegister<CommandDevice<1>   >::TYPE_ID{21 };
template<> constinit const int TypeRegister<CommandDevice<2>   >::TYPE_ID{22 };
template<> constinit const int TypeRegister<CommandDevice<3>   >::TYPE_ID{23 };

template<> constinit const int TypeRegister<CommandDeviceLaserPointer>::TYPE_ID{0x110};
template<> constinit const int TypeRegister<CommandDeviceLaserPower  >::TYPE_ID{0x120};
template<> constinit const int TypeRegister<CommandDeviceFocusator   >::TYPE_ID{0x130};
template<> constinit const int TypeRegister<RequestDeviceLaserPointer>::TYPE_ID{0x210};
template<> constinit const int TypeRegister<RequestDeviceLaserPower  >::TYPE_ID{0x220};

template<> constinit const int TypeRegister<SystemState>              ::TYPE_ID{0xA001}; 
template<> constinit const int TypeRegister<ControlRX  >              ::TYPE_ID{0xA002};
template<> constinit const int TypeRegister<ControlTX  >              ::TYPE_ID{0xB001};
template<> constinit const int TypeRegister<MessageRotaryStateJson >  ::TYPE_ID{0x7B22};

template<> class TypeRegisterSizes<sizeof(MESSAGE_HEADER_GENERIC)>
{
  public:
      static constexpr std::array<int,100> SIZES
      {
          sizeof(CommandSetPosRotary      ),
          sizeof(CommandSetPosScanator    ),
          sizeof(RequestPosRotary         ),
          sizeof(RequestPosScanator       ),
          sizeof(CommandAiming1           ),
          sizeof(CommandAiming2           ),
          sizeof(RequestAiming            ),
          sizeof(CommandDeviceLaserPower  ),
          sizeof(CommandDeviceLaserPointer),
          sizeof(CommandDeviceFocusator   ),
          sizeof(RequestDeviceLaserPower  ),
          sizeof(RequestDeviceLaserPointer),
          sizeof(CommandCheckConnection   ),
      };

      static constexpr int HEADER_SIZE = sizeof(MESSAGE_HEADER_GENERIC);
      static constexpr int MinSize = TypeRegisterSizes<0>::GetMinTypeSize(SIZES) + HEADER_SIZE; 
      static constexpr int MaxSize = TypeRegisterSizes<0>::GetMaxTypeSize(SIZES) + HEADER_SIZE; 
};
void RegisterTypes();
//=============================================================================================

//=====================================================================================================
using MessageType1    = MessageGeneric<void*, MESSAGE_HEADER_GENERIC>;
using MessageType2    = MessageGeneric<void*, MESSAGE_HEADER_ROTARY>;
using MessageType3    = MessageGeneric<void*, MESSAGE_HEADER_ROTARY_JSON>;
//=====================================================================================================
using MessageTypeRotary1    = MessageGeneric<CommandSetPair<0>     , MESSAGE_HEADER_GENERIC    >;
using MessageTypeRotary2    = MessageGenericExt<ControlRX          , MESSAGE_HEADER_ROTARY     >;
using MessageTypeRotary3    = MessageGeneric<MessageRotaryStateJson, MESSAGE_HEADER_ROTARY_JSON>;

using CommandScanator = MessageGenericExt<CommandSetPosScanator   ,MESSAGE_HEADER_EXT   >;
using CommandPlatform = MessageGenericExt<ControlTX               ,MESSAGE_HEADER_ROTARY>;
using CommandPlatformProc = MessageGenericExt<CommandSetPosRotary ,MESSAGE_HEADER_EXT>;
//==========================================================================================
using BufferType1=RingBufferGeneric<MESSAGE_HEADER_GENERIC, MessageTypeRotary1::GetSizeMessage(), 30,IteratorMode::Continous>; 
using BufferType2=RingBufferGeneric<MESSAGE_HEADER_ROTARY , MessageTypeRotary2::GetSizeMessage(), 100,IteratorMode::Chunked>; 
using BufferType3=RingBufferGeneric<MESSAGE_HEADER_ROTARY_JSON , 
                                    MESSAGE_HEADER_ROTARY_JSON::getMaxSizeStatic(), 100,IteratorMode::ChunkedContinous>; 
//==========================================================================================
using DispatcherType1 = MessageDispatcher<MESSAGE_HEADER_GENERIC     ,BufferType1>;
using DispatcherType2 = MessageDispatcher<MESSAGE_HEADER_ROTARY      ,BufferType2>;
using DispatcherType3 = MessageDispatcher<MESSAGE_HEADER_ROTARY_JSON ,BufferType3>;

//=====================================================================================================

using DeviceLaserPower   = DeviceLaserInterface      <TCPConnectionEngine,0>; 
using DeviceLaserIllum   = DeviceLaserInterface      <UDPConnectionEngine,1>; 
using DeviceFocusator    = DeviceFocusRangerInterface<UDPConnectionEngine,2>; 
using DeviceScanator     = DeviceRotaryControl<UDPConnectionEngine, CommandScanator    ,RequestPosScanator>;
using DevicePlatformProc = DeviceRotaryControl<UDPConnectionEngine, CommandPlatformProc,RequestPosRotary>;
using DevicePlatform     = DeviceRotaryControl<UDPConnectionEngine, CommandPlatform    ,ControlRX>;
using DeviceLid          = DeviceLidControl   <UDPConnectionEngine>;
//=====================================================================================================


void TestTrackApproximation();

int main(int argc, char* argv[])
{
  QApplication app(argc,argv);
  RegisterTypes();

  //==========================================================================================
  //CONNECTIONS
  std::shared_ptr<UDPConnectionEngine> ConnectionInterface1 = std::make_shared<UDPConnectionEngine>();
  std::shared_ptr<UDPConnectionEngine> ConnectionInterface2 = std::make_shared<UDPConnectionEngine>();
  std::shared_ptr<UDPConnectionEngine> ConnectionInterface3 = std::make_shared<UDPConnectionEngine>();
  std::shared_ptr<UDPConnectionEngine> ConnectionInterface4 = std::make_shared<UDPConnectionEngine>();
  std::shared_ptr<TCPConnectionEngine> ConnectionInterface5 = std::make_shared<TCPConnectionEngine>();

  std::shared_ptr<UDPConnectionEngine> ConnectionTest1 = std::make_shared<UDPConnectionEngine>();
  std::shared_ptr<UDPConnectionEngine> ConnectionTest2 = std::make_shared<UDPConnectionEngine>();

  std::shared_ptr<MessageStorageInterface> RingBuffer1_1 = std::make_shared<BufferType1>();
  std::shared_ptr<MessageStorageInterface> RingBuffer1_2 = std::make_shared<BufferType1>(); 
  std::shared_ptr<MessageStorageInterface> RingBuffer1_3 = std::make_shared<BufferType1>(); 
  std::shared_ptr<MessageStorageInterface> RingBuffer2_1 = std::make_shared<BufferType2>();

  std::shared_ptr<DispatcherType1> Dispatcher1_1 = std::make_shared<DispatcherType1>();
  std::shared_ptr<DispatcherType1> Dispatcher1_2 = std::make_shared<DispatcherType1>();
  std::shared_ptr<DispatcherType1> Dispatcher1_3 = std::make_shared<DispatcherType1>();
  std::shared_ptr<DispatcherType2> Dispatcher2_1 = std::make_shared<DispatcherType2>();

                                      *ConnectionInterface1 | RingBuffer1_1 | Dispatcher1_1;
                                      *ConnectionInterface2 | RingBuffer1_2 | Dispatcher1_2;
                                      *ConnectionInterface3 | RingBuffer1_3 | Dispatcher1_3;
                                      *ConnectionInterface4 | RingBuffer2_1 | Dispatcher2_1;

  QString     ip_terminal = "192.168.1.159";
  QString ip_proc_module1 = "192.168.1.57";
  QString ip_proc_module2 = "192.168.1.75";
  QString ip_proc_module3 = "192.168.1.59";
  QString     ip_platform = "192.168.1.100";
  QString     ip_laser = "192.168.1.10";
  QString     ip_local = "192.168.1.159";

  ConnectionInterface1->listenTo (ip_terminal,2323) ; ConnectionInterface1->connectTo(ip_proc_module1,2525);
  ConnectionInterface2->listenTo (ip_terminal,2324) ; ConnectionInterface2->connectTo(ip_proc_module2,2525);
  ConnectionInterface3->listenTo (ip_terminal,2325) ; ConnectionInterface3->connectTo(ip_proc_module3,2525);
  ConnectionInterface4->listenTo (ip_terminal,40805); ConnectionInterface4->connectTo(ip_platform    ,40804);
  ConnectionInterface5->connectTo(ip_laser   ,10001);

//  ConnectionTest1->listenTo (ip_terminal,5555); ConnectionTest1->connectTo(ip_terminal    ,6666);
//  ConnectionTest2->listenTo (ip_terminal,6666); ConnectionTest2->connectTo(ip_terminal    ,5555);
//  using TypeAimingMonitoring = ModuleAimingMonitoring<UDPConnectionEngine>;
//  std::shared_ptr<TypeAimingMonitoring> AimingMonitorTest = std::make_shared<TypeAimingMonitoring>(ConnectionTest1);
//  *ConnectionTest2 | RingBuffer1_2 | Dispatcher1_2;
//  AimingMonitorTest->startWork(true);

  //=============================================================================================================================================
  std::shared_ptr<DeviceGenericHandleControl> ControlLaserPower   = std::make_shared<DeviceLaserPower>(ConnectionInterface5 , "[LASER_POWER]");
  std::shared_ptr<DeviceGenericHandleControl> ControlFocusator    = std::make_shared<DeviceFocusator >(ConnectionInterface1 , "[FOCUSATOR]");
  std::shared_ptr<DeviceGenericHandleControl> ControlLid          = std::make_shared<DeviceLid>(ConnectionInterface4, "[LIDS]");
  std::shared_ptr<DevicePlatformProc>         ControlPlatform     = std::make_shared<DevicePlatformProc>(ConnectionInterface1, CONTROL_PARAM::POS, "[PLATFORM_PROC]");
  std::shared_ptr<DeviceScanator>             ControlScanator     = std::make_shared<DeviceScanator>(ConnectionInterface2, CONTROL_PARAM::POS, "[SCANATOR]");
  std::shared_ptr<DeviceGenericHandleControl> ControlAiming1      = std::make_shared<DeviceGenericAiming<UDPConnectionEngine>>(ConnectionInterface1, "[AIMING1]");
  std::shared_ptr<DeviceGenericHandleControl> ControlAiming2      = std::make_shared<DeviceGenericAiming<UDPConnectionEngine>>(ConnectionInterface2, "[AIMING2]");

  std::shared_ptr<DeviceGenericHandleControl> ControlAimingParam1 = std::make_shared<DeviceGenericControl<UDPConnectionEngine,0> >(ConnectionInterface1, "[AIMING_PARAM1]");
  std::shared_ptr<DeviceGenericHandleControl> ControlAimingParam2 = std::make_shared<DeviceGenericControl<UDPConnectionEngine,1> >(ConnectionInterface2, "[AIMING_PARAM2]");


  ControlScanator->setLimits({-30000,30000}, {-30000,30000});
  ControlPlatform->setLimits({0,360}, {-20,20}); 

  using Device1  = DeviceGenericControl<ConnectionInterface,0> ; 
  using Device2  = DeviceGenericControl<ConnectionInterface,1> ; 
  using Device3  = DeviceGenericControl<ConnectionInterface,2> ; 
  using DeviceHandle = std::shared_ptr<DeviceGenericHandleControl>;

  DeviceHandle ControlAimingReset1 = std::make_shared<Device1>(ConnectionInterface1 , "[AIMING_RESET_1]");
  DeviceHandle ControlAimingReset2 = std::make_shared<Device1>(ConnectionInterface2 , "[AIMING_RESET_2]");

  DeviceHandle ControlProcessing1 = std::make_shared<Device3>(ConnectionInterface1 , "[CONTROL_PROCESSING_1]");
  DeviceHandle ControlProcessing2 = std::make_shared<Device3>(ConnectionInterface2 , "[CONTROL_PROCESSING_2]");

  //=============================================================================================================================================
  QStringList links; links = LoadCameraLinks();
  gst_init(&argc, &argv);
  CameraInterfaceUniversal* Camera1 = new CameraInterfaceUniversal(links[0], "[CAMERA1]"); Camera1->isCropNeeded = true;  
                            Camera1->setRotate(cv::ROTATE_90_COUNTERCLOCKWISE);
  CameraInterfaceUniversal* Camera2 = new CameraInterfaceUniversal(links[1], "[CAMERA2]"); Camera2->isCropNeeded = true;
                            Camera2->setFlip(true);

  CameraInterfaceUniversal* Camera3 = new CameraInterfaceUniversal(links[2], "[CAMERA3]"); Camera3->isCropNeeded = false; 

  Camera1->setCameraSizes({720,1280}, {640,480});
  Camera2->setCameraSizes({720,540} , {640,480});
  Camera3->setCameraSizes({720,540} , {640,480});

  Camera1->printCameraSizes();
  Camera2->printCameraSizes();

                            Camera1->connectZoomControl("admin","admin","192.168.1.108","80");
                            Camera1->ControlZoom->linkTo(ControlAimingParam1);

  QThread* threadCamera1 = new QThread; Camera1->moveToThread(threadCamera1);
  QThread* threadCamera2 = new QThread; Camera2->moveToThread(threadCamera2);
  QThread* threadCamera3 = new QThread; Camera3->moveToThread(threadCamera3);

  //=============================================================================================================================================
  WidgetComplexInterface* WindowInterface = new WidgetComplexInterface; KeyboardFilter key;
                          WindowInterface->installEventFilter(&key);

       WindowInterface->widgetLidControl->linkToDevice(ControlLid);
  WindowInterface->widgetControlPlatform->linkToDevice(ControlPlatform->ControlRotaryVel);
  WindowInterface->widgetControlScanator->linkToDevice(ControlScanator->ControlRotaryVel);

  linkPeers(WindowInterface->widgetControlPlatform, WindowInterface->widgetMainControl1->widgetAzimuth);
  linkPeers(WindowInterface->widgetControlPlatform, WindowInterface->widgetMainControl1->widgetElevation);

  linkPeers(WindowInterface->widgetControlPlatform, WindowInterface->widgetMainControl2->widgetAzimuth);
  linkPeers(WindowInterface->widgetControlPlatform, WindowInterface->widgetMainControl2->widgetElevation);

  //linkPeers(WindowInterface->widgetMainControl1->widgetAzimuth, WindowInterface->widgetMainControl1->widgetElevation);
  //linkPeers(WindowInterface->widgetMainControl2->widgetAzimuth, WindowInterface->widgetMainControl2->widgetElevation);

  WindowInterface->widgetControlLaserPower->linkToDevice(ControlLaserPower);

        WindowInterface->outputVideo1->linkToDevice(ControlAiming1);
        WindowInterface->outputVideo2->linkToDevice(ControlAiming2);

  WindowInterface->widgetMainControl1->linkToDeviceRotary(ControlPlatform->ControlRotaryPos);
  WindowInterface->widgetMainControl1->linkToDevice(ControlLid,0);
  WindowInterface->widgetMainControl1->linkToDevice(ControlProcessing1,1);
  WindowInterface->widgetMainControl1->linkToDevice(ControlProcessing2,1);

  WindowInterface->widgetMainControl1->linkToDevice(ControlAimingReset1,2);
  WindowInterface->widgetMainControl1->linkToDevice(ControlAimingReset2,2);
  WindowInterface->widgetMainControl1->linkToDevice(ControlLaserPower  ,3);
  WindowInterface->widgetMainControl1->linkButtons();

  WindowInterface->widgetMainControl2->linkToDevice(ControlLid,0);
  WindowInterface->widgetMainControl2->linkToDevice(ControlProcessing1,1);
  WindowInterface->widgetMainControl2->linkToDevice(ControlProcessing2,1);

  WindowInterface->widgetMainControl2->linkToDevice(ControlAimingReset1,2);
  WindowInterface->widgetMainControl2->linkToDevice(ControlAimingReset2,2);
  WindowInterface->widgetMainControl2->linkToDevice(ControlLaserPower  ,3);
  WindowInterface->widgetMainControl1->linkButtons();


  float X_NULL = 263.827;
  float Y_NULL = 0.964;
  WindowInterface->widgetMainControl1->widgetAzimuth->setNull(X_NULL);
  WindowInterface->widgetMainControl1->widgetElevation->setNull(Y_NULL);

  WindowInterface->widgetMainControl2->widgetAzimuth->setNull(X_NULL);
  WindowInterface->widgetMainControl2->widgetElevation->setNull(Y_NULL);

  //=============================================================================================================================================

  WindowInterface->widgetControlCamera3Float->linkToDevice(Camera1->ControlZoom);
  QObject::connect(threadCamera1, &QThread::started, Camera1, &CameraInterfaceUniversal::slotStartStream);
  QObject::connect(threadCamera2, &QThread::started, Camera2, &CameraInterfaceUniversal::slotStartStream);
  QObject::connect(threadCamera3, &QThread::started, Camera3, &CameraInterfaceUniversal::slotStartStream);

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

  //==================================================================================================================
    Dispatcher1_1->AppendCallback<CommandCheckConnection> ( [](MessageType1& Message) { });
    Dispatcher1_2->AppendCallback<CommandCheckConnection> ( [Camera3](MessageType1& Message) { Camera3->reset(); });
    Dispatcher1_3->AppendCallback<CommandCheckConnection> ( [](MessageType1& Message) { });

    Dispatcher1_1->AppendCallback<CommandAiming<0>> ( [WindowInterface](MessageType1& Message)
    {
     auto data = DispatcherType1::ExtractData<CommandAiming<0>>(&Message);
     //qDebug() << OutputFilter::Filter(30) << "GET AIMING STATE: " << data->Command.first << data->Command.second << "[CHANNEL]" << 1;
     WindowInterface->outputVideo1->enablePlot(true);
     WindowInterface->outputVideo1->setCoordPaint(data->Command,0);
    });

    Dispatcher1_2->AppendCallback<CommandAiming<0>> ( [WindowInterface](MessageType1& Message)
    {
     auto data = DispatcherType1::ExtractData<CommandAiming<0>>(&Message);
     //qDebug() << OutputFilter::Filter(30) << "GET AIMING STATE: " << data->Command.first << data->Command.second << "[CHANNEL]" << 2;
     WindowInterface->outputVideo2->enablePlot(true);
     WindowInterface->outputVideo2->setCoordPaint(data->Command,0);
    }); 

    Dispatcher1_3->AppendCallback<CommandAiming<0>> ( [WindowInterface](MessageType1& Message)
    {
     auto data = DispatcherType1::ExtractData<CommandAiming<0>>(&Message);
     //qDebug() << OutputFilter::Filter(30) << "GET AIMING STATE: " << data->Command.first << data->Command.second << "[CHANNEL]" << 3;
     WindowInterface->outputVideo2->enablePlot(true);
     WindowInterface->outputVideo2->setCoordPaint(data->Command,0);
    });


    Dispatcher2_1->AppendCallback<ControlRX> ( [](MessageType2& Message) mutable 
    {
        //auto data = DispatcherType2::ExtractData<ControlRX>(&Message);
        //qDebug() <<  OutputFilter::Filter(6000) << "[ GET CONTROL RX ]" << data->value0.position << data->value1.position;;
    });

//    Dispatcher1_1->AppendCallback<CommandCheckConnection> ( [](MessageType1& Message) mutable { qDebug() << "CHECK CONNECTION 1"; });
//    Dispatcher1_2->AppendCallback<CommandCheckConnection> ( [](MessageType1& Message) mutable { qDebug() << "CHECK CONNECTION 2"; });
//    Dispatcher1_3->AppendCallback<CommandCheckConnection> ( [](MessageType1& Message) mutable { qDebug() << "CHECK CONNECTION 3"; });
//
//    Dispatcher1_1->AppendCallback<CommandDeviceLaserPointer> ( [](MessageType1& Message) mutable { qDebug() << "LASER POINTER REQUEST"; });
//    Dispatcher1_1->AppendCallback<CommandDeviceLaserPower> ( [](MessageType1& Message) mutable { qDebug() << "LASER POINTER REQUEST"; });
//
//    Dispatcher1_2->AppendCallback<CommandDeviceLaserPointer> ( [](MessageType1& Message) mutable { qDebug() << "LASER POINTER REQUEST"; });
//    Dispatcher1_2->AppendCallback<CommandDeviceLaserPower> ( [](MessageType1& Message) mutable { qDebug() << "LASER POINTER REQUEST"; });



  //==================================================================================================================
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
for(auto str: Records) if(str.contains("CAMERA")) DevicesList.append(str.split("|")[1].remove('"'));

for(auto Device: DevicesList) qDebug() << "[ CAMERA ] " << Device; 

return DevicesList;
}

void RegisterTypes()
{
  TypeRegister<CommandSetPosRotary  >    ::registerType("SET_POS_ROTARY"); 
  TypeRegister<CommandSetPosScanator>    ::registerType("SET_POS_SCANATOR");
  TypeRegister<RequestPosRotary  >       ::registerType("REQUEST_POS_SCANATOR");
  TypeRegister<RequestPosScanator>       ::registerType("REQUEST_POS_SCANATOR");
  TypeRegister<CommandAiming1>           ::registerType("COMMAND_AIMING_1");
  TypeRegister<CommandAiming2>           ::registerType("COMMAND_AIMING_2");
  TypeRegister<RequestAiming >           ::registerType("REQUEST_AIMING");
  TypeRegister<CommandDeviceLaserPower  >::registerType("COMMAND_LASER_POWER");
  TypeRegister<CommandDeviceLaserPointer>::registerType("COMMAND_LASER_POINTER");
  TypeRegister<CommandDeviceFocusator   >::registerType("COMMAND_LASER_FOCUSATOR");
  TypeRegister<RequestDeviceLaserPower  >::registerType("REQUEST_LASER_POWER");
  TypeRegister<RequestDeviceLaserPointer>::registerType("REQUEST_LASER_POINTER");
   TypeRegister<CommandCheckConnection  >::registerType("CHECK_CONNECTION");
                TypeRegister<SystemState>::registerType("SystemStateRotary"); 
               TypeRegister <ControlTX  >::registerType("ControlTXRotary");
                TypeRegister<ControlRX  >::registerType("ControlRXRotary");

  TypeRegister<>::TYPES_INFO.printTypesSignature();

}

void TestTrackApproximation()
{
  //=====================================================================================
  //TEST TRACK APPROXIMATION 
  SinusGeneratorClass SinusGenerator; SinusGenerator.setPeriod(3);
                                      SinusGenerator.slotSetAmplitude(200); 
                                      SinusGenerator.slotSetFrequency(0.09);
                                      SinusGenerator.slotSetOffset(240,240);
  WidgetLineGraph graphWidget; graphWidget.setSize(500); graphWidget.show();
  
  float amp1 = 12; float amp2 = 12;
  NodeCoordSplitToTime<float> SplitToTime; 
  NodeCoordRandomizer<float> Randomize {amp1,amp2};
  NodeCoordRandomizer<float> Randomize2{amp1,amp2};
  NodeCoordPassNorm<float> Norm;
  NodeCoordSwap SwapCoord; 

          //======================================================================================
          //STATIC TEST
          //PolynomApproximationTest<3> Test1(12,0.0001,0.002,0);
          //PolynomApproximationTest<2> Test1(2,1.2,0.000,0);
          //                graphWidget.GraphPointsStorage2->setPoints(Test1.TrackApprox);
          //                graphWidget.GraphPointsStorage->setPoints(Test1.TrackNoize);
          //======================================================================================
          //DYNAMIC TEST
          //PolynomApproximationDynamicTest<3> Test2{140,10,20};
          //                    SplitToTime.setResetCounter(SinusGenerator.Period.first);
          //                    SinusGenerator | Randomize2 | Test2;
          //                                      Test2.linkToGraph(graphWidget.GraphPointsStorage2);
          //                                      Test2.linkToGraph(graphWidget.GraphPointsStorage );
          //=====================================================================================
          //DISPERSION ESTIMATE BY APPROXIMATION
                      //EstimatorObjectMovingParams TrackStat;
          //   EstimatorTrackHold<float> TrackHold;
          // SinusGenerator | Randomize | TrackHold;
          //=====================================================================================
          //SinusGenerator.slotStartGenerate(true);


  //return app.exec();
  //=====================================================================================
  

}
  //QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, Camera1, &CameraInterfaceUniversal::slotEndWork, Qt::QueuedConnection);
  //QObject::connect(WindowInterface, &WidgetComplexInterface::signalEndWork, Camera2, &CameraInterfaceUniversal::slotEndWork, Qt::QueuedConnection);