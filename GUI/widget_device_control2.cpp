#include <QSpinBox>
#include "widget_device_control2.h"
#include <QTimer>
#include <QSpacerItem>

static QString styleToggledButtons{
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

static QString styleArrowButtons{

"QPushButton {"
"background-color: rgba(209, 142, 34, 60);"
"border: 2px solid line;"
"border-radius: 6px;"
"border-color: rgb(214, 136, 41); }"

"QPushButton:pressed {"
"background-color: rgba(209, 142, 34, 110);"
"border: 4px solid line;"
"border-radius: 6px;"
"border-color: rgb(171, 86, 38); }"
};


static QString styleBaseWidget
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

"QGroupBox { border: 0px solid line; border-color: green; border-radius: 6px; }"

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
};



WidgetDeviceControl2::WidgetDeviceControl2(QString name, Qt::Orientation orientation, QWidget* parent) : WidgetAdjustable(parent)
{
    this->setStyleSheet(styleBaseWidget);
    orientationWidget = orientation;

    QVector<QBoxLayout*> layouts;
    if(orientation == Qt::Horizontal) for(int n = 0; n < 6; n++) layouts.push_back(new QHBoxLayout());
    if(orientation == Qt::Vertical)   for(int n = 0; n < 6; n++) layouts.push_back(new QVBoxLayout());
                                                    mainLayout = layouts[0];

    labelName  = new QLabel(name); 
    labelsState.push_back(new QLabel("0000.00\n0000.00"));

    groupLabelsState = new QGroupBox;
    groupButtonsParam = new QGroupBox;
    groupButtonsLevel = new QGroupBox;
    groupButtonsDevice = new QGroupBox;
          groupArrows = new QGroupBox;

      groupLabelsState->setLayout(layouts[1]);   groupLabelsState->setStyleSheet(styleToggledButtons); 
     groupButtonsParam->setLayout(layouts[2]);  groupButtonsParam->setStyleSheet(styleToggledButtons); 
     groupButtonsLevel->setLayout(layouts[3]);  groupButtonsLevel->setStyleSheet(styleToggledButtons); 
    groupButtonsDevice->setLayout(layouts[4]); groupButtonsDevice->setStyleSheet(styleToggledButtons);
           groupArrows->setLayout(layouts[5]);        groupArrows->setStyleSheet(styleArrowButtons);

    groupLabelsState->layout()->setSpacing(2); groupLabelsState->layout()->setContentsMargins(1,1,1,1);
    groupButtonsParam->layout()->setSpacing(2); groupButtonsParam->layout()->setContentsMargins(1,1,1,1);
    groupButtonsLevel->layout()->setSpacing(2); groupButtonsLevel->layout()->setContentsMargins(1,1,1,1);
    groupButtonsDevice->layout()->setSpacing(2); groupButtonsDevice->layout()->setContentsMargins(1,1,1,1);
           groupArrows->layout()->setSpacing(2);       groupArrows->layout()->setContentsMargins(1,1,1,1);


    this->setLayout(mainLayout);
    this->layout()->addWidget(labelName);
    this->layout()->addWidget(groupLabelsState);
    this->layout()->addWidget(groupButtonsParam);
    this->layout()->addWidget(groupButtonsLevel);
    this->layout()->addWidget(groupButtonsDevice);
    this->layout()->addWidget(groupArrows);

    this->layout()->setSpacing(2); this->layout()->setContentsMargins(1,1,1,1);


    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

void WidgetDeviceControl2::setSizes()
{
    if(orientationWidget == Qt::Horizontal)

    {
                                                          this->setMinimumWidth(2*minLabelsSize.width());
      this->labelName->setMaximumSize(maxLabelsSize); this->labelName->setMinimumWidth(minLabelsSize.width());

      for(auto& but: buttonsParam) { but->setMaximumSize(maxLabelsSize);  but->setMinimumWidth(minLabelsSize.width()); }
      for(auto& but: labelsState)  { but->setMaximumSize(maxLabelsSize);  but->setMinimumWidth(minLabelsSize.width()); }
      for(auto& but: buttonsArrow) { but->setMinimumHeight(minButtonsSize.height());  but->setMaximumSize(maxButtonsSize); }

    groupButtonsLevel->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    groupButtonsDevice->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
          groupArrows->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
            labelName->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

      for(auto& but: buttonsParam)  { but->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));  }
      for(auto& but: labelsState)  { but->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));  }
      for(auto& but: buttonsArrow)  { but->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));  }

    this->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum));

    mainLayout->addSpacerItem(new QSpacerItem(10,20,QSizePolicy::Minimum, QSizePolicy::Fixed));
    }

    if(orientationWidget == Qt::Vertical)
    {
      maxButtonsSize = QSize(60,60);
      minButtonsSize = QSize(30,30);
     this->labelName->setMaximumSize(maxButtonsSize);

      for(auto& but: buttonsParam) { but->setMaximumSize(maxLabelsSize);  but->setMinimumWidth(minLabelsSize.width()); }
      for(auto& but: labelsState)  { but->setMaximumSize(maxLabelsSize);  but->setMinimumWidth(minLabelsSize.width()); }
      for(auto& but: buttonsArrow) { but->setMinimumHeight(minButtonsSize.height());  but->setMaximumSize(maxButtonsSize); }

      groupButtonsLevel->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));

      for(auto& but: buttonsParam) { but->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));  }
      for(auto& but: labelsState)  { but->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));  }
      for(auto& but: buttonsArrow) { but->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));  }


    this->setMinimumSize(minLabelsSize.width()+1, minLabelsSize.height()*2);
    this->setMaximumSize(maxLabelsSize.width()+1, maxLabelsSize.height()*10);
    this->setFixedWidth(55);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum));
    }
}


