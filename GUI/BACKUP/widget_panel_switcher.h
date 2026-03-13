#pragma once
#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QGraphicsEffect>
#include "widget_adjustable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WidgetPanelSwitcher; }
QT_END_NAMESPACE

class WidgetComplexInterface;

class WidgetPanelSwitcher : public WidgetAdjustable
{
    Q_OBJECT
public:
    WidgetPanelSwitcher(QWidget* parent = nullptr);
    ~WidgetPanelSwitcher();

    void linkSignals(WidgetComplexInterface* widgetControl);
    void hideButtons(std::vector<int> Numbers);
    void hideButton(int numberButton);

    std::vector<QPushButton*> Buttons;
public slots:

signals:
void signalRegim1();
void signalRegim2();
void signalRegim3();

private:
    Ui::WidgetPanelSwitcher *ui;
};
