#pragma once
#include <QWidget>
#include <QDebug>
#include <QGraphicsEffect>
#include <QPainter>
#include "widget_adjustable.h"


QT_BEGIN_NAMESPACE
namespace Ui { class WidgetLaserControl; }
QT_END_NAMESPACE

class DeviceLaserGenericInterface;

class WidgetLaserControl : public WidgetAdjustable
{
    Q_OBJECT
public:
    WidgetLaserControl(QWidget* parent = nullptr);
    ~WidgetLaserControl();
    void linkTo(DeviceLaserGenericInterface* LaserControl);
    void setState(int State);
    void setParam(int ID, int Param);

public slots:

private:
    Ui::WidgetLaserControl *ui;
};
