
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

#include <QDebug>
#include "interface_camera_rtsp.h"
#include "module_period_measure.h"
#include "nodes_track_approximation.h"
#include "nodes_track_approximation_test.h"

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
#include "widget_line_graph.h"
#include "engine_keyfilter.h"
#include "interface_node_synchronizer.h"

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

template<> constinit const int TypeRegister<CommandDeviceLaserPointer>::TYPE_ID{0x110};
template<> constinit const int TypeRegister<CommandDeviceLaserPower  >::TYPE_ID{0x120};
template<> constinit const int TypeRegister<CommandDeviceFocusator   >::TYPE_ID{0x130};
template<> constinit const int TypeRegister<RequestDeviceLaserPointer>::TYPE_ID{0x210};
template<> constinit const int TypeRegister<RequestDeviceLaserPower  >::TYPE_ID{0x220};
template<> constinit const int TypeRegister<CommandCheckConnection   >::TYPE_ID{0x230};

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
//=============================================================================================


int main(int argc, char* argv[])
{
  QApplication app(argc,argv);
  //WidgetDeviceControl2 wid; wid.setScheme(2,2,2,2,0);
  //wid.show();
  //return app.exec();

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

  MeasurePeriodNode periodMeasure;

  //=====================================================================================
  //TEST TRACK APPROXIMATION 
  SinusGeneratorClass SinusGenerator; SinusGenerator.setPeriod(3);
                                      SinusGenerator.slotSetAmplitude(200); 
                                      SinusGenerator.slotSetFrequency(0.001);
                                      SinusGenerator.slotSetOffset(240,240);
  WidgetLineGraph graphWidget; graphWidget.setSize(500); graphWidget.show();
  
  NodeCoordSplitToTime<float> SplitToTime; 
  NodeCoordRandomizer<float> Randomize{10,4};
  NodeCoordRandomizer<float> Randomize2{0,2};
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
            PolynomApproximationDynamicTest<3> Test2{100,10,4};
                                SplitToTime.setResetCounter(SinusGenerator.Period.first);
                                SinusGenerator | Test2;
                                                  Test2.linkToGraph(graphWidget.GraphPointsStorage2);
                                                  Test2.linkToGraph(graphWidget.GraphPointsStorage );
            //======================================================================================

            //  DetectorTrackHold ProcessorTrack;
            //  NodeCoordRandomizer<float> Randomizer{12};
            //  NodeCoordPassValue<float> PassValue;
            //  NodeCoordPassValue<float> PassValue2;
            //  SinusGenerator | Randomizer | ProcessorTrack | PassValue2(0) | graphWidget(0);
            //                   //Randomizer | PassValue(0) | graphWidget(1);
            //
            //  SinusGenerator.slotStartGenerate(true);
            //
            //=====================================================================================
            //DISPERSION ESTIMATE BY SPAN
            //StatisticCoordSpan<float,2> CoordSpan2(10);
            //StatisticCoordSpan<float,1> CoordSpan1(10);
            //SinusGenerator | Randomize | CoordSpan2;
            //SinusGenerator | Randomize | Norm | CoordSpan1;
            //=====================================================================================
            //DISPERSION ESTIMATE BY APPROXIMATION
//                        DetectorTrackHold TrackStat;
//             SinusGenerator | Randomize | TrackStat;
            //=====================================================================================

            SinusGenerator.slotStartGenerate(true);


  return app.exec();
  //=====================================================================================
  
  //TypeRegister<>::printRegisteredTypes();

  //ControlPTZCamera PTZDevice;
  //                 PTZDevice.connectToCamera("192.168.1.11", "8899", "admin", "admin");



  WidgetComplexInterface* WindowInterface = new WidgetComplexInterface;

                         KeyboardFilter key;
   WindowInterface->installEventFilter(&key);
  
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
                                      *ConnectionInterface2 | RingBuffer1_2 | Dispatcher1_2;

  std::shared_ptr<UDPConnectionEngine> ConnectionInterface3 = std::make_shared<UDPConnectionEngine>();
                                      *ConnectionInterface3 | RingBuffer1_3 | Dispatcher1_3;

  std::shared_ptr<UDPConnectionEngine> ConnectionInterface4 = std::make_shared<UDPConnectionEngine>();
                                      *ConnectionInterface4 | RingBuffer2_1 | Dispatcher2_1;

  std::shared_ptr<UDPConnectionEngine> ConnectionInterface5 = std::make_shared<UDPConnectionEngine>();

  QString ip_terminal = "192.168.1.200";
  QString ip_proc_module1 = "192.168.1.57";
  QString ip_proc_module2 = "192.168.1.75";
  QString ip_proc_module3 = "192.168.1.59";

  QString ip_platform = "192.168.1.120";

  ConnectionInterface1->listenTo (ip_terminal    ,2323);
  ConnectionInterface1->connectTo(ip_proc_module1,2525);

  ConnectionInterface2->listenTo (ip_terminal    ,2323);
  ConnectionInterface2->connectTo(ip_proc_module2,2525);

  ConnectionInterface3->listenTo (ip_terminal    ,2323);
  ConnectionInterface3->connectTo(ip_proc_module3,2525);
 
  ConnectionInterface4->listenTo (ip_terminal,40805);
  ConnectionInterface4->connectTo(ip_platform,40804);

  ConnectionInterface5->listenTo (ip_terminal,55217);
  ConnectionInterface5->connectTo(ip_platform,9000 );


  //==========================================================================================
  //MESSAGE PROCESSING
    Dispatcher1_1->AppendCallback<CommandAiming<0>> ( [WindowInterface](MessageType1& Message)
    {
     auto data = DispatcherType1::ExtractData<CommandAiming<0>>(&Message);
     //qDebug() << "GET AIMING STATE: " << data->Command.first << data->Command.second << "[CHANNEL]" << 1;
     WindowInterface->outputVideo1->enablePlot(true);
     WindowInterface->outputVideo1->setCoordPaint(data->Command,0);
    });

    Dispatcher1_2->AppendCallback<CommandAiming<0>> ( [WindowInterface](MessageType1& Message)
    {
     auto data = DispatcherType1::ExtractData<CommandAiming<0>>(&Message);
     //qDebug() << "GET AIMING STATE: " << data->Command.first << data->Command.second << "[CHANNEL]" << 2;
     WindowInterface->outputVideo2->enablePlot(true);
     WindowInterface->outputVideo2->setCoordPaint(data->Command,0);
    });

    Dispatcher1_3->AppendCallback<CommandAiming<0>> ( [WindowInterface](MessageType1& Message)
    {
     auto data = DispatcherType1::ExtractData<CommandAiming<0>>(&Message);
     //qDebug() << "GET AIMING STATE: " << data->Command.first << data->Command.second << "[CHANNEL]" << 3;
     WindowInterface->outputVideo3->enablePlot(true);
     WindowInterface->outputVideo3->setCoordPaint(data->Command,0);
    });

           
    Dispatcher2_1->AppendCallback<ControlRX> ( [](MessageType2& Message) 
    {
        auto data = DispatcherType2::ExtractData<ControlRX>(&Message);
        //qDebug() <<  OutputFilter::Filter(1000) << "[ GET CONTROL RX ]" << data->value0.position << data->value1.position;;
    });

                                  MessageRotaryStateJson receiver; 
    Dispatcher3_1->AppendCallback<MessageRotaryStateJson> ( [receiver](MessageType3& Message) mutable
    {
     //receiver.loadData<500>(Message.DATA); receiver.printMessage();
    });

  //=====================================================================================================
  //DEVICES
  using DeviceLaserPower = DeviceLaserInterface      <UDPConnectionEngine,0>; 
  using DeviceLaserIllum = DeviceLaserInterface      <UDPConnectionEngine,1>; 
  using DeviceFocusator  = DeviceFocusRangerInterface<UDPConnectionEngine,2> ; 

                           using CommandScanator = MessageGenericExt<CommandSetPosScanator, MESSAGE_HEADER_EXT   >;
                           using CommandPlatform = MessageGenericExt<ControlTX            , MESSAGE_HEADER_ROTARY>;

  using DeviceScanator = DeviceRotaryControl<UDPConnectionEngine, CommandScanator, RequestPosScanator>;
  using DevicePlatform = DeviceRotaryControl<UDPConnectionEngine, CommandPlatform, ControlRX>;

  using DeviceLid        = DeviceLidControl<UDPConnectionEngine>;
  //=====================================================================================================

  //LASERS
  std::shared_ptr<DeviceGenericHandleControl> ControlLaserPower = std::make_shared<DeviceLaserPower>(ConnectionInterface3 , "[LASER_POWER]");
  std::shared_ptr<DeviceGenericHandleControl> ControlLaserIllum = std::make_shared<DeviceLaserIllum>(ConnectionInterface3 , "[LASER_POINTER]");
  //FOCUS CONTROL
  std::shared_ptr<DeviceGenericHandleControl> ControlFocusator  = std::make_shared<DeviceFocusator >(ConnectionInterface3 , "[FOCUSATOR]");
  //=====================================================================================================
  //LIDS
  std::shared_ptr<DeviceGenericHandleControl> ControlLid  = std::make_shared<DeviceLid>(ConnectionInterface5, "[LIDS]");
  //==================================================================================================================
  //ROTARY SCANATOR
  std::shared_ptr<DeviceScanator> ControlScanator = std::make_shared<DeviceScanator>(ConnectionInterface2, CONTROL_PARAM::POS, "[SCANATOR]");
  std::shared_ptr<DevicePlatform> ControlPlatform = std::make_shared<DevicePlatform>(ConnectionInterface4, CONTROL_PARAM::POS, "[PLATFORM]");

  std::shared_ptr<DeviceGenericHandleControl> ControlAiming1 = std::make_shared<DeviceGenericAiming<UDPConnectionEngine>>(ConnectionInterface3, "[AIMING1]");
  std::shared_ptr<DeviceGenericHandleControl> ControlAiming2 = std::make_shared<DeviceGenericAiming<UDPConnectionEngine>>(ConnectionInterface3, "[AIMING2]");

    ControlScanator->setLimits(CONTROL_PARAM::POS,30000,30000);

    ControlPlatform->setLimits(CONTROL_PARAM::POS,360,20); 
    ControlPlatform->setNull(QPair<float,float>(73.48,-2.57));

    WindowInterface->widgetMainControl1->widgetAzimuth->setNullDevice(73.48);
    WindowInterface->widgetMainControl1->widgetElevation->setNullDevice(-2.57);

    WindowInterface->widgetMainControl2->widgetAzimuth->setNullDevice(73.48);
    WindowInterface->widgetMainControl2->widgetElevation->setNullDevice(-2.57);


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
  WindowInterface->widgetControlLaserIllum->linkToDevice(ControlLaserIllum);

  WindowInterface->outputVideo1->linkToDevice(ControlAiming1);
  WindowInterface->outputVideo2->linkToDevice(ControlAiming2);

  WindowInterface->widgetMainControl1->linkToDeviceRotary(ControlPlatform->ControlRotaryPos);
  WindowInterface->widgetMainControl1->linkToDevice(ControlLid,0);
  WindowInterface->widgetMainControl1->linkToDevice(ControlLaserIllum,1);
  WindowInterface->widgetMainControl1->linkToDevice(ControlLaserPower,2);
  WindowInterface->widgetMainControl1->linkToDevice(ControlAiming1,3);
  WindowInterface->widgetMainControl1->linkToDevice(ControlAiming2,4);

  WindowInterface->widgetMainControl2->linkToDeviceRotary(ControlPlatform->ControlRotaryPos);
  WindowInterface->widgetMainControl2->linkToDevice(ControlLid,0);
  WindowInterface->widgetMainControl2->linkToDevice(ControlLaserIllum,1);
  WindowInterface->widgetMainControl2->linkToDevice(ControlLaserPower,2);
  WindowInterface->widgetMainControl2->linkToDevice(ControlAiming1,3);
  WindowInterface->widgetMainControl2->linkToDevice(ControlAiming2,4);

  //==================================================================================================================

  //==================================================================================================================
  //CAMERAS
  QStringList links; //links = LoadCameraLinks();
  links.resize(10);
  links[0] = "rtspsrc location=rtsp://192.168.1.59:8554/test latency=10 drop-on-latency=true is-live=true buffer-mode=auto ! queue ! rtph264depay ! h264parse ! openh264dec ! videoconvert ! appsink drop=1 sync=0 max-buffers=1 async=1";
  links[1] = "rtspsrc location=rtsp://192.168.1.108:554/stream3 latency=10 drop-on-latency=true is-live=true buffer-mode=auto! queue ! rtpjpegdepay ! jpegparse ! jpegdec ! videoconvert ! appsink name=sink_node drop=1 sync=0 max-buffers=1 async=1";
  links[2] = "udpsrc port=5000 ! application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96 ! rtph264depay ! h264parse ! openh264dec ! videoconvert n-threads=3 ! video/x-raw,format=RGB ! appsink name=sink_node drop=1 async=false sync=true max-buffers=1";

  //links[0] = "rtsp://192.168.1.31:554/user=admin_password=_channel=1_stream=0.sdp";
  //links[0] = "rtspsrc location=rtsp://admin:123456@192.168.1.247/live/video is-live=true latency=1 buffer-mode=auto ! rtph264depay ! h264parse ! openh264dec ! videoconvert ! video/x-raw,format=RGB ! appsink name=sink_node drop=1 sync=0 max-buffers=1 async=1";
  //links[0] = "rtspsrc location=rtsp://192.168.1.31:554/user=admin_password=_channel=1_stream=0.sdp drop-on-latency=true is-live=1 latency=10 buffer-mode=auto ! queue ! rtph264depay ! h264parse ! openh264dec ! videoconvert n-threads=1 primaries-mode=fast ! video/x-raw,format=RGB ! appsink name=sink_node drop=1 sync=0 async=1 max-buffers=1";
  //links[0] = "rtspsrc location=rtsp://192.168.1.59:8554/test latency=10 drop-on-latency=true is-live=true buffer-mode=auto ! queue ! rtph264depay ! h264parse ! openh264dec ! videoconvert ! appsink name=sink_node drop=1 sync=0 max-buffers=1 async=1";
  //links[2] = "rtspsrc location=rtsp://192.168.1.31:554/user=admin_password=_channel=1_stream=0.sdp latency=1 ! queue ! rtph265depay ! h265parse ! d3d11h265dec ! videoconvert ! appsink";
  //links[2] = "rtsp://192.168.1.38:8554/test";
  //links[0] = "videotestsrc pattern=snow ! video/x-raw,format=BGR,width=640,height=480 ! queue ! videoconvert ! appsink name=sink_node";
  //links[1] = "videotestsrc pattern=ball ! video/x-raw,format=BGR,width=640,height=480 ! queue ! videoconvert ! appsink name=sink_node";
  //links[2] = "videotestsrc ! video/x-raw,format=BGR,width=640,height=480 ! queue ! videoconvert ! appsink name=sink_node";

  //gst_init(&argc, &argv);
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

  //std::chrono::time_point<std::chrono::high_resolution_clock> StartTime = std::chrono::high_resolution_clock::now();
  //std::chrono::time_point<std::chrono::high_resolution_clock> EndTime = std::chrono::high_resolution_clock::now();
  //std::chrono::milliseconds Period = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
                                               //StartTime = std::chrono::high_resolution_clock::now();
                                               //EndTime = std::chrono::high_resolution_clock::now();
                                               //Period = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
//template<> void CommandDispatcherGeneric<TypeRegister<CommandDevice<0>>::ID()>::dispatchCommand(const QByteArray& Command) { qDebug() << "DISPATCH COMMAND: " << TypeRegister<CommandDevice<0>>::TYPE_ID << " DEV 0"; };
//template<> void CommandDispatcherGeneric<TypeRegister<CommandSetPair<0>>::ID()>::dispatchCommand(const QByteArray& Command) { qDebug() << "DISPATCH COMMAND: " << TypeRegister<CommandSetPair<0>>::TYPE_ID << " POS ROTARY"; };