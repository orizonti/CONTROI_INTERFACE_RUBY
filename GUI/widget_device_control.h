#pragma once
#include <QWidget>
#include <QDebug>
#include <QGraphicsEffect>
#include <QPainter>
#include "widget_adjustable.h"


QT_BEGIN_NAMESPACE
namespace Ui { class WidgetDeviceControl; }
QT_END_NAMESPACE


class WidgetDeviceControl : public WidgetAdjustable
{
    Q_OBJECT
public:
    WidgetDeviceControl(QString name = "Устройство", QWidget* parent = nullptr);
    ~WidgetDeviceControl();

    void enableScheme(bool blockParam, bool blockLevel, bool blockButtons, bool blockArrows);
    void setScheme(int schemeParam, int schemeLevel, int schemeButtons);
    void setButtonsName(QVector<QString> names);
    void setLevelsName(QVector<QString> names);
    void setName(QString name);

public slots:

private:
    Ui::WidgetDeviceControl *ui;
};
