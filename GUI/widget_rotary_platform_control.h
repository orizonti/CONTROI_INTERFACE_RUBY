#pragma once

#include <QWidget>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QLine>
#include "widget_adjustable.h"
#include "widget_rotary_control.h"
class DeviceRotaryInterface;

QT_BEGIN_NAMESPACE
namespace Ui { class WidgetRotaryPlatformControl; }
QT_END_NAMESPACE


class WidgetRotaryPlatformControl : public WidgetAdjustable
{
    Q_OBJECT
public:
    explicit WidgetRotaryPlatformControl(int Scheme = 0, QWidget* parent = 0);
        void linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device);

        QPair<int,int> Position{0,0};
void setName(QString name);

WidgetRotaryControl* widgetAzimuth = nullptr;
WidgetRotaryControl* widgetElevation = nullptr;

private:
Ui::WidgetRotaryPlatformControl *ui;

QTimer timerCheckState;

private slots:
void slotValue1Changed(int value) { Position.first  = value; };
void slotValue2Changed(int value) { Position.second = value; };

signals:
};
