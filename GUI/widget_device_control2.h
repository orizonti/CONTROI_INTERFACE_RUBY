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
#include <QBoxLayout>
#include "interface_node_signal_adapter.h"


class WidgetDeviceControl2 : public WidgetAdjustable, public PassCoordClass<float>
{
    Q_OBJECT
public:
    WidgetDeviceControl2(QString name = "Устройство", Qt::Orientation orientation = Qt::Horizontal, QWidget* parent = nullptr);
    ~WidgetDeviceControl2();

    Qt::Orientation orientationWidget = Qt::Horizontal;
    //================================================================
    std::pair<float,float> State;
	const QPair<float, float>& getOutput() override { return State;};
	void setInput(const QPair<float, float>& Coord) override 
    { 
        if(labelsState.isEmpty()) return;
        State = Coord; labelsState[0]->setText(QString("%1 %2").arg(State.first).arg(State.second)); 
    };
    NodeCoordSignalAdapter NodeSynchronizer{this};
    void linkToWidget(WidgetDeviceControl2* widget) { NodeSynchronizer.linkPeers(&widget->NodeSynchronizer);};
    //================================================================

    void setScheme(int numberStates, int numberParams, int numberLevels, int numberDevice, int schemeArrows = 0);

    void setButtonsName(QVector<QString> names);
    void setLevelsName(QVector<QString> names);
    void setName(QString name);
    void setSizes();

    void linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device);
    std::shared_ptr<DeviceGenericHandleControl> DeviceLinked = nullptr;
    void linkSignals();

    public:
    QTimer timerCheckDevice;

    QLabel* labelName  = nullptr;

    QVector<QLabel*>      labelsState;
    QVector<QSpinBox*>    buttonsParam;
    QVector<QPushButton*> buttonsDevice;
    QVector<QPushButton*> buttonsLevel;
    QVector<QPushButton*> buttonsArrow;

    QGroupBox* groupLabelsState = nullptr;
    QGroupBox* groupButtonsParam = nullptr;
    QGroupBox* groupButtonsLevel = nullptr;
    QGroupBox* groupButtonsDevice = nullptr;
    QGroupBox* groupArrows = nullptr;

    int schemeArrowsControl = 0;
    QSize maxButtonsSize{50,50};
    QSize minButtonsSize{30,30};

    QSize maxLabelsSize{70,50};
    QSize minLabelsSize{70,30};

    QBoxLayout* mainLayout = nullptr;
};
