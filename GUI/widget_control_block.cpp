#include <QSpinBox>
#include "./ui_widget_control_block.h"
#include "widget_control_block.h"
#include <QTimer>
#include "widget_complex_interface.h"

WidgetControlBlock::WidgetControlBlock(QWidget *parent)
    : WidgetAdjustable(parent)
    , ui(new Ui::WidgetControlBlock)
{
    ui->setupUi(this);
    ui->butDeviceMalfunction->hide();
}

WidgetControlBlock::~WidgetControlBlock() { delete ui; }

void WidgetControlBlock::LinkSignals(WidgetComplexInterface* widgetControl)
{
   QObject::connect(ui->butOpenHandleControlPanel, SIGNAL(clicked()), widgetControl, SLOT(slotSetHandleMode()));
   QObject::connect(ui->butDeviceMalfunction, SIGNAL(clicked()), widgetControl, SLOT(slotSHowMalfunctionList()));
   QObject::connect(ui->butOpenSettingsPanel, SIGNAL(clicked()), widgetControl, SLOT(slotSetControlPanelMode()));
}
