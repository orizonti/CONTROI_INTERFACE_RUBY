#include <QSpinBox>
#include "./ui_widget_device_control.h"
#include "widget_device_control.h"
#include <QTimer>

WidgetDeviceControl::WidgetDeviceControl(QString name, QWidget *parent)
    : WidgetAdjustable(parent)
    , ui(new Ui::WidgetDeviceControl)
{
    ui->setupUi(this);
    this->setName(name);
}


void WidgetDeviceControl::enableScheme(bool blockParam, bool blockLevel, bool blockButtons, bool blockArrows)
{
    if(!blockParam)   ui->groupParam->hide();
    if(!blockLevel)   ui->groupLevel->hide();
    if(!blockButtons) ui->groupButtons->hide();
    if(!blockArrows)  ui->groupArrows->hide();

}
void WidgetDeviceControl::setScheme(int schemeParam, int schemeLevel, int schemeButtons)
{
                           ui->labelParam->show(); ui->spinParam->hide();
    if(schemeParam == 1) { ui->labelParam->hide(); ui->spinParam->show(); }

    QVector<QPushButton*> levelButtons; 
                          levelButtons.append(ui->butLevel5);
                          levelButtons.append(ui->butLevel4);
                          levelButtons.append(ui->butLevel3);
                          levelButtons.append(ui->butLevel2);
                          levelButtons.append(ui->butLevel1); int level = 5;
                          for(auto but: levelButtons)
                          {
                             if(schemeLevel == level) break; level--; 
                             but->hide(); 
                          }

    if(schemeButtons == 1) ui->butOnOff1->hide(); 

}

void WidgetDeviceControl::setButtonsName(QVector<QString> names)
{
    QVector<QPushButton*> Buttons; 
                          Buttons.append(ui->butOnOff1);
                          Buttons.append(ui->butOnOff2);

    auto name = names.begin();
    for(auto button: Buttons)
    {
       button->setText(*name); name++; if(name == names.end()) break;  
    }

}
void WidgetDeviceControl::setLevelsName(QVector<QString> names)
{
    QVector<QPushButton*> levelButtons; 
                          levelButtons.append(ui->butLevel5);
                          levelButtons.append(ui->butLevel4);
                          levelButtons.append(ui->butLevel3);
                          levelButtons.append(ui->butLevel2);
                          levelButtons.append(ui->butLevel1); int level = 5;

    auto name = names.begin();
    for(auto button: levelButtons)
    {
       button->setText(*name); name++; if(name == names.end()) break;  
    }

}

void WidgetDeviceControl::setName(QString name) { ui->labelName->setText(name); }

WidgetDeviceControl::~WidgetDeviceControl() { delete ui; }

