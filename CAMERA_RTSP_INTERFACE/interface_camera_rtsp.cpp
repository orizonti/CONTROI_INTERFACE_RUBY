#include "interface_camera_rtsp.h"
#include "interface_image_source.h"
#include <QDebug>
#include <memory>
#include <opencv2/videoio.hpp>
#include <QRegularExpression>
#include <QProcess>
#include <QThread>



std::shared_ptr<SourceImageInterface> CameraInterfaceUniversal::getImageSource() { return ImageStore; }

#ifdef GST_CAPTURE
void CameraInterfaceUniversal::slotStartStream() { if(isStreamActive) timerGetFrame.start(10); else timerWaitFrame.start(100); qDebug() << "START STREAM"; }
#endif

#ifdef CV_CAPTURE
void CameraInterfaceUniversal::slotStartStream() { timerGetFrame.start(10);}
#endif

void CameraInterfaceUniversal::slotStopStream()  { timerGetFrame.stop(); timerWaitFrame.stop(); }

CameraInterfaceUniversal::CameraInterfaceUniversal(QString strVideoSource, QString NAME) : TAG_NAME(NAME)
{
  QRegularExpression ip_match("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
  QRegularExpression udp_match("udpsrc");

  auto matched = udp_match.match(strVideoSource);                        isCameraUp = matched.hasMatch();
       matched =  ip_match.match(strVideoSource); if(matched.hasMatch()) isCameraUp = checkHost(matched.captured(0));

  #ifdef CV_CAPTURE
  if(!isCameraUp) return;
       capture.open(strVideoSource.toStdString(), cv::CAP_GSTREAMER);
  if (!capture.isOpened()) qDebug()  << "[ ERROR ] CANNOT OPEN VIDEO SOURCE: " << strVideoSource;
  QObject::connect(&timerGetFrame, SIGNAL(timeout()),this, SLOT(slotGetFrame()));
  #endif
  


  #ifdef GST_CAPTURE
      //qDebug() << "MAKE PIPE: " << strVideoSource;

                                                           GError *err = nullptr;
      GstElement* pipeLineInput = gst_parse_launch   (strVideoSource.toStdString().c_str(), &err);  
          frameInputNode = gst_bin_get_by_name(GST_BIN (pipeLineInput), "sink_node");        
          //qDebug() << "INPUT NODE: " << frameInputNode;
             auto result = gst_element_set_state(pipeLineInput, GST_STATE_PLAYING);          
                           //gst_element_set_state(pipeLineInput, GST_STATE_NULL); gst_object_unref(pipeLineInput);
  QObject::connect(&timerWaitFrame, SIGNAL(timeout()),this, SLOT(slotWaitFrame()));
  QObject::connect(&timerGetFrame , SIGNAL(timeout()),this, SLOT(slotGetFrame() ));
  #endif

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


#ifdef CV_CAPTURE
void CameraInterfaceUniversal::slotGetFrame()
{
        bool isFrameGrabbed = capture.grab();
        if (!isFrameGrabbed) return; 

         auto mat = ImageStore->getBuffer();

             isFrameGrabbed = capture.retrieve(mat);
        if (!isFrameGrabbed) return; 

        mutexStorage.lock(); ImageStore->putNewFrameToStorage(mat); mutexStorage.unlock();
}
#endif

#ifdef GST_CAPTURE
void CameraInterfaceUniversal::slotWaitFrame()
{
  GstStateChangeReturn ret;
  qDebug() << "CAMERA WAIT FRAME"; 
  //QThread::currentThread()->sleep(1);

       frameState.frameSample = gst_app_sink_pull_sample(GST_APP_SINK(frameInputNode)); 
   if (frameState.frameSample == nullptr) return; 

         frameState.frameCaps = gst_sample_get_caps   (frameState.frameSample);                               //myAssert(caps != nullptr);
       frameState.frameParams = gst_caps_get_structure(frameState.frameCaps, 0);

  auto result = gst_structure_get_int(     frameState.frameParams, "width"    , &frameState.frameWidth);              //MY_ASSERT(result);
       result = gst_structure_get_int(     frameState.frameParams, "height"   , &frameState.frameHeight);             //MY_ASSERT(result);
       auto format = gst_structure_get_string(     frameState.frameParams, "format");             //MY_ASSERT(result);
       result = gst_structure_get_fraction(frameState.frameParams, "framerate", &frameState.frameFreqNum, &frameState.frameFreqDenum); //MY_ASSERT(result);

       frameState.frameSize = frameState.frameWidth*frameState.frameHeight*3;

  qDebug() << " [ CAMERA START STREAM ]  [ WIDTH ]" <<  frameState.frameWidth << "[ HEIGHT ]" << frameState.frameHeight << "[ FORMAT ]" << format;

  timerWaitFrame.stop(); timerGetFrame.start();
  isStreamActive = true;
}

void CameraInterfaceUniversal::slotGetFrame()
{
  GstMapInfo mapBufferIn;

  if (gst_app_sink_is_eos(GST_APP_SINK(frameInputNode))) { std::cout << "INPUT PIPE END STREAM !" << std::endl; return; }

      frameState.frameSample = gst_app_sink_pull_sample(GST_APP_SINK(frameInputNode));
  if (frameState.frameSample == nullptr) return;

      //qDebug() << "SINK: " << frameInputNode << "GET FRAME";
      GstBuffer *bufferIn = gst_sample_get_buffer(frameState.frameSample);
                            gst_buffer_map(bufferIn, &mapBufferIn, GST_MAP_READ); 

      ImageStore->putNewFrameToStorage((void*)mapBufferIn.data, frameState.frameHeight, frameState.frameWidth); 
                            gst_buffer_unmap(bufferIn, &mapBufferIn);


}
#endif


void CameraInterfaceUniversal::slotEndWork() 
{ 
  qDebug() << TAG_NAME << "[ STOP CAMERA ]";
  slotStopStream(); 
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


void CameraImageStorage::putNewFrameToStorage(void* Frame, int width, int height)
{

                          *BufferToWrite = cv::Mat(width, height, CV_8UC3, Frame).clone();
   ImageToDisplay = QImage(BufferToWrite->data,BufferToWrite->cols, BufferToWrite->rows, QImage::Format_RGB888);
                           BufferToWrite++; 
                        if(BufferToWrite == Buffers.end()) BufferToWrite = Buffers.begin();
  SizeImage.first = BufferToWrite->cols; SizeImage.second = BufferToWrite->rows;
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

void CameraInterfaceUniversal::CameraSetHeight(int Height) { }
void CameraInterfaceUniversal::CameraSetWidth(int Width) { }
void CameraInterfaceUniversal::CameraSetOffset(int XOffset, int YOffset) { }
void CameraInterfaceUniversal::CameraSetSize(int Width, int Height) { }
void CameraInterfaceUniversal::CameraSetExposure(float Exposure) { }

bool CameraInterfaceUniversal::checkHost(const QString& ipAddress)
{
    QProcess process;
    process.start("nmap", QStringList() << "-sP" << ipAddress ); // Ping once on Windows
    process.waitForFinished(7000);
    auto output = QString(process.readAllStandardOutput());
    qDebug() << "[ CHECK HOST ] " << ipAddress;

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