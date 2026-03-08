#pragma once
#include <QWidget>
#include <QDebug>
#include <QString>
#include <QDoubleSpinBox>
#include <QThread>
#include <QTimer>
#include <thread>
#include <math.h>
#include <QFile>
#include "widget_adjustable.h"
#include <QGraphicsEffect>
#include <QPainter>
#include "widget_control_block.h"
#include "widget_rotary_platform_control.h"
#include "widget_panel_switcher.h"
#include "label_active_image.h"
#include "widget_device_control.h"


QT_BEGIN_NAMESPACE
namespace Ui { class WidgetComplexInterface; }
QT_END_NAMESPACE


class WidgetComplexInterface : public WidgetAdjustable
{
    Q_OBJECT

public:
    WidgetComplexInterface(QWidget* parent = nullptr);
    ~WidgetComplexInterface();

    WidgetControlBlock* ControlBlock = nullptr;
    WidgetRotaryPlatformControl* ControlRotary   = nullptr;
    WidgetRotaryPlatformControl* ControlRotary2   = nullptr;

    WidgetDeviceControl* ControlLaserPower = nullptr;
    WidgetDeviceControl* ControlLaserIllum = nullptr;

    WidgetDeviceControl* ControlScanator = nullptr;
    WidgetDeviceControl* ControlPlatform = nullptr;

    WidgetDeviceControl* ControlRanger    = nullptr;
    WidgetDeviceControl* ControlFocusator = nullptr;

    WidgetDeviceControl* ControlCamera1 = nullptr;
    WidgetDeviceControl* ControlCamera2 = nullptr;
    WidgetDeviceControl* ControlCamera3 = nullptr;
    WidgetDeviceControl* ControlCamera4 = nullptr;

    SinkDisplayLabel* outputVideo1 = nullptr;
    SinkDisplayLabel* outputVideo2 = nullptr;
    SinkDisplayLabel* outputVideo3 = nullptr;

    SinkDisplayLabel* outputVideo1Control = nullptr;
    SinkDisplayLabel* outputVideo2Control = nullptr;
    SinkDisplayLabel* outputVideo3Control = nullptr;

    SinkDisplayLabel* outputVideo1Mini = nullptr;
    SinkDisplayLabel* outputVideo2Mini = nullptr;
    SinkDisplayLabel* outputVideo3Mini = nullptr;

    SinkDisplayLabel* outputVideoBig = nullptr;

public slots:
    void slotSetMainMode();
    void slotSetHandleMode();
    void slotSetBigImageMode();
    void slotSetControlPanelMode();

    void slotShowMalfunctionList();
signals:
void signalEndWork();
private:
    Ui::WidgetComplexInterface *ui;
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void activateMainOutput(bool OnOff);
    void activateControlOutput(bool OnOff);
    void activateBigOutput(bool OnOff);

private slots:
    void slotEndWork(QCloseEvent *event);
};
