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
        void linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device, int Number);
        void linkToDeviceRotary(std::shared_ptr<DeviceGenericHandleControl> Device);
        void linkButtons();
        void synchronizePeer(WidgetMainControl* widget);

        QPair<int,int> Position{0,0};
void setName(QString name);

WidgetMainControl* widgetPeer = nullptr;

WidgetRotaryControl* widgetAzimuth = nullptr;
WidgetRotaryControl* widgetElevation = nullptr;

QPushButton butReset{"Сброс"};
QPushButton butRegimReady{"Готовность"};
QPushButton butRegimWork {"Работа"} ;
QPushButton butRegimLaser{"Огонь"};

QPushButton butControl1 {"Ручной"};
QPushButton butControl2 {"Контроль"};

QHBoxLayout mainLayout;
QVBoxLayout rightLayout;
QGridLayout gridLayout;

QVBoxLayout mainLayout2;

private:

QTimer timerCheckState;

public slots:
void slotPeerChanged();
void slotResetPressed() 
{ 
    qDebug() << "[MAIN CONTROl]" << "[ SLOT RESET ]";
    emit signalReset(); 
};

signals:
void signalReset();
};
