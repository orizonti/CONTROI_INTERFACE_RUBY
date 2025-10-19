#pragma once
#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QGraphicsEffect>
#include "widget_adjustable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WidgetBlockStatus; }
QT_END_NAMESPACE

class WidgetComplexInterface;

class WidgetBlockStatus : public WidgetAdjustable
{
    Q_OBJECT
public:
    WidgetBlockStatus(QWidget* parent = nullptr);
    ~WidgetBlockStatus();

    void linkSignals(WidgetComplexInterface* widgetControl);
    void hideButtons(std::vector<int> Numbers);
    void hideButton(int numberButton);

    std::vector<QPushButton*> Buttons;
public slots:


private:
    Ui::WidgetBlockStatus *ui;
};
