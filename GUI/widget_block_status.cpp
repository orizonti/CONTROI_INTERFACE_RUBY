#include <QSpinBox>
#include "./ui_widget_block_status.h"
#include "widget_control_block.h"
#include <QTimer>
#include "widget_block_status.h"

WidgetBlockStatus::WidgetBlockStatus(QWidget *parent)
    : WidgetAdjustable(parent)
    , ui(new Ui::WidgetBlockStatus)
{
    ui->setupUi(this);

    //Buttons.push_back(ui->butSetMainMode);
    //Buttons.push_back(ui->butOpenHandleControlPanel);
    //Buttons.push_back(ui->butOpenSettingsPanel);

    //connect(Buttons[0],&QPushButton::clicked, [this]() { emit signalRegim1(); });
    //connect(Buttons[1],&QPushButton::clicked, [this]() { emit signalRegim2(); });
    //connect(Buttons[2],&QPushButton::clicked, [this]() { emit signalRegim3(); });
}

void WidgetBlockStatus::hideButtons(std::vector<int> Numbers)
{
  //for(auto number: Numbers) if(Buttons.size() > number) Buttons[number]->hide();
}

void WidgetBlockStatus::hideButton(int numberButton)
{
}

WidgetBlockStatus::~WidgetBlockStatus() { delete ui; }

void WidgetBlockStatus::linkSignals(WidgetComplexInterface* widgetControl)
{
   //QObject::connect(ui->butDeviceMalfunction, SIGNAL(clicked()), widgetControl, SLOT(slotSHowMalfunctionList()));
   //QObject::connect(ui->butOpenSettingsPanel, SIGNAL(clicked()), widgetControl, SLOT(slotSetControlPanelMode()));
}
