#include <QSpinBox>
#include "./ui_widget_device_control.h"
#include "widget_device_control.h"
#include <QTimer>

QString styleToggledButtons{
"QPushButton{"
"background-color: rgba(204, 53, 10, 62);"
"border: 2px solid line;"
"border-radius: 6px;"	
"border-color: rgb(215, 50, 28); }"

"QPushButton::checked{"
"background-color: rgba(32, 156, 19, 83);"
"border: 2px solid line;"
"border-radius: 6px;"
"border-color: rgb(30, 221, 78);}"

};

QString styleArrowButtons{

"QPushButton {"
"background-color: rgba(209, 142, 34, 60);"
"border: 2px solid line;"
"border-radius: 6px;"
"border-color: rgb(214, 136, 41); }"

"QPushButton:pressed {"
"background-color: rgba(209, 142, 34, 110);"
"border: 4px solid line;"
"border-radius: 6px;"
"order-color: rgb(171, 86, 38); }"
};


QString styleBaseWidget
{
"QWidget { background-color: rgb(43, 43, 43); }"

"QPushButton { background-color: rgba(32, 156, 19, 83);"
"border: 2px solid line;"
"border-radius: 6px;"
"border-color: rgb(30, 221, 78); }"

"QPushButton:checked { background-color: rgba(204, 53, 10, 62);"
"border: 2px solid line;"
"border-radius: 6px;	"
"border-color: rgb(215, 50, 28); }"

"QLineEdit { background-color: rgba(32, 156, 19, 83);"
"border: 2px solid line;"
"border-radius: 6px;"
"border-color: rgb(30, 221, 78); }"

"QLabel { background-color: rgba(209, 142, 34, 60);"
"border: 2px solid line;"
"border-radius: 6px;"
"border-color: rgb(214, 136, 41); }"

"QGroupBox { border: 0px solid line black; }"

"QLabel { background-color: rgba(209, 142, 34, 60);"
"border: 2px solid line;"
"border-radius: 6px;"
"border-color: rgb(214, 136, 41); }"


"QSpinBox { background-color: rgba(209, 142, 34, 60);"
"   border-radius: 6px;"
"   border: 2px solid line;"
"   border-color: rgb(214, 136, 41);"
"   padding-right: 0px; }"

"QSpinBox::up-button { subcontrol-origin: border;"
"    subcontrol-position: top right; "
"    width: 28px; "
"    border-width: 1px; }"


"QSpinBox::down-button {"
"subcontrol-origin: border;"
"subcontrol-position: bottom right; "
"width: 28px; "
"border-width: 1px;}"

"QSpinBox::up-button:hover { }"
"QSpinBox::up-button:pressed { }"

"QSpinBox::down-arrow { image: url(D:/PICTURES/ImagesToPrograms/arrow_down.png);"
"width: 20px;"
"height: 40px; }"

"QSpinBox::up-arrow { image: url(D:/PICTURES/ImagesToPrograms/arrow_up.png);"
"width: 20px;"
"height: 40px; }"

"QSpinBox::up-arrow:disabled, QSpinBox::up-arrow:off { image: url(:/images/up_arrow_disabled.png); }"
"QSpinBox::down-button:hover { border-image: url(:/images/spindown_hover.png) 1; }"
"QSpinBox::down-button:pressed { border-image: url(:/images/spindown_pressed.png) 1; }"
};



