#include "widget_rotary_platform_control.h"
#include "ui_widget_rotary_platform_control.h"
#include "debug_output_filter.h"
#include "widget_rotary_control.h"
#include "device_rotary_interface.h"
#include "QSpacerItem"
#include <QPushButton>

WidgetRotaryPlatformControl::WidgetRotaryPlatformControl(int Scheme, QWidget* parent) : WidgetAdjustable(parent), ui(new Ui::WidgetRotaryPlatformControl)
{
    ui->setupUi(this);
    ui->groupBoxArrowButtons->hide();

    widgetElevation = new WidgetRotaryControl;
    widgetAzimuth   = new WidgetRotaryControl;
    widgetElevation->setType(WidgetRotaryControl::TypeDraw::TypeArc);

    if(Scheme == 0)
    {
    widgetElevation->setMaximumSize(110,110);
    ui->leftLayout->addWidget(widgetAzimuth);
    ui->rightLayout->addWidget(widgetElevation, Qt::AlignTop);

    widgetAzimuth->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    widgetElevation->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

    auto button = new QPushButton("button"); ui->rightLayout->addWidget(button, Qt::AlignTop); button->setMinimumHeight(40);
         button = new QPushButton("button"); ui->rightLayout->addWidget(button, Qt::AlignTop); button->setMinimumHeight(40);
         button = new QPushButton("button"); ui->rightLayout->addWidget(button, Qt::AlignTop); button->setMinimumHeight(40);
    ui->rightLayout->addSpacerItem(new QSpacerItem(20,100,QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    }

    if(Scheme == 1)
    {

    widgetElevation->setMinimumSize(80,80);
    widgetElevation->setMaximumSize(150,150);
    widgetAzimuth->setMaximumSize(340,340);
    widgetAzimuth->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    widgetElevation->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

    auto layout = new  QHBoxLayout;
         layout->setSpacing(0);
         layout->setContentsMargins(1,1,1,1);
         ui->leftLayout->setContentsMargins(1,1,1,1);
         ui->leftLayout->setSpacing(0);
         layout->addSpacerItem(new QSpacerItem(50,10,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
         layout->addWidget(widgetElevation, Qt::AlignLeft);
    ui->leftLayout->addLayout(layout);
    ui->leftLayout->addWidget(widgetAzimuth);

    widgetAzimuth->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    widgetElevation->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    }


}

void WidgetRotaryPlatformControl::linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device)
{
     widgetAzimuth->linkToDevice(Device);
     widgetElevation->linkToDevice(Device);

//    QObject::connect(&timerCheckState, &QTimer::timeout, [Device,RangeScale,this]()
//    {
//                                        Position = Device->getPos()*RangeScale; 
//    });
}


void WidgetRotaryPlatformControl::setName(QString name) 
{ 
};

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