void WidgetDeviceControl2::setScheme(int numberStates,int numberParams, int numberLevels, int numberDevice, int schemeArrows) 
{

    for(int n = 0; n < numberStates; n++)
    {
      labelsState.push_back(new QLabel("0000.00\n0000.00")); groupButtonsLevel->layout()->addWidget(labelsState.last());
      groupLabelsState->layout()->addWidget(labelsState.last()); 
    }

    for(int n = 0; n < numberParams; n++)
    {
      buttonsParam.push_back(new QSpinBox); groupButtonsParam->layout()->addWidget(buttonsParam.last());
      groupButtonsParam->layout()->addWidget(buttonsParam.last());
    }

    for(int n = 0; n < numberLevels; n++)
    {
      buttonsLevel.push_back(new QPushButton(QString("X%1").arg(n))); groupButtonsLevel->layout()->addWidget(buttonsLevel.last());
      buttonsLevel.last()->setAutoExclusive(true);
      buttonsLevel.last()->setCheckable(true); 

      groupButtonsLevel->layout()->addWidget(buttonsLevel.last());
    }
    if( !buttonsLevel.isEmpty()) buttonsLevel.first()->setChecked(true);

    for(int n = 0; n < numberDevice; n++)
    {
      buttonsDevice.push_back(new QPushButton(QString("DEV_%1").arg(n))); groupButtonsDevice->layout()->addWidget(buttonsDevice.last());
      buttonsDevice.last()->setCheckable(true); 
      groupButtonsDevice->layout()->addWidget(buttonsDevice.last());
    }

    if(schemeArrows == 4)
    {
    groupArrows->layout()->addWidget(new QPushButton(QIcon(":/buttonImages/arrow_left_hollow.png" ),"")); 
    groupArrows->layout()->addWidget(new QPushButton(QIcon(":/buttonImages/arrow_right_hollow.png" ),""));
    groupArrows->layout()->addWidget(new QPushButton(QIcon(":/buttonImages/arrow_up_hollow.png" ),""));   
    groupArrows->layout()->addWidget(new QPushButton(QIcon(":/buttonImages/arrow_down_hollow.png" ),"")); 
    }

    if(schemeArrows == 2)
    {
    groupArrows->layout()->addWidget(new QPushButton(QIcon(":/buttonImages/arrow_left_hollow.png" ),"")); 
    groupArrows->layout()->addWidget(new QPushButton(QIcon(":/buttonImages/arrow_right_hollow.png" ),""));
    }
    this->schemeArrowsControl = schemeArrows;

    this->setSizes();
}

void WidgetDeviceControl2::setButtonsName(QVector<QString> names)
{

    auto name = names.begin();
    for(auto button: buttonsDevice)
    {
       button->setText(*name); name++; if(name == names.end()) break;  
    }

}
void WidgetDeviceControl2::setLevelsName(QVector<QString> names)
{
    auto name = names.begin();
    for(auto button: buttonsLevel)
    {
       button->setText(*name); name++; if(name == names.end()) break;  
    }
}

void WidgetDeviceControl2::setName(QString name) { labelName->setText(name); }


void WidgetDeviceControl2::linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device)
{
   DeviceLinked = Device; 
               if(Device) linkSignals();
}

void WidgetDeviceControl2::linkSignals()
{
    QVector<QPushButton*> buttonsLevel;

    int level = 1;
    for(auto button: buttonsLevel)
    {
      connect(button, &QPushButton::toggled, [level, this](bool Device) { if(Device) DeviceLinked->setLevel(level); } ); level++;
    }

    int device = 1;
    for(auto button: buttonsDevice)
    {
      connect(button, &QPushButton::toggled, [device, this](bool Device) { DeviceLinked->setEnable(Device,device); } ); device++;
    }

    //connect(spinParam, &QSpinBox::valueChanged, [this](int Value) { DeviceLinked->setValue(Value); } ); 


    //if(!groupArrows->isVisible()) return;

    std::vector<QPair<float,float>> VelsVector;
    float VelocityScale = 0.10;
    if(schemeArrowsControl == 4)
    {
        VelsVector.push_back(QPair<float,float>(-VelocityScale, 0));
        VelsVector.push_back(QPair<float,float>( VelocityScale, 0));
        VelsVector.push_back(QPair<float,float>( 0, VelocityScale));
        VelsVector.push_back(QPair<float,float>( 0,-VelocityScale));

        for(int n = 0; n < 4; n++)
        {
        auto Velocity = VelsVector[n];
        auto button = buttonsArrow[n];
        QObject::connect(button, &QPushButton::pressed,  [this, Velocity]() 
        {   
            qDebug() << "MOVE VELOCITY: " << Velocity; 
            DeviceLinked->setPair(Velocity); timerCheckDevice.start(1);
        });
        QObject::connect(button, &QPushButton::released, [this     ]()      { DeviceLinked->setEnable(false);  timerCheckDevice.stop(); });
        }

        QObject::connect(&timerCheckDevice, &QTimer::timeout, [this]() mutable
        {
            NodeSynchronizer.synchronizePeers();
            State = DeviceLinked->getPair(); 
            labelsState[0]->setText(QString("%1\n%2").arg(State.first).arg(State.second));
        });
    }


    std::vector<float> VelsScalar;
    if(schemeArrowsControl == 2)
    {
        VelsScalar.push_back(VelocityScale );
        VelsScalar.push_back(-VelocityScale);

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
            labelsState[0]->setText(QString("%1\n%2").arg(Position.first).arg(Position.second));
        });

}

WidgetDeviceControl2::~WidgetDeviceControl2() { }