WidgetDeviceControl::WidgetDeviceControl(QString name, QWidget *parent) : WidgetAdjustable(parent)
{
    this->setStyleSheet(styleBaseWidget);
    this->setMinimumWidth(400);

    labelName  = new QLabel(name); 
    labelState = new QLabel("0000.00\n0000.00"); 
    spinParam = new QSpinBox();
    labelName->setMaximumSize(100,50);
    labelState->setMaximumSize(100,50);
    spinParam->setMaximumSize(100,50);

    groupButtonsLevel = new QGroupBox;
    groupButtonsOnOff = new QGroupBox;
          groupArrows = new QGroupBox;

    labelName->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    labelState->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    spinParam->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    
    groupButtonsLevel->setLayout(new QHBoxLayout); groupButtonsLevel->setStyleSheet(styleToggledButtons); 
    groupButtonsOnOff->setLayout(new QHBoxLayout); groupButtonsOnOff->setStyleSheet(styleToggledButtons);
          groupArrows->setLayout(new QHBoxLayout);       groupArrows->setStyleSheet(styleArrowButtons);
    groupButtonsLevel->layout()->setSpacing(3); groupButtonsLevel->layout()->setContentsMargins(1,1,1,1);
    groupButtonsOnOff->layout()->setSpacing(3); groupButtonsOnOff->layout()->setContentsMargins(1,1,1,1);
          groupArrows->layout()->setSpacing(3);       groupArrows->layout()->setContentsMargins(1,1,1,1);

    groupButtonsLevel->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    groupButtonsOnOff->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    groupArrows->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

    this->setLayout(new QHBoxLayout);
    this->layout()->addWidget(labelName);
    this->layout()->addWidget(labelState);
    this->layout()->addWidget(spinParam);
    this->layout()->addWidget(groupButtonsLevel);
    this->layout()->addWidget(groupButtonsOnOff);
    this->layout()->addWidget(groupArrows);

    this->layout()->setSpacing(3); this->layout()->setContentsMargins(1,1,1,1);
}


void WidgetDeviceControl::enableScheme(bool enableState, 
                                       bool enableParam, 
                                       bool enableLevels, 
                                       bool enableOnOff, 
                                       bool enableArrows)
{
    if(!enableState)  labelState->hide();
    if(!enableParam)  spinParam->hide();
    if(!enableLevels) groupButtonsLevel->hide();
    if(!enableOnOff)  groupButtonsOnOff->hide();
    if(!enableArrows) groupArrows->hide();

}
void WidgetDeviceControl::setScheme(int schemeParam, int numberLevels, int numberDevice, int schemeArrows) {
                           labelState->show(); spinParam->hide();
    if(schemeParam == 1) { labelState->hide(); spinParam->show(); }
    if(schemeParam == 2) { labelState->show(); spinParam->show(); }

    for(int n = 0; n < numberLevels; n++)
    {
      buttonsLevel.push_back(new QPushButton(QString("X%1").arg(n))); groupButtonsLevel->layout()->addWidget(buttonsLevel.last());
      buttonsLevel.last()->setMinimumWidth(30); buttonsLevel.last()->setMaximumSize(50,50);
      buttonsLevel.last()->setAutoExclusive(true);
      buttonsLevel.last()->setCheckable(true); 
    }
      buttonsLevel.first()->setChecked(true);

    for(int n = 0; n < numberDevice; n++)
    {
      buttonsOnOff.push_back(new QPushButton(QString("DEV_%1").arg(n))); groupButtonsOnOff->layout()->addWidget(buttonsOnOff.last());
      buttonsOnOff.last()->setMinimumWidth(30); buttonsOnOff.last()->setMaximumSize(50,50);
      buttonsOnOff.last()->setCheckable(true); 
    }

    if(schemeArrows == 4)
    {
    buttonLeft  = new QPushButton;
    buttonRight = new QPushButton;
    buttonUp    = new QPushButton;
    buttonDown  = new QPushButton;

     buttonLeft->setMinimumWidth(30);  buttonLeft->setMaximumSize(50,50);
    buttonRight->setMinimumWidth(30); buttonRight->setMaximumSize(50,50);
       buttonUp->setMinimumWidth(30);    buttonUp->setMaximumSize(50,50);
     buttonDown->setMinimumWidth(30);  buttonDown->setMaximumSize(50,50);

    buttonLeft->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    buttonRight->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    buttonUp->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    buttonDown->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

    groupArrows->layout()->addWidget(buttonLeft);
    groupArrows->layout()->addWidget(buttonRight);
    groupArrows->layout()->addWidget(buttonUp);
    groupArrows->layout()->addWidget(buttonDown);
    }


    if(schemeArrows == 2)
    {
    buttonLeft  = new QPushButton;
    buttonRight = new QPushButton;

    groupArrows->layout()->addWidget(buttonLeft);
    groupArrows->layout()->addWidget(buttonRight);
    }
    this->schemeArrowsControl = schemeArrows;

}

