#pragma once
#include <QWidget>
#include <QDebug>
#include <QGraphicsEffect>
#include <QPainter>
#include "widget_adjustable.h"
#include <QTimer>

#include "device_generic_interface.h"
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QPushButton>


class WidgetDeviceControl : public WidgetAdjustable
{
    Q_OBJECT
public:
    WidgetDeviceControl(QString name = "Устройство", QWidget* parent = nullptr);
    ~WidgetDeviceControl();

    void enableScheme(bool enableState, bool enableParam, bool enableLevels, bool enableOnOff, bool enableArrows);
    void setScheme(int schemeParam, int numberLevels, int numberDevice, int schemeArrows = 0);
    void setButtonsName(QVector<QString> names);
    void setLevelsName(QVector<QString> names);
    void setName(QString name);

    void linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device);

    std::shared_ptr<DeviceGenericHandleControl> DeviceLinked = nullptr;
    void linkSignals();
    QTimer timerCheckDevice;

      QLabel* labelName  = nullptr;
      QLabel* labelState = nullptr;
    QSpinBox* spinParam  = nullptr;

    QPushButton* buttonLeft  = nullptr;
    QPushButton* buttonRight = nullptr;
    QPushButton* buttonUp    = nullptr;
    QPushButton* buttonDown  = nullptr;

    QVector<QPushButton*> buttonsLevel;
    QVector<QPushButton*> buttonsOnOff;
    QVector<QPushButton*> buttonsArrow;

    QGroupBox* groupButtonsLevel = nullptr;
    QGroupBox* groupButtonsOnOff = nullptr;
    QGroupBox* groupArrows = nullptr;

    int schemeArrowsControl = 0;
};
