#ifndef WIDGET_CAMERA_DISPLAY_H
#define WIDGET_CAMERA_DISPLAY_H

#include <QTimer>
#include <functional>
#include <QBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <qboxlayout.h>
#include <qnamespace.h>
#include <qsharedpointer.h>

#include "interface_camera_rtsp.h"
#include "interface_image_source.h"
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <qsizepolicy.h>
#include <qwidget.h>
#include "widget_adjustable.h"

class CommonStyleSheet
{
  public:
  static QString style;
  static QString style2;
};


class WidgetProcessDisplay : public WidgetAdjustable
{
  Q_OBJECT
  public:
  WidgetProcessDisplay(QWidget* parent = 0): WidgetAdjustable(parent)
  {
  auto layout = new QVBoxLayout(this);
       layout->addWidget(&LabelActiveImage);
       layout->setContentsMargins(1,1,1,1);
       layout->setSpacing(1);

  this->setLayout(layout);
  this->setMinimumSize(610,510);
  this->LabelActiveImage.setMargin(1);

  ImageDisplay = QImage(400,400,QImage::Format_BGR888);  ImageDisplay.fill(Qt::darkGray);
  LabelActiveImage.setPixmap(QPixmap::fromImage(ImageDisplay)); 
  LabelActiveImage.setScaledContents(true);

  connect(&timerDisplayImage, SIGNAL(timeout()), this, SLOT(SlotDisplayImage()));

  this->setStyleSheet ( CommonStyleSheet::style);
  ImageDisplay = QImage(2560,1440,QImage::Format_RGB888);
  }

  QLabel LabelActiveImage; 
  QImage ImageDisplay;

  void LinkToSource(std::shared_ptr<SourceImageInterface> Source);
  std::shared_ptr<SourceImageInterface> ImageSource = nullptr;

  QTimer timerDisplayImage;

public slots:
void SlotDisplayImage()
{
//   if( ImageSource != nullptr) ImageSource->getImageToDisplay(ImageDisplay);
//                     LabelActiveImage.setPixmap(QPixmap::fromImage(ImageDisplay)); 
}

};

//class WidgetCameraControl : public QWidget
//{
//  Q_OBJECT
//  public:
//  WidgetCameraControl(QWidget* parent = nullptr) : QWidget(parent) 
//  {
//             auto layout = new QHBoxLayout(this);
//                  layout->setSpacing(2);
//                  layout->setContentsMargins(2,2,2,2);
//  this->setLayout(layout);
//  this->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
//
//  lineExposure.setText("500");
//  butStartStopCamera.setCheckable(true);
//
//  auto box = new QGroupBox(this); 
//
//  auto layout3 = new QVBoxLayout(this); 
//  auto layout4 = new QHBoxLayout(this); layout3->addLayout(layout4);
//       layout4->addWidget(&butZoom1); 
//       layout4->addWidget(&butZoom2); 
//       layout4->addWidget(&butZoom3); 
//       layout4->addWidget(&butZoom4); 
//       layout4->addWidget(&butZoom5); 
//       butZoom1.setFixedSize(60,60); butZoom1.setCheckable(true); butZoom1.setAutoExclusive(true);
//       butZoom2.setFixedSize(60,60); butZoom2.setCheckable(true); butZoom2.setAutoExclusive(true);
//       butZoom3.setFixedSize(60,60); butZoom3.setCheckable(true); butZoom3.setAutoExclusive(true);
//       butZoom4.setFixedSize(60,60); butZoom4.setCheckable(true); butZoom4.setAutoExclusive(true);
//       butZoom5.setFixedSize(60,60); butZoom5.setCheckable(true); butZoom5.setAutoExclusive(true);
//       butZoom1.setChecked(true);
//
//       layout4 = new QHBoxLayout(this); layout3->addLayout(layout4);
//       layout4->addWidget(&butSetExposure); layout4->addWidget(&lineExposure);
//
//  box->setLayout(layout3);
//
//                  layout->addWidget(box);
//                  layout->addWidget(&butStartStopCamera);
//  lineExposure.setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
//  box->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
//
//  butStartStopCamera.setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
//
//  this->setStyleSheet ( CommonStyleSheet::style);
//  }
//
//  void LinkToCamera(CameraInterfaceUniversal* Camera);
//
//  QPushButton butStartStopCamera{"ПУСК КАМЕРЫ"};
//  QPushButton butSetCameraImageSize{"УСТАНОВИТЬ"};
//  QPushButton butSetExposure{"УСТАНОВИТЬ"};
//
//  QPushButton butZoom1{"X1"};
//  QPushButton butZoom2{"X2"};
//  QPushButton butZoom3{"X3"};
//  QPushButton butZoom4{"X4"};
//  QPushButton butZoom5{"X5"};
//
//  QLineEdit lineExposure;
//
//  CameraInterfaceUniversal* CameraDevice = nullptr;
//};
//
//class WidgetCombinedInterface : public QWidget
//{
//  public:
//  WidgetCombinedInterface(QWidget* parent = 0): QWidget(parent)
//  {
//             auto layout = new QVBoxLayout(this);
//  this->setLayout(layout);
//
//             auto layout2 = new QHBoxLayout(this);
//                  layout2->addWidget(&Display1);
//                  layout2->addWidget(&Display2);
//
//                  layout->addLayout(layout2);
//                  layout->addWidget(&CameraControl);
//  this->setStyleSheet(CommonStyleSheet::style2);
//
//  this->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
//  }
//  WidgetProcessDisplay Display1;
//  WidgetProcessDisplay Display2;
//  WidgetCameraControl CameraControl;
//};



#endif //WIDGET_CAMERA_DISPLAY_H
