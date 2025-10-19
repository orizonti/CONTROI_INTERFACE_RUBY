#include <QSpinBox>
#include "./ui_widget_laser_control.h"
#include "widget_laser_control.h"
#include <QTimer>
#include "device_laser_interface.h"

WidgetLaserControl::WidgetLaserControl(QWidget *parent)
    : WidgetAdjustable(parent)
    , ui(new Ui::WidgetLaserControl)
{
    ui->setupUi(this);
}

WidgetLaserControl::~WidgetLaserControl() 
{ 
    delete ui; 
}


void WidgetLaserControl::linkTo(DeviceLaserGenericInterface* LaserControl)
{

}

void WidgetLaserControl::setState(int State)
{

}

void WidgetLaserControl::setParam(int ID, int Param)
{

}
