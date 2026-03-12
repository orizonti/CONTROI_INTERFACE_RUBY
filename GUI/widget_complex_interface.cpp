#include <QSpinBox>
#include "./ui_widget_complex_interface.h"
#include "widget_complex_interface.h"
#include <QTimer>
#include <QKeyEvent>
#include <QtCore\qmath.h>
#include <QGraphicsPixmapItem>
#include "label_active_image.h"

#include "message_command_id.h"
#include <QSizePolicy>

WidgetComplexInterface::WidgetComplexInterface(QWidget *parent)
    : WidgetAdjustable(parent)
    , ui(new Ui::WidgetComplexInterface)
{
    ui->setupUi(this);

    qDebug() << "CREATE COMPLEX INTERFACE";
    widgetControlBlock = new WidgetControlBlock;
    widgetControlRotary1 = new WidgetRotaryPlatformControl(0);
    widgetControlRotary2 = new WidgetRotaryPlatformControl(1);
    widgetControlRotary1->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    widgetControlRotary2->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    //auto button = new QPushButton(tr("Reset all shortcuts to default"), this);
    //button->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed, QSizePolicy::ToolButton));


    widgetControlRanger    = new WidgetDeviceControl("Дальномер");
    widgetControlFocusator = new WidgetDeviceControl("Фокусатор");

//void WidgetDeviceControl::enableScheme(bool enableState, 
//                                       bool enableParam, 
//                                       bool enableLevels, 
//                                       bool enableOnOff, 
//                                       bool enableArrows)

//void WidgetDeviceControl::setScheme(int schemeParam, int numberLevels, int numberDevice, int schemeArrows) {

    widgetControlRanger->enableScheme(1,0,0,1,0);    widgetControlRanger->setScheme(0,0,1); 
    widgetControlFocusator->enableScheme(1,0,0,1,0); widgetControlFocusator->setScheme(0,0,1);

    widgetControlLaserPower = new WidgetDeviceControl("Лазер    "); 
    widgetControlLaserIllum = new WidgetDeviceControl("Подсвет  "); 
    widgetControlLaserPower->enableScheme(1,0,1,0,0); widgetControlLaserPower->setScheme(0,2,2); 
    widgetControlLaserIllum->enableScheme(1,0,1,0,0); widgetControlLaserIllum->setScheme(0,2,1);

       widgetControlCamera1 = new WidgetDeviceControl("КамераТК "); 
       widgetControlCamera2 = new WidgetDeviceControl("КамераТК "); 
       widgetControlCamera3 = new WidgetDeviceControl("КамераГК "); 
       widgetControlCamera4 = new WidgetDeviceControl("Тепловиз "); 
    widgetControlCamera1->enableScheme(1,1,0,0,0); widgetControlCamera1->setScheme(1,5,0); widgetControlCamera1->setLevelsName(QStringList({"X1", "X2", "X3", "X4", "X5"}));
    widgetControlCamera2->enableScheme(1,1,0,0,0); widgetControlCamera2->setScheme(1,5,0); widgetControlCamera2->setLevelsName(QStringList({"X1", "X2", "X3", "X4", "X5"})); 
    widgetControlCamera3->enableScheme(1,1,0,0,0); widgetControlCamera3->setScheme(1,5,0); widgetControlCamera3->setLevelsName(QStringList({"X1", "X2", "X3", "X4", "X5"}));
    widgetControlCamera4->enableScheme(1,1,0,0,0); widgetControlCamera4->setScheme(1,5,0); widgetControlCamera4->setLevelsName(QStringList({"X1", "X2", "X3", "X4", "X5"}));


      widgetControlScanator = new WidgetDeviceControl("Сканатор "); 
      widgetControlPlatform = new WidgetDeviceControl("Платформа"); 
    widgetControlScanator->enableScheme(1,0,0,1,1); widgetControlScanator->setScheme(0,0,1,4); 
    widgetControlPlatform->enableScheme(1,0,0,1,1); widgetControlPlatform->setScheme(0,0,1,4);

      widgetLidControl = new WidgetDeviceControl("Крышки"); 
      widgetLidControl->enableScheme(0,0,1,0,0); widgetLidControl->setScheme(0,0,2,0); 

    ui->layoutControlTable->addWidget(widgetControlRanger,1,1);
    ui->layoutControlTable->addWidget(widgetControlFocusator,2,1);
    ui->layoutControlTable->addWidget(widgetControlScanator,3,1);
    ui->layoutControlTable->addWidget(widgetControlPlatform,4,1);
    ui->layoutControlTable->addWidget(widgetLidControl,5,1);

    ui->layoutControlTable->addWidget(widgetControlLaserPower,1,2);
    ui->layoutControlTable->addWidget(widgetControlLaserIllum,2,2);
       ui->layoutControlTable->addWidget(widgetControlCamera1,3,2);
       ui->layoutControlTable->addWidget(widgetControlCamera2,4,2);
       ui->layoutControlTable->addWidget(widgetControlCamera3,5,2);
       ui->layoutControlTable->addWidget(widgetControlCamera4,6,2);

    ui->layoutControlBlock->addWidget(widgetControlBlock);
    ui->layoutControlBlock->addWidget(widgetControlRotary1);
    ui->layoutControlBlockBigPanel->addWidget(widgetControlRotary2);

    ui->layoutControlBlockBigPanel->addSpacerItem(new QSpacerItem(100,100,QSizePolicy::Fixed, QSizePolicy::Minimum));

    ui->widgetSwitcherFullMode->hideButton(1);

    connect(ui->labelCameraFast, &LabelActiveImage::signalLabelPicked, this, &WidgetComplexInterface::slotSetBigImageMode);
    connect(ui->labelCameraZoom, &LabelActiveImage::signalLabelPicked, this, &WidgetComplexInterface::slotSetBigImageMode);
    connect(ui->labelCameraThermal, &LabelActiveImage::signalLabelPicked, this, &WidgetComplexInterface::slotSetBigImageMode);


    outputVideo1 = new SinkDisplayLabel;
    outputVideo2 = new SinkDisplayLabel;
    outputVideo3 = new SinkDisplayLabel;

    outputVideo1Control = new SinkDisplayLabel;
    outputVideo2Control = new SinkDisplayLabel;
    outputVideo3Control = new SinkDisplayLabel;

    outputVideo1Mini = new SinkDisplayLabel;
    outputVideo2Mini = new SinkDisplayLabel;
    outputVideo3Mini = new SinkDisplayLabel;

    outputVideoBig = new SinkDisplayLabel;

    outputVideo1->linkToDisplay(ui->labelCameraFast);
    outputVideo2->linkToDisplay(ui->labelCameraZoom);
    outputVideo3->linkToDisplay(ui->labelCameraThermal);

    outputVideo1Mini->linkToDisplay(ui->labelMiniCameraFast);
    outputVideo2Mini->linkToDisplay(ui->labelMiniCameraZoom);
    outputVideo3Mini->linkToDisplay(ui->labelMiniCameraThermal);
      outputVideoBig->linkToDisplay(ui->labelCameraBig);
      
    outputVideo1Mini->linkToSinkNode(outputVideoBig);
    outputVideo2Mini->linkToSinkNode(outputVideoBig);
    outputVideo3Mini->linkToSinkNode(outputVideoBig);

    slotSetMainMode();
    //slotSetBigImageMode();
    //slotSetHandleMode();
    //slotSetControlPanelMode();
    this->grabKeyboard();
}

