#pragma once

#include <QObject>
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QString>
#include <QTimer>
#include <QDebug>

template<int NUM> class RegisterNode
{
    public:
    RegisterNode(std::string TAG) { CounterSources++; Number = CounterSources; SetTAG(TAG); };
    RegisterNode() { CounterSources++; Number = CounterSources; };
    int Number = 0;
    static int CounterSources;
    static std::string TAG;
    static void SetTAG(std::string TAG_NAME) { TAG = TAG_NAME;};
};
template<int NUM> int RegisterNode<NUM>::CounterSources = 0;
template<int NUM> std::string RegisterNode<NUM>::TAG = "[ NODE_REGISTER ]";

class SourceImageInterface
{
    public:
    virtual cv::Mat& getImageToProcess() = 0;
    virtual     void getImageToProcess(cv::Mat& ImageDst) = 0;

    virtual std::pair<int,int> getSizeImage()       = 0;
    virtual                int getAvailableFrames() = 0;
    virtual               bool isFrameAvailable()   { return getAvailableFrames() > 0;};

    virtual bool switchToNextFrame() { return false; };
    virtual void skipFrames() {};


};

class SinkDisplayNode;
class SourceImageDisplayInterface
{
    public:
    virtual     void getImageToDisplay(QImage& ImageDst)  = 0;

    virtual const std::vector<QPair<int,int>>& getPoints() = 0;  
    virtual const          std::vector<QRect>& getRects()  = 0;  
    virtual const                     QString& getInfo()   = 0;  

    virtual std::pair<int,int> getSizeImage()  = 0;

    void linkToSink(SinkDisplayNode* Node) { LinkNode = Node;}
    void passToLink();

    private:
    SinkDisplayNode* LinkNode = nullptr;
};

class SinkDisplayNode : public QObject
{
    Q_OBJECT
	public:
    SinkDisplayNode() { connect(&timerSetImage, &QTimer::timeout, this, &SinkDisplayNode::slotGetImage); }

            void setFrequency(int Freq) { Frequency = Freq; timerSetImage.setInterval(1000/Freq); };
	virtual void setImageFrom(SourceImageDisplayInterface* Src) = 0;
    void linkToSource(SourceImageDisplayInterface* Src) { SrcNode = Src; };
    
    SourceImageDisplayInterface* GetSource() { return SrcNode;};
    private:
    SourceImageDisplayInterface* SrcNode = nullptr;

       int Frequency = 30;
    QTimer timerSetImage;
    
    public slots:
    void slotActivate(bool OnOff = true) 
    { 
        if(SrcNode == nullptr) return;
        if(OnOff) timerSetImage.start(1000/Frequency); else timerSetImage.stop(); 
        qDebug() << "ACTIVATE RTSP DISPLAY" << OnOff;
    }; 

    private slots:
	void slotGetImage() { if(SrcNode != nullptr) setImageFrom(SrcNode); };
};