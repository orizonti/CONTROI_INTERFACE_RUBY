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
#include "widget_main_control.h"
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
    std::vector<WidgetDeviceControl*> widgetsHidden;
    std::vector<QPushButton*> buttonsHidden;

    WidgetMainControl* widgetMainControl1   = nullptr;
    WidgetMainControl* widgetMainControl2   = nullptr;

    WidgetDeviceControl* widgetControlLaserPower = nullptr;
    WidgetDeviceControl* widgetControlLaserIllum = nullptr;

    WidgetDeviceControl* widgetControlScanator = nullptr;
    WidgetDeviceControl* widgetControlPlatform = nullptr;

    WidgetDeviceControl* widgetControlRanger    = nullptr;
    WidgetDeviceControl* widgetControlFocusator = nullptr;

    WidgetDeviceControl* widgetLidControl    = nullptr;

    QPushButton* buttonSwitcher   = nullptr;
    QPushButton* buttonSwitcher2  = nullptr;

    WidgetDeviceControl* widgetControlCamera1Float    = nullptr;
    WidgetDeviceControl* widgetControlCamera2Float    = nullptr;
    WidgetDeviceControl* widgetControlCamera3Float    = nullptr;

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

void installEventFilter(KeyboardFilter *filterObj);

public slots:
    void slotSetMainMode();
    void slotSetHandleMode();
    void slotSetBigImageMode();
    void slotShowMalfunctionList();
    void slotShowHiddenWidgets();
    void slotHideHiddenWidgets();
signals:
void signalEndWork();
private:
    Ui::WidgetComplexInterface *ui;
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void activateMainOutput(bool OnOff);
    void activateControlOutput(bool OnOff);
    void activateBigOutput(bool OnOff);

private slots:
    void slotEndWork(QCloseEvent *event);
};