void WidgetDeviceControl::setButtonsName(QVector<QString> names)
{

    auto name = names.begin();
    for(auto button: buttonsOnOff)
    {
       button->setText(*name); name++; if(name == names.end()) break;  
    }

}
void WidgetDeviceControl::setLevelsName(QVector<QString> names)
{
    auto name = names.begin();
    for(auto button: buttonsLevel)
    {
       button->setText(*name); name++; if(name == names.end()) break;  
    }
}

void WidgetDeviceControl::setName(QString name) { labelName->setText(name); }

void WidgetDeviceControl::linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device)
{
   DeviceLinked = Device; 
               if(Device) linkSignals();
}

void WidgetDeviceControl::linkSignals()
{
    QVector<QPushButton*> buttonsLevel;

    int level = 1;
    for(auto button: buttonsLevel)
    {
      connect(button, &QPushButton::toggled, [level, this](bool OnOff) { if(OnOff) DeviceLinked->setLevel(level); } ); level++;
    }

    int device = 1;
    for(auto button: buttonsOnOff)
    {
      connect(button, &QPushButton::toggled, [device, this](bool OnOff) { DeviceLinked->setEnable(OnOff,device); } ); device++;
    }

      connect(spinParam, &QSpinBox::valueChanged, [this](int Value) { DeviceLinked->setValue(Value); } ); 


    if(!groupArrows->isVisible()) return;

    std::vector<QPair<float,float>> VelsVector;
    float VelocityScale = 0.5;
    if(schemeArrowsControl == 4)
    {
        buttonsArrow.push_back(buttonLeft);  VelsVector.push_back(QPair<float,float>(-VelocityScale, 0));
        buttonsArrow.push_back(buttonRight); VelsVector.push_back(QPair<float,float>( VelocityScale, 0));
        buttonsArrow.push_back(buttonUp);    VelsVector.push_back(QPair<float,float>( 0, VelocityScale));
        buttonsArrow.push_back(buttonDown);  VelsVector.push_back(QPair<float,float>( 0,-VelocityScale));

        for(int n = 0; n < 4; n++)
        {
        auto Velocity = VelsVector[n];
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
            labelState->setText(QString("%1 %2").arg(Position.first).arg(Position.second));
        });
    }


    std::vector<float> VelsScalar;
    if(schemeArrowsControl == 2)
    {
        buttonsArrow.push_back(buttonLeft);  VelsScalar.push_back(VelocityScale );
        buttonsArrow.push_back(buttonRight); VelsScalar.push_back(-VelocityScale);

        auto velocityValue = VelsScalar.begin();
        for(auto but: buttonsArrow)
        {
        QObject::connect(but, &QPushButton::pressed,  [this, velocityValue]() 
        {   
            DeviceLinked->setValue(*velocityValue); timerCheckDevice.start(1);
        });
        QObject::connect(but, &QPushButton::released, [this     ]()      { DeviceLinked->setEnable(false);  timerCheckDevice.stop(); });
        }

    }

                                            std::pair<float,float> Position;
        QObject::connect(&timerCheckDevice, &QTimer::timeout, [this, Position]() mutable
        {
            Position = DeviceLinked->getPair(); 
            labelState->setText(QString("%1 %2").arg(Position.first).arg(Position.second));
        });

}

WidgetDeviceControl::~WidgetDeviceControl() { }

