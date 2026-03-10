#include <QSpinBox>
#include "./ui_widget_complex_interface.h"
#include "widget_complex_interface.h"
#include <QTimer>
#include <QKeyEvent>
#include <QtCore\qmath.h>
#include <QGraphicsPixmapItem>
#include "label_active_image.h"

#include "message_command_id.h"

WidgetComplexInterface::WidgetComplexInterface(QWidget *parent)
    : WidgetAdjustable(parent)
    , ui(new Ui::WidgetComplexInterface)
{
    ui->setupUi(this);

    widgetControlBlock = new WidgetControlBlock;
    widgetControlRotary  = new WidgetRotaryPlatformControl;
    widgetControlRotary2 = new WidgetRotaryPlatformControl;

    widgetControlRanger    = new WidgetDeviceControl("Дальномер");
    widgetControlFocusator = new WidgetDeviceControl("Фокусатор");

    widgetControlRanger->enableScheme(1,0,1,0);    widgetControlRanger->setScheme(0,0,1); 
    widgetControlFocusator->enableScheme(1,0,1,0); widgetControlFocusator->setScheme(0,0,1);

    widgetControlLaserPower = new WidgetDeviceControl("Лазер    "); 
    widgetControlLaserIllum = new WidgetDeviceControl("Подсвет  "); 
    widgetControlLaserPower->enableScheme(1,1,1,0); widgetControlLaserPower->setScheme(0,2,2); 
    widgetControlLaserIllum->enableScheme(1,1,1,0); widgetControlLaserIllum->setScheme(0,2,1);

       widgetControlCamera1 = new WidgetDeviceControl("КамераТК "); 
       widgetControlCamera2 = new WidgetDeviceControl("КамераТК "); 
       widgetControlCamera3 = new WidgetDeviceControl("КамераГК "); 
       widgetControlCamera4 = new WidgetDeviceControl("Тепловиз "); 
    widgetControlCamera1->enableScheme(1,1,0,0); widgetControlCamera1->setScheme(1,5,0); widgetControlCamera1->setLevelsName(QStringList({"X1", "X2", "X3", "X4", "X5"}));
    widgetControlCamera2->enableScheme(1,1,0,0); widgetControlCamera2->setScheme(1,5,0); widgetControlCamera2->setLevelsName(QStringList({"X1", "X2", "X3", "X4", "X5"})); 
    widgetControlCamera3->enableScheme(1,1,0,0); widgetControlCamera3->setScheme(1,5,0); widgetControlCamera3->setLevelsName(QStringList({"X1", "X2", "X3", "X4", "X5"}));
    widgetControlCamera4->enableScheme(1,1,0,0); widgetControlCamera4->setScheme(1,5,0); widgetControlCamera4->setLevelsName(QStringList({"X1", "X2", "X3", "X4", "X5"}));


      widgetControlScanator = new WidgetDeviceControl("Сканатор "); 
      widgetControlPlatform = new WidgetDeviceControl("Платформа"); 
    widgetControlScanator->enableScheme(1,0,0,1); widgetControlScanator->setScheme(0,0,1); 
    widgetControlPlatform->enableScheme(1,0,0,1); widgetControlPlatform->setScheme(0,0,1);

    ui->layoutControlListRight->addWidget(widgetControlRanger);
    ui->layoutControlListRight->addWidget(widgetControlFocusator);

    ui->layoutControlListRight->addWidget(widgetControlScanator);
    ui->layoutControlListRight->addWidget(widgetControlPlatform);

    ui->layoutControlListLeft->addWidget(widgetControlLaserPower);
    ui->layoutControlListLeft->addWidget(widgetControlLaserIllum);
    ui->layoutControlListLeft->addWidget(widgetControlCamera1);
    ui->layoutControlListLeft->addWidget(widgetControlCamera2);
    ui->layoutControlListLeft->addWidget(widgetControlCamera3);
    ui->layoutControlListLeft->addWidget(widgetControlCamera4);

    ui->layoutControlBlock->addWidget(widgetControlBlock);
    ui->layoutControlBlock->addWidget(widgetControlRotary);

    ui->layoutControlBlockBigPanel->addWidget(widgetControlRotary2);

    ui->widgetSwitcherFullMode->hideButton(1);

    //ControlBlock->LinkSignals(this);

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

    //slotSetMainMode();
    //slotSetBigImageMode();
    slotSetHandleMode();
    //slotSetControlPanelMode();
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

void WidgetComplexInterface::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F11)
    {
      if(!this->isFullScreen()) { this->showFullScreen(); return; } 
      if( this->isFullScreen()) { this->showNormal(); return; }
    }
}

void WidgetComplexInterface::slotSetMainMode()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->widgetStackedMainControl->setCurrentIndex(0);
           widgetControlRotary->hide(); widgetControlBlock->show();
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
