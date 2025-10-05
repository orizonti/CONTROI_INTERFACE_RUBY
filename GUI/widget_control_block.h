#pragma once
#include <QWidget>
#include <QDebug>
#include <QGraphicsEffect>
#include <QPainter>
#include "widget_adjustable.h"


QT_BEGIN_NAMESPACE
namespace Ui { class WidgetControlBlock; }
QT_END_NAMESPACE

class WidgetComplexInterface;

class WidgetControlBlock : public WidgetAdjustable
{
    Q_OBJECT
public:
    WidgetControlBlock(QWidget* parent = nullptr);
    ~WidgetControlBlock();

    void LinkSignals(WidgetComplexInterface* widgetControl);
public slots:

private:
    Ui::WidgetControlBlock *ui;
};