void WidgetComplexInterface::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F11)
    {
      if(!this->isFullScreen()) { this->showFullScreen(); return; } 
      if( this->isFullScreen()) { this->showNormal(); return; }
    }
    if(event->isAutoRepeat()) return;

    if(event->key() == Qt::Key_Up)   qDebug() << "KEY PRESS UP"; 
    if(event->key() == Qt::Key_Down) qDebug() << "KEY PRESS DOWN"; 
    if(event->key() == Qt::Key_Left) qDebug() << "KEY PRESS LEFT"; 
    if(event->key() == Qt::Key_Right)qDebug() << "KEY PRESS RIGHT"; 


    if(event->key() == Qt::Key_W) qDebug() << "KEY W UP"; 
    if(event->key() == Qt::Key_S) qDebug() << "KEY S DOWN"; 
    if(event->key() == Qt::Key_A) qDebug() << "KEY A LEFT"; 
    if(event->key() == Qt::Key_D) qDebug() << "KEY D RIGHT"; 

    switch(event->key())
    {
      case Qt::Key_Up:    widgetControlRotary1->widgetElevation->slotMoveStart(1);  break;
      case Qt::Key_Down:  widgetControlRotary1->widgetElevation->slotMoveStart(-1); break;
      case Qt::Key_Left:  widgetControlRotary1->widgetAzimuth->slotMoveStart(1);    break;
      case Qt::Key_Right: widgetControlRotary1->widgetAzimuth->slotMoveStart(-1);   break;

      case Qt::Key_W: widgetControlRotary1->widgetElevation->slotMoveStart(1);  break;
      case Qt::Key_S: widgetControlRotary1->widgetElevation->slotMoveStart(-1); break;
      case Qt::Key_A: widgetControlRotary1->widgetAzimuth->slotMoveStart(1);    break;
      case Qt::Key_D: widgetControlRotary1->widgetAzimuth->slotMoveStart(-1);   break;
    }

    //QWidget::keyPressEvent(event);
}

