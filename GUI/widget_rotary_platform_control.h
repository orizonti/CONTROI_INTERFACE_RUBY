#pragma once

#include <QWidget>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QLine>
#include "widget_adjustable.h"
#include "device_generic_interface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WidgetRotaryPlatformControl; }
QT_END_NAMESPACE


class WidgetRotaryPlatformControl : public WidgetAdjustable
{
    Q_OBJECT
public:
    explicit WidgetRotaryPlatformControl(QWidget* parent = 0);
        void linkToDevice(DeviceRotaryGenericInterface* Device);

        QPair<int,int> Position{0,0};
private:
Ui::WidgetRotaryPlatformControl *ui;

public slots:
void slotDevicePosChanged(int pos1, int pos2);

private slots:
void slotValue1Changed(int value) { Position.first  = value; };
void slotValue2Changed(int value) { Position.second = value; };

signals:
};
