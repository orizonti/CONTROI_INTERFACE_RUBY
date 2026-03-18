
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


class TrackLinearRegression: public PassCoordClass<float>
{
  public:
    TrackLinearRegression(int Size) 
    { 
      track.resize(Size);
      pos_track = track.begin();
      SizeWindow = Size;
    };

    int SizeWindow = 10;
    std::vector<std::pair<float,float>> track;
    std::vector<std::pair<float,float>>::iterator pos_track;

    float slope  =  1;
    float offset =  1;

    const QPair<float,float>& getOutput() override { return OutputCoord;};
    void setInput(const QPair<float,float>& Coord) override  
    {
      *pos_track = Coord; pos_track++; if(pos_track == track.end()) { pos_track = track.begin(); 
                                                                      getApproximation(); } 
    };

    float sumX  = 0.0f; 
    float sumY  = 0.0f; 
    float sumXY = 0.0f; 
    float sumXX = 0.0f;

    void getApproximation() 
    {
        for (const auto& point : track) 
        {
            sumX  += point.first;
            sumY  += point.second;
            sumXY += point.first * point.second;
            sumXX += point.first * point.first;
        }
      qDebug() << "GET LINEAR APPROXIMATION" << sumX << sumY << sumXY << sumXX << slope << offset;
        slope = (SizeWindow * sumXY - sumX * sumY) / (SizeWindow * sumXX - sumX * sumX);
        offset = (sumY - slope * sumX) / SizeWindow;
    }
};


class CoordTrackApproximation: public PassCoordClass<float>
{
  public:
    CoordTrackApproximation(int Size = 100) 
    { 
      track.resize(Size); pos_track = track.begin();
      SizeWindow = Size;

      A_MAT = Eigen::MatrixXd(static_cast<Eigen::Index>(SizeWindow), 3);
      Y_VEC = Eigen::VectorXd(static_cast<Eigen::Index>(SizeWindow));
    };
    void operator=(const CoordTrackApproximation& copy)
    {
        track.resize(copy.SizeWindow);
        pos_track = track.begin();
        SizeWindow = copy.SizeWindow;
        A_MAT = Eigen::MatrixXd(static_cast<Eigen::Index>(SizeWindow), 3);
        Y_VEC = Eigen::VectorXd(static_cast<Eigen::Index>(SizeWindow));
    };


    int SizeWindow = 10;
    std::vector<std::pair<float,float>> track;
    std::vector<std::pair<float,float>>::iterator pos_track;

    double RMSE = std::numeric_limits<double>::quiet_NaN();
    bool isResultValid = false;

    Eigen::MatrixXd A_MAT;
    Eigen::VectorXd Y_VEC;
    Eigen::Vector3d PARAMS_FUNC;

void getApproximation()  
{
    qDebug() << "GET APPROXIMATION SIZE: " << track.size();
    for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(SizeWindow); ++i) 
    {
        const double x = static_cast<double> (track[static_cast<std::size_t>(i)].first);

        A_MAT(i, 0) = x * x;
        A_MAT(i, 1) = x;
        A_MAT(i, 2) = 1.0;
        Y_VEC(i) = static_cast<double>(track[static_cast<std::size_t>(i)].second);
    }

         PARAMS_FUNC = A_MAT.colPivHouseholderQr().solve(Y_VEC);
    if (!PARAMS_FUNC.allFinite()) { isResultValid = false; return; } // Check for numerical issues (optional but good practice)
                                    isResultValid = true;

    //Eigen::VectorXd residual = A_MAT * PARAMS_FUNC - Y_VEC;
    //const double mse = residual.squaredNorm() / static_cast<double>(SizeWindow);
    //RMSE = std::sqrt(mse);

}


std::tuple<float,float,float> getApproximation(std::vector<std::pair<float,float>> track)  
{
    qDebug() << "GET APPROXIMATION";

    A_MAT = Eigen::MatrixXd(static_cast<Eigen::Index>(track.size()), 3);
    Y_VEC = Eigen::VectorXd(static_cast<Eigen::Index>(track.size()));

    for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(track.size()); ++i) 
    {
        const double x = static_cast<double> (track[static_cast<std::size_t>(i)].first);
        A_MAT(i, 0) = x * x;
        A_MAT(i, 1) = x;
        A_MAT(i, 2) = 1.0;
        Y_VEC(i) = static_cast<double>(track[static_cast<std::size_t>(i)].second);
    }

         PARAMS_FUNC = A_MAT.colPivHouseholderQr().solve(Y_VEC);
    if (!PARAMS_FUNC.allFinite()) { isResultValid = false; return std::make_tuple(0,0,0); } // Check for numerical issues (optional but good practice)
                                    isResultValid = true;

    return std::make_tuple(PARAMS_FUNC(0),PARAMS_FUNC(1),PARAMS_FUNC(2));
}

