#include "widget_rotary_platform_control.h"
#include "ui_widget_rotary_platform_control.h"
WidgetRotaryPlatformControl::WidgetRotaryPlatformControl(QWidget* parent) : WidgetAdjustable(parent), ui(new Ui::WidgetRotaryPlatformControl)
{
    qDebug() << " [ CREATE WIDGET CAMERA CONTROL ]";
    ui->setupUi(this);
    QObject::connect(ui->scrollRotaryAxisX, &QScrollBar::valueChanged, this, &WidgetRotaryPlatformControl::slotValue1Changed);
    QObject::connect(ui->scrollRotaryAxisY, &QScrollBar::valueChanged, this, &WidgetRotaryPlatformControl::slotValue2Changed);
}

void WidgetRotaryPlatformControl::linkToDevice(DeviceRotaryGenericInterface* Device)
{

    std::vector<QPushButton*> ArrowButtons;
    std::vector<QPair<int,int>> Vels;
    ArrowButtons.push_back(ui->butRotaryX_Left);  Vels.push_back(QPair<int,int>(-1, 0));
    ArrowButtons.push_back(ui->butRotaryX_Right); Vels.push_back(QPair<int,int>( 1, 0));
    ArrowButtons.push_back(ui->butRotaryY_Up);    Vels.push_back(QPair<int,int>( 0, 1));
    ArrowButtons.push_back(ui->butRotaryY_Down);  Vels.push_back(QPair<int,int>( 0,-1));

    for(int n = 0; n < 4; n++)
    {
    auto Velocity = Vels[n];
    auto button = ArrowButtons[n];
    QObject::connect(button, &QPushButton::pressed,  [Device, Velocity]() { Device->moveWithVelocity(Velocity);});
    QObject::connect(button, &QPushButton::released, [Device     ]() { Device->stopMove(); });
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
}

void WidgetRotaryPlatformControl::slotDevicePosChanged(int pos1, int pos2)
{
 Position.first  = pos1; ui->scrollRotaryAxisX->setValue(pos1*4); 
 Position.second = pos2; ui->scrollRotaryAxisY->setValue(pos2*4);
}
