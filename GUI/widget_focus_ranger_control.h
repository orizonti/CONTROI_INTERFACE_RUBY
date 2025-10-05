#pragma once
#include <QWidget>
#include <QDebug>
#include <QGraphicsEffect>
#include <QPainter>
#include "widget_adjustable.h"


QT_BEGIN_NAMESPACE
namespace Ui { class WidgetFocusRangerControl; }
QT_END_NAMESPACE

class WidgetFocusRangerControl : public WidgetAdjustable
{
    Q_OBJECT
public:
    WidgetFocusRangerControl(QWidget* parent = nullptr);
    ~WidgetFocusRangerControl();

public slots:

private:
    Ui::WidgetFocusRangerControl *ui;
};
