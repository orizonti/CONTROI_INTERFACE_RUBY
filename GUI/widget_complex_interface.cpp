#include <QSpinBox>
#include "./ui_widget_complex_interface.h"
#include "widget_complex_interface.h"
#include <QTimer>
#include <QKeyEvent>
#include <QtCore\qmath.h>
#include <QGraphicsPixmapItem>
#include "label_active_image.h"
#include <QSizePolicy>

#include "message_header_generic_ext.h"
#include "message_struct_generic_ext.h"
#include "message_command_structures.h"

static QString styleButtons{

"QPushButton {"
"background-color: rgba(209, 142, 34, 60);"
"border: 2px solid line;"
"border-radius: 6px;"
"border-color: rgb(214, 136, 41); }"

"QPushButton:pressed {"
"background-color: rgba(209, 142, 34, 110);"
"border: 4px solid line;"
"border-radius: 6px;"
"border-color: rgb(171, 86, 38); }"
};

WidgetComplexInterface::WidgetComplexInterface(QWidget *parent)
    : WidgetAdjustable(parent)
    , ui(new Ui::WidgetComplexInterface)
{
    ui->setupUi(this);

    qDebug() << "CREATE COMPLEX INTERFACE";

    widgetMainControl1 = new WidgetMainControl(0);
    widgetMainControl2 = new WidgetMainControl(1);
    widgetMainControl1->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    widgetMainControl2->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

    widgetMainControl1->synchronizePeer(widgetMainControl2);

    QObject::connect(&widgetMainControl1->butControl1, SIGNAL(clicked()), this, SLOT(slotSetHandleMode()));
    QObject::connect(&widgetMainControl1->butControl2, SIGNAL(clicked()), this, SLOT(slotSetBigImageMode()));

    QObject::connect(&widgetMainControl2->butControl1, SIGNAL(clicked()), this, SLOT(slotSetHandleMode()));
    QObject::connect(&widgetMainControl2->butControl2, SIGNAL(clicked()), this, SLOT(slotSetMainMode()));



       
     widgetControlRanger    = new WidgetDeviceControl("Дальномер");
     widgetControlFocusator = new WidgetDeviceControl("Фокусатор");
    widgetControlLaserPower = new WidgetDeviceControl("Лазер    "); 
    widgetControlLaserIllum = new WidgetDeviceControl("Подсвет  "); 
      widgetControlScanator = new WidgetDeviceControl("Сканатор "); 
      widgetControlPlatform = new WidgetDeviceControl("Платформа"); 
           widgetLidControl = new WidgetDeviceControl("Крышки"); 


        widgetControlRanger->enableScheme(1,0,0,1,0);     widgetControlRanger->setScheme(0,1);   
     widgetControlFocusator->enableScheme(1,0,0,1,1);  widgetControlFocusator->setScheme(0,1,2);
      widgetControlScanator->enableScheme(1,0,0,1,1);   widgetControlScanator->setScheme(0,1,4); 
      widgetControlPlatform->enableScheme(1,0,0,1,1);   widgetControlPlatform->setScheme(0,1,4);
           widgetLidControl->enableScheme(0,0,0,1,0);        widgetLidControl->setScheme(0,2,0); 
    widgetControlLaserPower->enableScheme(0,0,1,1,0); widgetControlLaserPower->setScheme(3,2);
    widgetControlLaserIllum->enableScheme(0,0,1,1,0); widgetControlLaserIllum->setScheme(3,1);

    WidgetDeviceControl* widget_test = new WidgetDeviceControl;
    widget_test->enableScheme(0,0,0,1,0); widget_test->setScheme(0,4); widget_test->setButtonsMode({true,true, false,false});
    widget_test->show();


      widgetControlScanator->setButtonsMode({false});
      widgetControlPlatform->setButtonsMode({false});
      widgetControlRanger->setLabelMode(1);
      widgetControlFocusator->setLabelMode(1);
      widgetControlLaserPower->setLevelsName({"20%","50%","100%"});
      widgetControlLaserIllum->setLevelsName({"20%","50%","100%"});

        widgetControlRanger->setButtonsName({"ПУСК", "ПУСК"});     
     widgetControlFocusator->setButtonsName({"ПУСК", "ПУСК"});
    widgetControlLaserPower->setButtonsName({"ПУСК", "ПИЛОТ"});
    widgetControlLaserIllum->setButtonsName({"ПУСК", "ПУСК"});
      widgetControlScanator->setButtonsName({"СБРОС", "ПУСК"}); 
      widgetControlPlatform->setButtonsName({"СБРОС", "ПУСК"});
           widgetLidControl->setButtonsName({"ПУСК", "ПУСК"}); 

    buttonSwitcher = new QPushButton("РАБОТА");
    buttonSwitcher->setStyleSheet(styleButtons);
    buttonSwitcher->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    buttonSwitcher->setMaximumSize(widgetLidControl->maxButtonsSize);
    QObject::connect(buttonSwitcher, SIGNAL(clicked()), this, SLOT(slotSetMainMode()));


    ui->layoutControlTable->setSpacing(1);
    ui->layoutControlTable->setContentsMargins(2,2,2,2);
    ui->layoutControlTable->addWidget(widgetControlRanger   ,1,0);
    ui->layoutControlTable->addWidget(widgetControlFocusator,2,0);
    ui->layoutControlTable->addWidget(widgetControlScanator ,3,0);
    ui->layoutControlTable->addWidget(widgetControlPlatform ,4,0);
    ui->layoutControlTable->addWidget(widgetLidControl      ,5,0);

    ui->layoutControlTable->addWidget(widgetControlLaserPower,6,0);
    ui->layoutControlTable->addWidget(widgetControlLaserIllum,7,0);

    ui->layoutControlTable->addWidget(buttonSwitcher,1,1);

            ui->layoutControlBlock->addWidget(widgetMainControl1);
    ui->layoutControlBlockBigPanel->addWidget(widgetMainControl2);

    ui->layoutControlBlockBigPanel->addSpacerItem(new QSpacerItem(100,100,QSizePolicy::Fixed, QSizePolicy::Minimum));

    //========================================
           QImage image(720,540,QImage::Format_RGB888);
                  image.fill(Qt::black);

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

        outputVideo1->linkToDisplay(ui->labelCameraZoom);
        outputVideo2->linkToDisplay(ui->labelCameraFast);
        outputVideo3->linkToDisplay(ui->labelCameraThermal);
           outputVideo1->setImage(image);
           outputVideo2->setImage(image);
           outputVideo3->setImage(image);

    outputVideo1Mini->linkToDisplay(ui->labelMiniCameraZoom);
    outputVideo2Mini->linkToDisplay(ui->labelMiniCameraFast);
    outputVideo3Mini->linkToDisplay(ui->labelMiniCameraThermal);
      outputVideoBig->linkToDisplay(ui->labelCameraBig);
      
    outputVideo1Mini->linkToSinkNode(outputVideoBig);
    outputVideo2Mini->linkToSinkNode(outputVideoBig);
    outputVideo3Mini->linkToSinkNode(outputVideoBig);
    //========================================

       ui->labelCameraFast->setMaximumSize(800,350);
       ui->labelCameraZoom->setMaximumSize(800,350);
    ui->labelCameraThermal->setMaximumSize(800,350);

       widgetControlCamera1Float = new WidgetDeviceControl("КамераТК ", Qt::Vertical, this); 
       widgetControlCamera2Float = new WidgetDeviceControl("КамераГК ", Qt::Vertical, this); 
       widgetControlCamera3Float = new WidgetDeviceControl("Тепловиз ", Qt::Vertical, this); 

       widgetControlCamera1Float->enableScheme(0,0,1,0,0,0); widgetControlCamera1Float->setScheme(6,0,0);
       widgetControlCamera2Float->enableScheme(0,0,1,0,0,0); widgetControlCamera2Float->setScheme(6,0,0);
       widgetControlCamera3Float->enableScheme(0,0,1,0,0,0); widgetControlCamera3Float->setScheme(6,0,0);

       widgetControlCamera1Float->move(ui->labelCameraZoom->pos().x(), ui->labelCameraZoom->pos().y());  
       widgetControlCamera2Float->move(ui->labelCameraZoom->pos().x(), ui->labelCameraZoom->pos().y());  
       widgetControlCamera3Float->move(
       ui->labelCameraZoom->pos().x() + ui->labelCameraZoom->width() + 170, 
       ui->labelCameraZoom->pos().y() + 70); 

       buttonSwitcher2 = new QPushButton("<- ->", this);
       buttonSwitcher2->setStyleSheet(styleButtons);
       buttonSwitcher2->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
       buttonSwitcher2->setMaximumSize(70,50);
       buttonSwitcher2->hide();
       //QObject::connect(buttonSwitcher2, SIGNAL(clicked()), this, SLOT(slotSetBigImageMode()));
       buttonSwitcher2->move(
       ui->labelCameraZoom->pos().x() + 20, 
       ui->labelCameraZoom->pos().y() + ui->labelCameraZoom->height() + 80); 

       
       //widgetControlCamera1Float->show();
       //widgetControlCamera2Float->show();
       //widgetControlCamera3Float->show();

       widgetsHidden.push_back(widgetControlCamera3Float);
       buttonsHidden.push_back(buttonSwitcher2);


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
      case Qt::Key_Up:    widgetMainControl1->widgetElevation->slotMoveStart(1);  break;
      case Qt::Key_Down:  widgetMainControl1->widgetElevation->slotMoveStart(-1); break;
      case Qt::Key_Left:  widgetMainControl1->widgetAzimuth->slotMoveStart(1);    break;
      case Qt::Key_Right: widgetMainControl1->widgetAzimuth->slotMoveStart(-1);   break;

      case Qt::Key_W: widgetMainControl1->widgetElevation->slotMoveStart(1);  break;
      case Qt::Key_S: widgetMainControl1->widgetElevation->slotMoveStart(-1); break;
      case Qt::Key_A: widgetMainControl1->widgetAzimuth->slotMoveStart(1);    break;
      case Qt::Key_D: widgetMainControl1->widgetAzimuth->slotMoveStart(-1);   break;
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
      case Qt::Key_Up:    widgetMainControl1->widgetElevation->slotMoveStart(0); break;
      case Qt::Key_Down:  widgetMainControl1->widgetElevation->slotMoveStart(0); break;
      case Qt::Key_Left:  widgetMainControl1->widgetAzimuth->slotMoveStart(0);   break;
      case Qt::Key_Right: widgetMainControl1->widgetAzimuth->slotMoveStart(0);   break;

      case Qt::Key_W: widgetMainControl1->widgetElevation->slotMoveStart(0); break;
      case Qt::Key_S: widgetMainControl1->widgetElevation->slotMoveStart(0); break;
      case Qt::Key_A: widgetMainControl1->widgetAzimuth->slotMoveStart(0);   break;
      case Qt::Key_D: widgetMainControl1->widgetAzimuth->slotMoveStart(0);   break;
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
    activateMainOutput(true);
}
void WidgetComplexInterface::slotSetHandleMode()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->widgetStackedMainControl->setCurrentIndex(1);
}
void WidgetComplexInterface::slotSetBigImageMode()
{
   ui->stackedWidget->setCurrentIndex(1);
   activateBigOutput(true);
}

void WidgetComplexInterface::slotShowMalfunctionList() { }
void WidgetComplexInterface::slotShowHiddenWidgets() { for(auto& widget: widgetsHidden) widget->show(); 
                                                       for(auto& widget: buttonsHidden) widget->show(); }
void WidgetComplexInterface::slotHideHiddenWidgets() { for(auto& widget: widgetsHidden) widget->hide();
                                                       for(auto& widget: buttonsHidden) widget->hide(); }


void WidgetComplexInterface::installEventFilter(KeyboardFilter *filterObj)
{
  connect(filterObj, &KeyboardFilter::signalControlAlt, this, &WidgetComplexInterface::slotShowHiddenWidgets);
  connect(filterObj, &KeyboardFilter::signalRelease, this, &WidgetComplexInterface::slotHideHiddenWidgets);
  QWidget::installEventFilter(filterObj);
}