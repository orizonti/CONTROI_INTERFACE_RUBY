#ifndef WIDGET_CAMERA_INTERFACE_H
#define WIDGET_CAMERA_INTERFACE_H
#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include "interface_image_source.h"
#include <QDebug>
#include <QTimer>
#include <QMutex>


class CameraInterface
{
  public:
  virtual void CameraSetSize(int Width, int Height) = 0;
  virtual void CameraSetOffset(int XOffset, int YOffset) = 0;
  virtual void CameraSetHeight(int Height) = 0;
  virtual void CameraSetWidth(int Width) = 0;
  virtual void CameraSetExposure(float Exposure) = 0;
};


class CameraImageStorage;
class CameraInterfaceUniversal :public QObject, public SourceImageInterface, public SourceImageDisplayInterface, public CameraInterface
{
  Q_OBJECT
  public:

  public:
  explicit CameraInterfaceUniversal(std::string strVideoSource, QString NAME = "[ CAMERA ]");
           ~CameraInterfaceUniversal();
  public:
  QString    TAG_NAME{"[ CAMERA ]"};
  QString CAMERA_INFO{"[ CAMERA NO DATA ]"};

  std::shared_ptr<SourceImageInterface> getImageSource();

       int getAvailableFrames() override;
      void getImageToDisplay(QImage& ImageDst) override ;

  cv::Mat& getImageToProcess()                 override ;
      void getImageToProcess(cv::Mat& ImageDst)override ;

    const std::vector<QPair<int,int>>& getPoints() override { return CameraPoints;};  
              const std::vector<QRect>& getRects() override { return CameraRects;};  
                          const QString& getInfo() override { return CAMERA_INFO;};  
                 std::pair<int,int> getSizeImage() override { return SizeImage;};

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

  //=============================================
  std::pair<int,int> ImagePos {20 ,20 }; 
  std::pair<int,int> SizeImage{720,540}; 

  std::vector<QPair<int,int>> CameraPoints{2};
  std::vector<QRect>          CameraRects {2};

  cv::VideoCapture capture;
  QTimer timerGetFrame{this};

  QMutex mutexStorage;
  std::shared_ptr<CameraImageStorage> ImageStore = std::make_shared<CameraImageStorage>(this);
  //=============================================

  public slots:
  void slotGetFrame();
  void slotStartStream();
  void slotStopStream();
  void slotEndWork();

  private:

  void enumerateCameras();
  void initCamera();
  void deinitCamera();
};

  class CameraImageStorage: public SourceImageInterface
  {
    public:
    explicit CameraImageStorage(CameraInterfaceUniversal* CameraDevice) { initStorage(); };
            ~CameraImageStorage() {deinitStorage(); };
    void initStorage();
    void deinitStorage();
    cv::Mat& getBuffer();
    cv::Mat InputImage;

              std::vector<cv::Mat> Buffers;
    std::vector<cv::Mat>::iterator BufferToWrite;
    std::vector<cv::Mat>::iterator BufferToRead;

    cv::Mat ImageToProcess;
     QImage ImageToDisplay;

    std::pair<int,int> SizeImage; 

    void putNewFrameToStorage(cv::Mat& Frame);

    int getAvailableFrames() override { return BufferToWrite - BufferToRead; }
     bool isFrameAvailable() override { return BufferToWrite != BufferToRead;};
    bool switchToNextFrame() override;
    void skipFrames()        override;


    std::pair<int,int> getSizeImage() { return std::pair<int,int>(ImageToProcess.cols, ImageToProcess.rows); };

  cv::Mat& getImageToProcess() override ;
      void getImageToProcess(cv::Mat& ImageDst) override ;
  };

#endif "WIDGET_CAMERA_INTERFACE_H"


