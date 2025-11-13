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
    ui->butLaserReady->setEnabled(false);
}

WidgetLaserControl::~WidgetLaserControl() 
{ 
    delete ui; 
}


void WidgetLaserControl::linkTo(DeviceLaserGenericInterface* LaserControl)
{
    ui->labelLaserPowerStatus->setText(LaserControl->getName());
    QObject::connect(ui->butLaserReady, &QPushButton::toggled, [LaserControl](bool OnOff)
    {
           LaserControl->setEnable(OnOff);
    });

    QObject::connect(ui->butLaserPower, &QPushButton::toggled, [LaserControl](bool OnOff)
    {
           LaserControl->setPowerEnable(OnOff);
    });

    QObject::connect(ui->butLaserPilot, &QPushButton::toggled, [LaserControl](bool OnOff)
    {
           LaserControl->setPilotEnable(OnOff);
    });

    //QObject::connect(ui->scrollSetPower, &QScrollBar::valueChanged, [LaserControl](int Value)
    //{
    //       LaserControl->setPower(Value);
    //});

    QObject::connect(ui->butSetPowerHigh, &QPushButton::toggled, [LaserControl](bool OnOff)
    {
           if(OnOff) { LaserControl->setPowerHigh(); qDebug() << "[ LASER POWER HIGH ]"; }
    });
    QObject::connect(ui->butSetPowerMiddle, &QPushButton::toggled, [LaserControl](bool OnOff)
    {
           if(OnOff) { LaserControl->setPowerMiddle(); qDebug() << "[ LASER POWER MIDDLE ]";} 
    });
    QObject::connect(ui->butSetPowerLow, &QPushButton::toggled, [LaserControl](bool OnOff)
    {
           if(OnOff) { LaserControl->setPowerLow(); qDebug() << "[ LASER POWER LOW ]";}
    });

}

void WidgetLaserControl::setState(int State)
{

}

void WidgetLaserControl::setParam(int ID, int Param)
{

}
