#pragma once
#include <QWidget>
#include <QDebug>
#include <QGraphicsEffect>
#include <QPainter>
#include "widget_adjustable.h"

#include "device_focus_ranger_interface.h"


QT_BEGIN_NAMESPACE
namespace Ui { class WidgetFocusRangerControl; }
QT_END_NAMESPACE

class WidgetFocusRangerControl : public WidgetAdjustable
{
    Q_OBJECT
public:
    WidgetFocusRangerControl(QWidget* parent = nullptr);
    ~WidgetFocusRangerControl();


   void linkTo(DeviceFocusGenericInterface* FocusControl);
   int Position = 0;
public slots:

private:
    Ui::WidgetFocusRangerControl *ui;
};
