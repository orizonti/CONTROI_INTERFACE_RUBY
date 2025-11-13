#include "widget_rotary_platform_control.h"
#include "ui_widget_rotary_platform_control.h"
WidgetRotaryPlatformControl::WidgetRotaryPlatformControl(QWidget* parent) : WidgetAdjustable(parent), ui(new Ui::WidgetRotaryPlatformControl)
{
    qDebug() << " [ CREATE WIDGET CAMERA CONTROL ]";
    ui->setupUi(this);
    QObject::connect(ui->scrollRotaryAxisX, &QScrollBar::valueChanged, this, &WidgetRotaryPlatformControl::slotValue1Changed);
    QObject::connect(ui->scrollRotaryAxisY, &QScrollBar::valueChanged, this, &WidgetRotaryPlatformControl::slotValue2Changed);
}

void WidgetRotaryPlatformControl::linkToDevice(std::shared_ptr<DeviceRotaryInterface> Device)
{

    int Velocity = 10;
    std::vector<QPushButton*> ArrowButtons;
    std::vector<QPair<int,int>> Vels;
    ArrowButtons.push_back(ui->butRotaryX_Left);  Vels.push_back(QPair<int,int>(-Velocity, 0));
    ArrowButtons.push_back(ui->butRotaryX_Right); Vels.push_back(QPair<int,int>( Velocity, 0));
    ArrowButtons.push_back(ui->butRotaryY_Up);    Vels.push_back(QPair<int,int>( 0, Velocity));
    ArrowButtons.push_back(ui->butRotaryY_Down);  Vels.push_back(QPair<int,int>( 0,-Velocity));

    for(int n = 0; n < 4; n++)
    {
    auto Velocity = Vels[n];
    auto button = ArrowButtons[n];
    QObject::connect(button, &QPushButton::pressed,  [this, Device, Velocity]() { Device->moveWithVelocityManual(Velocity); timerCheckState.start(1);});
    QObject::connect(button, &QPushButton::released, [this, Device     ]() { Device->stopMove(); timerCheckState.stop();});
    }

    auto limitsMoveX = Device->getLimits(0);
    auto limitsMoveY = Device->getLimits(0);
    auto rangeMove = Device->getRange();

    ui->scrollRotaryAxisX->setRange(limitsMoveX.first, limitsMoveX.second);
    ui->scrollRotaryAxisX->setValue(limitsMoveX.first + rangeMove.first/2);
    ui->scrollRotaryAxisX->setPageStep(20);

    ui->scrollRotaryAxisY->setRange(limitsMoveY.first, limitsMoveY.second);
    ui->scrollRotaryAxisY->setValue(limitsMoveY.first + rangeMove.second/2);
    ui->scrollRotaryAxisY->setPageStep(20);

    std::vector<QScrollBar*> scrollWidgets;
                             scrollWidgets.push_back(ui->scrollRotaryAxisX);
                             scrollWidgets.push_back(ui->scrollRotaryAxisY);

    for(auto scroll_axis: scrollWidgets)
    {
    QObject::connect(scroll_axis, &QScrollBar::sliderReleased, [Device,this]() 
    { 
        Device->moveToPos(this->Position); 
    });
    }

    QObject::connect(&timerCheckState, &QTimer::timeout, [Device,this]()
    {
        Position = Device->getPos(); ui->scrollRotaryAxisX->setValue(Position.first); 
                                     ui->scrollRotaryAxisY->setValue(Position.second);
    });
}