std::tuple<float,float,float> getResult()  { return std::make_tuple(PARAMS_FUNC(0),PARAMS_FUNC(1),PARAMS_FUNC(2)); }

	const QPair<float,float>& getOutput() override { return OutputCoord;};
	void setInput(const QPair<float,float>& Coord) override  
  {
     *pos_track = Coord; 
     pos_track++; if(pos_track == track.end()) { pos_track = track.begin(); getApproximation(); } 
  };

};

template<typename T_NODE>
class TrackRegresstionTest
{
  public:
  int NumberPoints = 300;
  float Range = 15;
  float Step = Range/NumberPoints;
  CoordTrackApproximation trackApprox{NumberPoints};

  ValueRandomization<float> Randomize{14};
  std::pair<float,float> Coord;
  std::pair<float,float> CoordNoize;

  float C1 = 0;
  float C2 = 0;
  float C3 = 0;
  float C4 = 0;

  QList<QPointF> Track;
  QList<QPointF> TrackNoize;
  QList<QPointF> TrackApprox;

  TrackQuadraticRegresstionTest(float C1, float C2, float C3, float C4 = 0, int Size)
  {
    NumberPoints = Size;
    Step = Range/Size;
    trackApprox = CoordTrackApproximation(Size);

    std::pair<float,float> Coord;
    std::pair<float,float> CoordNoize;

    for(float n = 0; n < NumberPoints; n++)
    {
      Coord.first  = n*Step;
      CoordNoize.first = Coord.first;
      Coord.second = std::pow(Coord.first,2)*C1 + Coord.first*C2 + C3;
      Coord.second >> Randomize(20) >> CoordNoize.second;

            Track.append(QPointF(Coord.first,Coord.second));
       TrackNoize.append(QPointF(CoordNoize.first,CoordNoize.second));
      TrackApprox.append(QPointF(Coord.first,Coord.second));

      CoordNoize >> trackApprox;
    }

      auto [A_RES,B_RES,C_RES] = trackApprox.getResult(); 

      for(auto& point: TrackApprox) point.setY(std::pow(point.x(),2)*A_APPROX + point.x()*B_APPROX + C_APPROX);

      //qDebug() << "[ INPUT PARAM ]" << A_PARAM << B_PARAM << C_PARAM 
      //         << "[ APPROX ]" << A_APPROX << B_APPROX << C_APPROX
      //         << "[ DIFF   ]" << abs(A_PARAM - A_APPROX) << abs(B_PARAM - B_APPROX)  << abs(C_PARAM - C_APPROX) 
      //         << "[ REUSLT ]" << isResultValid();

  }

//  bool isResultValid() { return abs(A_PARAM - A_APPROX) < A_PARAM/10 && 
//                                abs(B_PARAM - B_APPROX) < B_PARAM/10 && 
//                                abs(C_PARAM - C_APPROX) < C_PARAM/10; }
};




int main(int argc, char* argv[])
{
  QApplication app(argc,argv);


//  WidgetDeviceControl wid{"Device", Qt::Vertical};
//  wid.enableScheme(0,1,1,0,0,0); wid.setScheme(5,0,0);
//  wid.show();

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

  WindowInterface->widgetMainControl2->linkToDeviceRotary(ControlScanator->ControlRotaryPos);
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

  TrackLinearRegression  trackApproxLinear(200);
  std::pair<float,float> Coord;
  std::pair<float,float> CoordNoize;
  ValueRandomization<float> Randomize{14};
  for(int n = 0; n < 200; n++)
  {
     Coord.first  = n;
     Coord.second = (float)n*0.35 + 22;
     CoordNoize.first = Coord.first;
     Coord.second >> Randomize >> CoordNoize.second;
     CoordNoize >> trackApproxLinear;
  }

  qDebug() << "GET LINEAR PARAM: " << trackApproxLinear.slope << trackApproxLinear.offset;

  MeasurePeriodNode periodMeasure;

                                               
  periodMeasure++;
  TrackQuadraticRegresstionTest Test1(2,22,8,200);
  periodMeasure++; qDebug() << "PROC PERIOD: " << periodMeasure.getMicroseconds();

  TrackQuadraticRegresstionTest Test2(5,22,8,200);
  TrackQuadraticRegresstionTest Test3(2,22,8,200);

  WidgetLineGraph graphWidget;
                  graphWidget.GraphPointsStorage->setPoints(Test1.TrackNoize);
                  graphWidget.GraphPointsStorage2->setPoints(Test1.TrackApprox);
                  graphWidget.show();

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
//std::pair<float, float> linearRegression(const std::vector<std::pair<float, float>>& data) 
//{
//    float sumX = 0.0f, sumY = 0.0f, sumXY = 0.0f, sumXX = 0.0f;
//    for (const auto& point : data) 
//    {
//        sumX += point.first;
//        sumY += point.second;
//        sumXY += point.first * point.second;
//        sumXX += point.first * point.first;
//    }
//
//    float n = static_cast<float>(data.size());
//    float slope = (n * sumXY - sumX * sumY) / (n * sumXX - sumX * sumX);
//    float intercept = (sumY - slope * sumX) / n;
//
//    return std::make_pair(slope, intercept);
//}