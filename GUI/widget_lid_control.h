#pragma once
#include <QWidget>
#include <QDebug>
#include <QGraphicsEffect>
#include <QPainter>
#include "widget_adjustable.h"
#include "device_generic_interface.h"


QT_BEGIN_NAMESPACE
namespace Ui { class WidgetLidControl; }
QT_END_NAMESPACE

class WidgetLidControl : public WidgetAdjustable
{
    Q_OBJECT
public:
    WidgetLidControl(QWidget* parent = nullptr);
    ~WidgetLidControl();
    void linkTo(std::shared_ptr<DeviceLidInterface> Lid1, std::shared_ptr<DeviceLidInterface> Lid2);
    void linkTo(DeviceLidInterface& Lid1, DeviceLidInterface& Lid2);

public slots:

private:
    Ui::WidgetLidControl *ui;
};
