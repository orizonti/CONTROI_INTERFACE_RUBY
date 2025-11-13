#include <QSpinBox>
#include "./ui_widget_lid_control.h"
#include "widget_lid_control.h"
#include <QTimer>

WidgetLidControl::WidgetLidControl(QWidget *parent)
    : WidgetAdjustable(parent)
    , ui(new Ui::WidgetLidControl)
{
    ui->setupUi(this);
}

WidgetLidControl::~WidgetLidControl() 
{ 
    delete ui; 
}

void WidgetLidControl::linkTo(std::shared_ptr<DeviceLidInterface> Lid1, std::shared_ptr<DeviceLidInterface> Lid2)
{
    QObject::connect(ui->butLeftLid, &QPushButton::toggled, [Lid1](bool OnOff){ if(OnOff) Lid1->openLid(); else Lid1->closeLid(); });
    QObject::connect(ui->butRightLid, &QPushButton::toggled, [Lid2](bool OnOff){ if(OnOff) Lid2->openLid(); else Lid2->closeLid(); });

}
void WidgetLidControl::linkTo(DeviceLidInterface& Lid1, DeviceLidInterface& Lid2)
{
    auto LidLeft = &Lid1;
    auto LidRight = &Lid2;
    QObject::connect(ui->butLeftLid, &QPushButton::toggled, [LidLeft](bool OnOff){ if(OnOff) LidLeft->openLid(); else LidLeft->closeLid(); });
    QObject::connect(ui->butRightLid, &QPushButton::toggled, [LidRight](bool OnOff){ if(OnOff) LidRight->openLid(); else LidRight->closeLid(); });
}
