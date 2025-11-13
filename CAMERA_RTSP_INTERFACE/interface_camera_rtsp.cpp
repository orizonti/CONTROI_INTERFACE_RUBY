#include "interface_camera_rtsp.h"
#include "interface_image_source.h"
#include <QDebug>
#include <memory>
#include <opencv2/videoio.hpp>
#include <QRegularExpression>
#include <QProcess>


std::shared_ptr<SourceImageInterface> CameraInterfaceUniversal::getImageSource() { return ImageStore; }

void CameraInterfaceUniversal::slotStartStream() { timerGetFrame.start(20); }
void CameraInterfaceUniversal::slotStopStream()  { timerGetFrame.stop(); }

CameraInterfaceUniversal::CameraInterfaceUniversal(std::string strVideoSource, QString NAME) : TAG_NAME(NAME)
{
  QRegularExpression ip_match("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");

           auto ip = ip_match.match(QString::fromStdString(strVideoSource)).captured(0); 
  if(!checkHost(ip)) return;

  capture.set(cv::CAP_GSTREAMER,3);
  //capture.set(cv::CAP_FFMPEG,3);
  capture.open(strVideoSource);
  
  if (!capture.isOpened()) qDebug()  << "[ ERROR ] CANNOT OPEN VIDEO SOURCE: " << strVideoSource.c_str();
  QObject::connect(&timerGetFrame, SIGNAL(timeout()),this, SLOT(slotGetFrame()));

}

CameraInterfaceUniversal::~CameraInterfaceUniversal() { qDebug() << "[ THERMAL CAMERA INTERFACE DEINIT]" << TAG_NAME; }


void CameraInterfaceUniversal::getImageToDisplay(QImage& ImageDst) 
{ 
  mutexStorage.lock(); ImageDst = ImageStore->ImageToDisplay.copy(); mutexStorage.unlock();
}

cv::Mat& CameraInterfaceUniversal::getImageToProcess()                  { return ImageStore->getImageToProcess(); };
    void CameraInterfaceUniversal::getImageToProcess(cv::Mat& ImageDst) {        ImageStore->getImageToProcess(ImageDst);};

int CameraInterfaceUniversal::getAvailableFrames() { return ImageStore->getAvailableFrames(); }


void CameraImageStorage::initStorage()
{
if(Buffers.empty()) { for (int i = 0; i < 10; i++) Buffers.push_back(cv::Mat()); BufferToWrite = Buffers.begin(); }
                                                                                 BufferToRead = Buffers.begin();
}

void CameraImageStorage::deinitStorage() { }


void CameraInterfaceUniversal::slotGetFrame()
{
        bool isFrameGrabbed = capture.grab();
        if (!isFrameGrabbed) return; 

         auto mat = ImageStore->getBuffer();

             isFrameGrabbed = capture.retrieve(mat);
        if (!isFrameGrabbed) return; 

        mutexStorage.lock(); ImageStore->putNewFrameToStorage(mat); mutexStorage.unlock();
}

void CameraInterfaceUniversal::slotEndWork() 
{ 
  qDebug() << TAG_NAME << "[ STOP CAMERA ]";
  slotStopStream(); deinitCamera(); 
  delete this;
}

void CameraImageStorage::skipFrames()
{
	qDebug() << "[ SKIP FRAMES ]" << BufferToWrite - BufferToRead;
	while(BufferToRead != BufferToWrite)
	{     BufferToRead++; if(BufferToRead == Buffers.end()) BufferToRead = Buffers.begin();}
}

cv::Mat& CameraImageStorage::getImageToProcess()                  
{
    switchToNextFrame(); return ImageToProcess; 
}

void   CameraImageStorage::getImageToProcess(cv::Mat& ImageDst) 
{
    switchToNextFrame(); ImageDst = ImageToProcess.clone(); 
};

bool CameraImageStorage::switchToNextFrame() 
{
   if(BufferToRead == BufferToWrite) return false;
   
   ImageToProcess = *BufferToRead; 
                     BufferToRead++; 
                  if(BufferToRead == Buffers.end()) BufferToRead = Buffers.begin(); 

   ImageToDisplay = QImage(ImageToProcess.data, ImageToProcess.cols, ImageToProcess.rows,QImage::Format_BGR888);
   
   return true;
}

cv::Mat& CameraImageStorage::getBuffer()
{
                                     BufferToWrite++; 
  if(BufferToWrite == Buffers.end()) BufferToWrite = Buffers.begin();
                             return *BufferToWrite;
}

void CameraImageStorage::putNewFrameToStorage(cv::Mat& Frame)
{
                          *BufferToWrite = Frame.clone(); 
   ImageToDisplay = QImage(BufferToWrite->data,Frame.cols,Frame.rows,QImage::Format_BGR888);
                           BufferToWrite++; 
                        if(BufferToWrite == Buffers.end()) BufferToWrite = Buffers.begin();

  SizeImage.first = Frame.cols; SizeImage.second = Frame.rows;

}

void CameraInterfaceUniversal::moveToThread(QThread* thread)
{
         QObject::moveToThread(thread);
    timerGetFrame.moveToThread(thread);
}

void CameraInterfaceUniversal::enumerateCameras() { }
void CameraInterfaceUniversal::initCamera() { }
void CameraInterfaceUniversal::deinitCamera() { }

void CameraInterfaceUniversal::CameraSetHeight(int Height) { }
void CameraInterfaceUniversal::CameraSetWidth(int Width) { }
void CameraInterfaceUniversal::CameraSetOffset(int XOffset, int YOffset) { }
void CameraInterfaceUniversal::CameraSetSize(int Width, int Height) { }
void CameraInterfaceUniversal::CameraSetExposure(float Exposure) { }

bool CameraInterfaceUniversal::checkHost(const QString& ipAddress)
{
    QProcess process;
    process.start("nmap", QStringList() << "-sP" << ipAddress ); // Ping once on Windows
    process.waitForFinished(3000);
    auto output = QString(process.readAllStandardOutput());

    //qDebug() << "===================";
    //for(auto str: output) qDebug() << str;
    //qDebug() << "===================";

    //QRegularExpression re("Host is up");
    QRegularExpression re("MAC Address");
    QRegularExpressionMatch result = re.match(output);
    
    if(result.hasMatch()) qDebug() << ipAddress << "[ UP ]"; else qDebug() << ipAddress << "[ DOWN ]";

           isCameraUp = result.hasMatch();
    return isCameraUp;
}