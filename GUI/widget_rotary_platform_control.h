#pragma once

#include <QWidget>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QLine>
#include "widget_adjustable.h"
//#include "CAMERA_INTERFACE/camera_interface_class.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WidgetRotaryPlatformControl; }
QT_END_NAMESPACE


class WidgetRotaryPlatformControl : public WidgetAdjustable
{
    Q_OBJECT
public:
    explicit WidgetRotaryPlatformControl(QWidget* parent = 0);
    //void LinkToDevice(std::shared_ptr<CameraControlInterface> CameraInterface);

private:
Ui::WidgetRotaryPlatformControl *ui;

signals:
void SignalslotStartStream(bool);
};
