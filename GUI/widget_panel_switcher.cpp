#include <QSpinBox>
#include "./ui_widget_panel_switcher.h"
#include "widget_control_block.h"
#include <QTimer>
#include "widget_panel_switcher.h"

WidgetPanelSwitcher::WidgetPanelSwitcher(QWidget *parent)
    : WidgetAdjustable(parent)
    , ui(new Ui::WidgetPanelSwitcher)
{
    ui->setupUi(this);

    Buttons.push_back(ui->butSetMainMode);
    Buttons.push_back(ui->butOpenHandleControlPanel);
    Buttons.push_back(ui->butOpenSettingsPanel);

    connect(Buttons[0],&QPushButton::clicked, [this]() { emit signalRegim1(); });
    connect(Buttons[1],&QPushButton::clicked, [this]() { emit signalRegim2(); });
    connect(Buttons[2],&QPushButton::clicked, [this]() { emit signalRegim3(); });
}

void WidgetPanelSwitcher::hideButtons(std::vector<int> Numbers)
{
  for(auto number: Numbers) if(Buttons.size() > number) Buttons[number]->hide();
}

void WidgetPanelSwitcher::hideButton(int numberButton)
{
   Buttons[numberButton]->hide();
}

WidgetPanelSwitcher::~WidgetPanelSwitcher() { delete ui; }

void WidgetPanelSwitcher::linkSignals(WidgetComplexInterface* widgetControl)
{
   //QObject::connect(ui->butOpenHandleControlPanel, SIGNAL(clicked()), widgetControl, SLOT(slotSetHandleMode()));
   //QObject::connect(ui->butDeviceMalfunction, SIGNAL(clicked()), widgetControl, SLOT(slotSHowMalfunctionList()));
   //QObject::connect(ui->butOpenSettingsPanel, SIGNAL(clicked()), widgetControl, SLOT(slotSetControlPanelMode()));
}
