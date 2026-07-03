#ifndef WIDGET_CAMERA_INTERFACE_H
#define WIDGET_CAMERA_INTERFACE_H
#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include "interface_image_source.h"
#include <QDebug>
#include <QTimer>
#include <QMutex>

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include "device_generic_interface.h"
#include "engine_udp_interface.h"
#include "device_zoom_rotation.h"
#include "module_period_measure.h"


#define CV_CAPTURE 1
//#define GST_CAPTURE 1

struct frameStateStruct
{
  GstSample*    frameSample = nullptr;
  GstCaps*      frameCaps   = nullptr;
  GstStructure* frameParams = nullptr;
            int frameWidth     = 0;
            int frameHeight    = 0;
            int frameFreqNum   = 0; 
            int frameFreqDenum = 0;
            int frameSize      = 0;

};



class CameraImageStorage;
class CameraInterfaceUniversal :public QObject, public SourceImageInterface, 
                                                public SourceImageDisplayInterface 
{
  Q_OBJECT
  public:
  explicit CameraInterfaceUniversal(QString strVideoSource, QString NAME = "[ CAMERA ]");
           ~CameraInterfaceUniversal();
  public:
  QString    TAG_NAME{"[ CAMERA ]"};
  QString CAMERA_INFO{"[ CAMERA NO DATA ]"};
  QString AddressCamera;

  std::shared_ptr<DeviceZoomRotation> ControlZoom = nullptr;

  MeasurePeriodNode MeasurePeriod;
  void connectZoomControl(QString user, QString pass, QString ip, QString port);
  //std::shared_ptr<DeviceCameraRemoteInterface<UDPConnectionEngine, int,int>> ControlCameraRemote = nullptr; 

  cv::VideoCapture capture;
            QTimer timerGetFrame{this};
            QTimer timerWaitFrame{this};
  frameStateStruct frameState; 
  GstElement *frameInputNode = nullptr;
  cv::RotateFlags RotationDirection = cv::ROTATE_90_COUNTERCLOCKWISE;
  void setRotate(cv::RotateFlags Rotation) { RotationDirection = Rotation; isRotateNeeded = true;};
  void setFlip(bool OnOff) { isFlipNeeded = OnOff; };

  std::shared_ptr<SourceImageInterface> getImageSource();

       int getAvailableFrames() override;
      void getImageToDisplay(QImage& ImageDst) override ;

  cv::Mat& getImageToProcess()                 override ;
      void getImageToProcess(cv::Mat& ImageDst)override ;


    const std::vector<QPair<int,int>>& getPoints() override { return CameraPoints;};  
              const std::vector<QRect>& getRects() override { return CameraRects;};  
                          const QString& getInfo() override { return CAMERA_INFO;};  
                 std::pair<int,int> getSizeImage() override { return SIZE_ROI;};

    void moveToThread(QThread* thread);
  //=========================================

  void CameraSetSize(int Width, int Height);
  void CameraSetOffset(int XOffset, int YOffset);
  void CameraSetHeight(int Height);
  void CameraSetWidth(int Width);
  void CameraSetExposure(float Exposure);

  bool checkHost(const QString& ipAddress);
  bool isCameraAccessable() { return isCameraUp;};
  bool isCameraUp = false;
  bool isStreamActive = false;

  //=============================================
  std::pair<int,int> SIZE_CAMERA{720,1280}; 
  std::pair<int,int> SIZE_ROI {640 ,480}; 
  std::pair<int,int> OFFSET_ROI = (SIZE_CAMERA-SIZE_ROI)*0.5; 
  cv::Rect rectCrop{OFFSET_ROI.first,OFFSET_ROI.second,SIZE_ROI.first,SIZE_ROI.second};
  void setCameraSizes(std::pair<int,int> SizeCamera, std::pair<int,int> ROI);
  void printCameraSizes();

  std::vector<QPair<int,int>> CameraPoints{2};
  std::vector<QRect>          CameraRects {2};

  cv::Mat inputImage          {SIZE_CAMERA.first,SIZE_CAMERA.second,CV_8UC3};
  cv::Mat inputImageProcessed {SIZE_CAMERA.first,SIZE_CAMERA.second,CV_8UC3};
  cv::Mat inputImageRotated   {SIZE_ROI.first,SIZE_ROI.second,CV_8UC3};
  cv::Mat inputImageResized   {SIZE_ROI.first,SIZE_ROI.second,CV_8UC3};
  cv::Mat inputImageGrayscale {SIZE_ROI.first,SIZE_ROI.second,CV_8UC1};
  cv::Mat processImage;
     bool isCropNeeded = false;
     bool isRotateNeeded = false;
     bool isFlipNeeded = false;
     bool isBrightNeeded = false;
     bool isColorConvertNeeded = false;


  QMutex mutexStorage;
  std::shared_ptr<CameraImageStorage> ImageStore = std::make_shared<CameraImageStorage>(this);
  //=============================================

  bool IsROIValid(cv::Rect& ROI);
  void reset() { emit signalReset(); }

  public slots:
  void slotGetFrame();
  void slotStartStream();
  void slotStopStream();
  void slotEndWork();
  void slotCheckHost();
  void slotReset();

  #ifdef GST_CAPTURE
  void slotWaitFrame();
  #endif
  signals:
  void signalReset();
};



class CameraImageStorage: public SourceImageInterface
{
  public:
  explicit CameraImageStorage(CameraInterfaceUniversal* CameraDevice) { initStorage(); };
          ~CameraImageStorage() { deinitStorage(); };
  void initStorage();
  void deinitStorage();
  cv::Mat& getBuffer();
  cv::Mat InputImage;
  std::mutex lockBuffer;

            std::vector<cv::Mat> Buffers;
  std::vector<cv::Mat>::iterator BufferToWrite;
  std::vector<cv::Mat>::iterator BufferToRead;

   cv::Mat ImageToProcess;
    QImage ImageToDisplay;

  std::pair<int,int> SizeImage; 

  void putNewFrameToStorage(cv::Mat& Frame);
  void putNewFrameToStorage(void* Frame, int width, int height);

  int getAvailableFrames() override { return BufferToWrite - BufferToRead; }
   bool isFrameAvailable() override { return BufferToWrite != BufferToRead;};
  bool switchToNextFrame() override;
  void skipFrames()        override;

  std::pair<int,int> getSizeImage() { return std::pair<int,int>(ImageToProcess.cols, ImageToProcess.rows); };

cv::Mat& getImageToProcess() override ;
    void getImageToProcess(cv::Mat& ImageDst) override ;
};

#endif "WIDGET_CAMERA_INTERFACE_H"


