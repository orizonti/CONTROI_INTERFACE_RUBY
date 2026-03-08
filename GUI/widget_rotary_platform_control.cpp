#include "widget_rotary_platform_control.h"
#include "ui_widget_rotary_platform_control.h"
#include "debug_output_filter.h"
#include "widget_rotary_control.h"
WidgetRotaryPlatformControl::WidgetRotaryPlatformControl(QWidget* parent) : WidgetAdjustable(parent), ui(new Ui::WidgetRotaryPlatformControl)
{
    qDebug() << " [ CREATE WIDGET CAMERA CONTROL ]";
    ui->setupUi(this);
 //   QObject::connect(ui->scrollRotaryAxisX, &QScrollBar::valueChanged, this, &WidgetRotaryPlatformControl::slotValue1Changed);
 //   QObject::connect(ui->scrollRotaryAxisY, &QScrollBar::valueChanged, this, &WidgetRotaryPlatformControl::slotValue2Changed);
    ui->groupBoxArrowButtons->hide();
    ui->widgetRotaryAzimuth->setType(WidgetRotaryControl::TypeDraw::TypeCircle);
}

void WidgetRotaryPlatformControl::linkToDevice(std::shared_ptr<DeviceRotaryInterface> Device)
{

//    float VelocityScale = 0.5;
//    float VelocityScale = 2;
//
//    std::vector<QPushButton*> ArrowButtons;
//    std::vector<QPair<float,float>> Vels;
//    ArrowButtons.push_back(ui->butRotaryX_Left);  Vels.push_back(QPair<float,float>(-VelocityScale, 0));
//    ArrowButtons.push_back(ui->butRotaryX_Right); Vels.push_back(QPair<float,float>( VelocityScale, 0));
//    ArrowButtons.push_back(ui->butRotaryY_Up);    Vels.push_back(QPair<float,float>( 0, VelocityScale));
//    ArrowButtons.push_back(ui->butRotaryY_Down);  Vels.push_back(QPair<float,float>( 0,-VelocityScale));
//
//    for(int n = 0; n < 4; n++)
//    {
//    auto Velocity = Vels[n];
//    auto button = ArrowButtons[n];
//    QObject::connect(button, &QPushButton::pressed,  [this, Device, Velocity]() { Device->moveWithVelocityManual(Velocity); timerCheckState.start(1);});
//    //QObject::connect(button, &QPushButton::pressed,  [this, Device, Velocity]() { Device->moveWithVelocity(QPair<float,float>(0.5,0.5)); timerCheckState.start(1);});
//    QObject::connect(button, &QPushButton::released, [this, Device     ]()      { Device->stopMove(); timerCheckState.stop();});
//    }
//
//    auto  MoveLimits = Device->getLimits();
//    qDebug() << "GET LIMITS : " << MoveLimits.first << MoveLimits.second;
//    float RangeScale = 2000;
//
//    ui->scrollRotaryAxisX->setRange(-MoveLimits.first*RangeScale, MoveLimits.first*RangeScale);
//    ui->scrollRotaryAxisX->setValue(0);
//    ui->scrollRotaryAxisX->setPageStep(50);
//
//    ui->scrollRotaryAxisY->setRange(-MoveLimits.second*RangeScale, MoveLimits.second*RangeScale);
//    ui->scrollRotaryAxisY->setValue(0);
//    ui->scrollRotaryAxisY->setPageStep(50);
//
//    std::vector<QScrollBar*> scrollWidgets;
//                             scrollWidgets.push_back(ui->scrollRotaryAxisX);
//                             scrollWidgets.push_back(ui->scrollRotaryAxisY);
//
//    for(auto scroll_axis: scrollWidgets)
//    {
//    QObject::connect(scroll_axis, &QScrollBar::sliderReleased, [Device, this]() 
//    { 
//        Device->moveToPos(this->Position); 
//    });
//    }
//
//    QObject::connect(&timerCheckState, &QTimer::timeout, [Device,RangeScale,this]()
//    {
//                                        Position = Device->getPos()*RangeScale; 
//        ui->scrollRotaryAxisX->setValue(Position.first); 
//        ui->scrollRotaryAxisY->setValue(Position.second);
//    });
}


void WidgetRotaryPlatformControl::setName(QString name) 
{ 
};
