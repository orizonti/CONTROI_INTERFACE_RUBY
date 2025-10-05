#pragma once
#include <QWidget>
#include <QDebug>
#include <QGraphicsEffect>
#include <QPainter>
#include "widget_adjustable.h"


QT_BEGIN_NAMESPACE
namespace Ui { class WidgetLaserControl; }
QT_END_NAMESPACE

class WidgetLaserControl : public WidgetAdjustable
{
    Q_OBJECT
public:
    WidgetLaserControl(QWidget* parent = nullptr);
    ~WidgetLaserControl();

public slots:

private:
    Ui::WidgetLaserControl *ui;
};
