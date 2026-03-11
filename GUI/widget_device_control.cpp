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

void WidgetDeviceControl::linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device)
{
   DeviceLinked = Device; 
               if(Device) linkSignals();
}

void WidgetDeviceControl::linkSignals()
{
    QVector<QPushButton*> buttonsLevel;

    buttonsLevel.append(ui->butLevel1);
    buttonsLevel.append(ui->butLevel2);
    buttonsLevel.append(ui->butLevel3);
    buttonsLevel.append(ui->butLevel4);
    buttonsLevel.append(ui->butLevel5);

    int level = 1;
    for(auto button: buttonsLevel)
    {
      connect(button, &QPushButton::toggled, [level, this](bool OnOff) { if(OnOff) DeviceLinked->setLevel(level); } ); level++;
    }

      connect(ui->butOnOff1, &QPushButton::toggled, [this](bool OnOff) { DeviceLinked->setEnable(OnOff,1); } ); 
      connect(ui->butOnOff2, &QPushButton::toggled, [this](bool OnOff) { DeviceLinked->setEnable(OnOff,2); } ); 

      connect(ui->spinParam, &QSpinBox::valueChanged, [this](int Value) { DeviceLinked->setValue(Value); } ); 

    float VelocityScale = 0.5;
    //float VelocityScale = 2;

    std::vector<QPushButton*> buttonsArrow;
    std::vector<QPair<float,float>> Vels;

    //if(!ui->groupArrows->isVisible()) return;

        buttonsArrow.push_back(ui->butMoveLeft);  Vels.push_back(QPair<float,float>(-VelocityScale, 0));
        buttonsArrow.push_back(ui->butMoveRight); Vels.push_back(QPair<float,float>( VelocityScale, 0));
        buttonsArrow.push_back(ui->butMoveUp);    Vels.push_back(QPair<float,float>( 0, VelocityScale));
        buttonsArrow.push_back(ui->butMoveDown);  Vels.push_back(QPair<float,float>( 0,-VelocityScale));

        for(int n = 0; n < 4; n++)
        {
        auto Velocity = Vels[n];
        auto button = buttonsArrow[n];
        QObject::connect(button, &QPushButton::pressed,  [this, Velocity]() 
        {   
            DeviceLinked->setPair(Velocity); timerCheckDevice.start(1);
        });
        QObject::connect(button, &QPushButton::released, [this     ]()      { DeviceLinked->setEnable(false);  timerCheckDevice.stop(); });
        }

                                              std::pair<float,float> Position;
        QObject::connect(&timerCheckDevice, &QTimer::timeout, [this, Position]() mutable
        {
            Position = DeviceLinked->getPair(); 
            ui->labelParam->setText(QString("%1 %2").arg(Position.first).arg(Position.second));
        });

}

WidgetDeviceControl::~WidgetDeviceControl() { delete ui; }

