#include <QSpinBox>
#include "./ui_widget_focus_ranger_control.h"
#include "widget_focus_ranger_control.h"
#include <QTimer>

WidgetFocusRangerControl::WidgetFocusRangerControl(QWidget *parent)
    : WidgetAdjustable(parent)
    , ui(new Ui::WidgetFocusRangerControl)
{
    ui->setupUi(this);

    //QGraphicsBlurEffect * glow1 = new QGraphicsBlurEffect();
    //QGraphicsBlurEffect * glow2 = new QGraphicsBlurEffect();
    //QGraphicsBlurEffect * glow3 = new QGraphicsBlurEffect();
    ////glow->setStrength(4);
    //glow1->setBlurRadius(3.2);
    //glow2->setBlurRadius(3.2);
    //glow3->setBlurRadius(1.2);

    //ui->label->setGraphicsEffect(glow1);
    //ui->label_2->setGraphicsEffect(glow2);
    //ui->label_3->setGraphicsEffect(glow3);
}

WidgetFocusRangerControl::~WidgetFocusRangerControl() 
{ 
    delete ui; 
}


void WidgetFocusRangerControl::linkTo(DeviceFocusGenericInterface* FocusControl)
{
    //ui->labelLaserPowerStatus->setText(LaserControl->getName());
}