void WidgetComplexInterface::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat()) return;

    if(event->key() == Qt::Key_Up)   qDebug() << "KEY RELEASE UP"; 
    if(event->key() == Qt::Key_Down) qDebug() << "KEY RELEASE DOWN"; 
    if(event->key() == Qt::Key_Left) qDebug() << "KEY RELEASE LEFT"; 
    if(event->key() == Qt::Key_Right)qDebug() << "KEY RELEASE RIGHT"; 
    if(event->key() == Qt::Key_W) qDebug() << "KEY W RELEASE UP"; 
    if(event->key() == Qt::Key_S) qDebug() << "KEY S RELEASE DOWN"; 
    if(event->key() == Qt::Key_A) qDebug() << "KEY A RELEASE LEFT"; 
    if(event->key() == Qt::Key_D) qDebug() << "KEY D RELEASE RIGHT"; 

    switch(event->key())
    {
      case Qt::Key_Up:    widgetControlRotary1->widgetElevation->slotMoveStart(0); break;
      case Qt::Key_Down:  widgetControlRotary1->widgetElevation->slotMoveStart(0); break;
      case Qt::Key_Left:  widgetControlRotary1->widgetAzimuth->slotMoveStart(0);   break;
      case Qt::Key_Right: widgetControlRotary1->widgetAzimuth->slotMoveStart(0);   break;

      case Qt::Key_W: widgetControlRotary1->widgetElevation->slotMoveStart(0); break;
      case Qt::Key_S: widgetControlRotary1->widgetElevation->slotMoveStart(0); break;
      case Qt::Key_A: widgetControlRotary1->widgetAzimuth->slotMoveStart(0);   break;
      case Qt::Key_D: widgetControlRotary1->widgetAzimuth->slotMoveStart(0);   break;
    }

}

void WidgetComplexInterface::activateMainOutput(bool OnOff) 
{
  outputVideo1->slotActivate(OnOff);
  outputVideo2->slotActivate(OnOff);
  outputVideo3->slotActivate(OnOff); if(!OnOff) return;

  activateControlOutput(!OnOff);
      activateBigOutput(!OnOff);
}
void WidgetComplexInterface::activateControlOutput(bool OnOff)
{
  outputVideo1Control->slotActivate(OnOff);
  outputVideo2Control->slotActivate(OnOff);
  outputVideo3Control->slotActivate(OnOff); if(!OnOff) return;

     activateMainOutput(!OnOff);
      activateBigOutput(!OnOff);
}
void WidgetComplexInterface::activateBigOutput(bool OnOff)
{
  outputVideo1Mini->slotActivate(OnOff);
  outputVideo2Mini->slotActivate(OnOff);
  outputVideo3Mini->slotActivate(OnOff);
    outputVideoBig->slotActivate(OnOff); if(!OnOff) return;

       activateMainOutput(!OnOff);
    activateControlOutput(!OnOff);
}

WidgetComplexInterface::~WidgetComplexInterface() 
{ 
    delete ui; 
}

void WidgetComplexInterface::closeEvent(QCloseEvent *event) 
{ 
  qDebug() << "[ MAIN WINDOW ]" << "CLOSE EVENT";
  emit signalEndWork();  
  QThread::sleep(4);
  QWidget::closeEvent(event);
}
void WidgetComplexInterface::slotEndWork(QCloseEvent *event) { qDebug() << "[ END WORK ]"; QWidget::closeEvent(event); }


void WidgetComplexInterface::slotSetMainMode()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->widgetStackedMainControl->setCurrentIndex(0);
        widgetControlBlock->show();
    activateMainOutput(true);
}
void WidgetComplexInterface::slotSetHandleMode()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->widgetStackedMainControl->setCurrentIndex(1);

  //ControlPanelSwitcher->show();
  //       ControlRotary->show(); ControlBlock->hide();
  //   activateMainOutput(true);
}
void WidgetComplexInterface::slotSetBigImageMode()
{
   qDebug() << "[ SLOT FULL IMAGE MODE ]";
   ui->stackedWidget->setCurrentIndex(1);
    activateBigOutput(true);
}
void WidgetComplexInterface::slotSetControlPanelMode()
{
   ui->stackedWidget->setCurrentIndex(1);
    activateControlOutput(true);
}

void WidgetComplexInterface::slotShowMalfunctionList()
{

}
