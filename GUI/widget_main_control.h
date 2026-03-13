#pragma once

#include <QWidget>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QLine>
#include "widget_adjustable.h"
#include "widget_rotary_control.h"
#include <QBoxLayout>
#include <QPushButton>

class DeviceRotaryInterface;

class WidgetMainControl : public WidgetAdjustable
{
    Q_OBJECT
public:
    explicit WidgetMainControl(int Scheme = 0, QWidget* parent = 0);
        void linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device);

        QPair<int,int> Position{0,0};
void setName(QString name);

WidgetRotaryControl* widgetAzimuth = nullptr;
WidgetRotaryControl* widgetElevation = nullptr;

QPushButton butRegimReady{"Готовность"};
QPushButton butRegimWork {"Работа"} ;
QPushButton butRegimLaser{"Огонь"};
QPushButton butRegimIllum{"Подсвет"};
QPushButton butToHandle  {"Панель"};
QPushButton butToBigControl{"Большой"};

QHBoxLayout mainLayout;
QVBoxLayout rightLayout;
QGridLayout gridLayout;

QVBoxLayout mainLayout2;

private:

QTimer timerCheckState;

private slots:
void slotValue1Changed(int value) { Position.first  = value; };
void slotValue2Changed(int value) { Position.second = value; };

signals:
};
