#include <QSpinBox>
#include "./ui_widget_complex_interface.h"
#include "widget_complex_interface.h"
#include <QTimer>
#include <QKeyEvent>
#include <QtCore\qmath.h>
#include <QGraphicsPixmapItem>
#include "label_active_image.h"

WidgetComplexInterface::WidgetComplexInterface(QWidget *parent)
    : WidgetAdjustable(parent)
    , ui(new Ui::WidgetComplexInterface)
{
    ui->setupUi(this);

    ControlBlock = new WidgetControlBlock;
    ControlRotary = new WidgetRotaryPlatformControl;
    ControlFocusRanger = new WidgetFocusRangerControl;
    ControlPanelSwitcher = new WidgetPanelSwitcher;

    ControlRotaryPanel   = ui->widgetRotaryPlatformControl;
    ControlScanatorPanel = ui->widgetScanatorControl;

    //ControlBlock->show();
    ControlFocusRanger->setMaximumHeight(220);
    ControlPanelSwitcher->setMaximumHeight(220);
    ControlPanelSwitcher->setMaximumWidth(80);
    ControlPanelSwitcher->hideButton(1);

    ui->layoutControlBlock->addWidget(ControlBlock);

    ui->layoutControlBlock->addWidget(ControlRotary);
    ui->layoutControlBlock->addWidget(ControlFocusRanger);
    ui->layoutControlBlock->addWidget(ControlPanelSwitcher);
    ui->widgetSwitcherFullMode->hideButton(1);

    connect(ControlPanelSwitcher, &WidgetPanelSwitcher::signalRegim1, this, &WidgetComplexInterface::slotSetMainMode);
    connect(ControlPanelSwitcher, &WidgetPanelSwitcher::signalRegim2, this, &WidgetComplexInterface::slotSetHandleMode);
    connect(ControlPanelSwitcher, &WidgetPanelSwitcher::signalRegim3, this, &WidgetComplexInterface::slotSetControlPanelMode);

    connect(ui->widgetSwitcher, &WidgetPanelSwitcher::signalRegim1, this, &WidgetComplexInterface::slotSetMainMode);
    connect(ui->widgetSwitcher, &WidgetPanelSwitcher::signalRegim2, this, &WidgetComplexInterface::slotSetHandleMode);
    connect(ui->widgetSwitcher, &WidgetPanelSwitcher::signalRegim3, this, &WidgetComplexInterface::slotSetControlPanelMode);

    connect(ui->widgetSwitcherFullMode, &WidgetPanelSwitcher::signalRegim1, this, &WidgetComplexInterface::slotSetMainMode);
    connect(ui->widgetSwitcherFullMode, &WidgetPanelSwitcher::signalRegim2, this, &WidgetComplexInterface::slotSetHandleMode);
    connect(ui->widgetSwitcherFullMode, &WidgetPanelSwitcher::signalRegim3, this, &WidgetComplexInterface::slotSetControlPanelMode);
    ControlBlock->LinkSignals(this);

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

    outputVideo1Control->linkToDisplay(ui->labelCameraFastControl);
    outputVideo2Control->linkToDisplay(ui->labelCameraZoomControl);
    outputVideo3Control->linkToDisplay(ui->labelCameraThermalControl);

    outputVideo1Mini->linkToDisplay(ui->labelMiniCameraFast);
    outputVideo2Mini->linkToDisplay(ui->labelMiniCameraZoom);
    outputVideo3Mini->linkToDisplay(ui->labelMiniCameraThermal);
      outputVideoBig->linkToDisplay(ui->labelCameraBig);
      
    outputVideo1Mini->linkToSinkNode(outputVideoBig);
    outputVideo2Mini->linkToSinkNode(outputVideoBig);
    outputVideo3Mini->linkToSinkNode(outputVideoBig);

    slotSetHandleMode();
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
      ControlFocusRanger->hide();
    ControlPanelSwitcher->hide();
           ControlRotary->hide(); ControlBlock->show();
    activateMainOutput(true);
}
void WidgetComplexInterface::slotSetHandleMode()
{
    ui->stackedWidget->setCurrentIndex(0);

    ControlFocusRanger->show();
  ControlPanelSwitcher->show();
         ControlRotary->show(); ControlBlock->hide();
     activateMainOutput(true);
}
void WidgetComplexInterface::slotSetBigImageMode()
{
   qDebug() << "[ SLOT FULL IMAGE MODE ]";
   ui->stackedWidget->setCurrentIndex(2);
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
