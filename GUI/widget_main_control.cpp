#include "widget_main_control.h"
#include "debug_output_filter.h"
#include "widget_rotary_control.h"
#include "device_rotary_interface.h"
#include "QSpacerItem"
#include <QPushButton>

QString styleButtonsToggled
{
"QPushButton { background-color: rgba(204, 53, 10, 62);"
"    border: 2px solid line;"
"    border-radius: 6px;	"
"    border-color: rgb(215, 50, 28); }"

"QPushButton:checked {"
"	background-color: rgba(32, 156, 19, 83);"
"     border: 2px solid line;"
"     border-radius: 6px;"
"	border-color: rgb(30, 221, 78); }"
};


QString styleButtons{

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

QString StyleTest{

"QWidget { background-color: rgba(209, 142, 34, 60); }"
};


WidgetMainControl::WidgetMainControl(int Scheme, QWidget* parent) : WidgetAdjustable(parent)
{
    this->setStyleSheet(styleButtonsToggled);
        butToHandle.setStyleSheet(styleButtons);
    butToBigControl.setStyleSheet(styleButtons);


    //widgetAzimuth->setStyleSheet(StyleTest);
    widgetElevation = new WidgetRotaryControl;
    widgetAzimuth   = new WidgetRotaryControl;
    widgetElevation->setType(WidgetRotaryControl::TypeDraw::TypeArc);

     mainLayout.setSpacing(2);
     gridLayout.setSpacing(2);
    rightLayout.setSpacing(2);

     mainLayout.setContentsMargins(1,1,1,1);
     gridLayout.setContentsMargins(1,1,1,1);
    rightLayout.setContentsMargins(1,1,1,1);

    butRegimReady.setCheckable(true);
     butRegimWork.setCheckable(true);
    butRegimLaser.setCheckable(true);
    butRegimIllum.setCheckable(true);

    butRegimReady.setMinimumWidth(68);
     butRegimWork.setMinimumWidth(68);
    butRegimLaser.setMinimumWidth(68);
    butRegimIllum.setMinimumWidth(68);



    if(Scheme == 0)
    {

        butToHandle.setMinimumHeight(40);
    butToBigControl.setMinimumHeight(40);
                     mainLayout.addLayout(&gridLayout);
                     mainLayout.addLayout(&rightLayout);
    this->setLayout(&mainLayout);

    widgetElevation->setMaximumSize(110,110);
      widgetAzimuth->setMaximumSize(300,300);

    gridLayout.addWidget(&butRegimLaser ,1,1);
    gridLayout.addWidget(&butRegimIllum ,2,1);
    gridLayout.addWidget(&butRegimWork  ,1,2);
    gridLayout.addWidget(&butRegimReady ,2,2);
    gridLayout.addWidget(widgetAzimuth  ,1,3,-1,1);

    rightLayout.addWidget(widgetElevation );
    rightLayout.addSpacerItem(new QSpacerItem(20,100,QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    rightLayout.addWidget(&butToHandle    );
    rightLayout.addWidget(&butToBigControl);

      butRegimLaser.setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding));
      butRegimIllum.setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding));
      butRegimWork.setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding));
      butRegimReady.setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding));

      widgetAzimuth->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    widgetElevation->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

    }

    if(Scheme == 1)
    {
                     mainLayout2.addLayout(&gridLayout);
    this->setLayout(&mainLayout2);

    butRegimReady.setMinimumHeight(40);
     butRegimWork.setMinimumHeight(40);
    butRegimLaser.setMinimumHeight(40);
    butRegimIllum.setMinimumHeight(40);

    butToBigControl.setMinimumHeight(40);
        butToHandle.setMinimumHeight(40);

    widgetElevation->setMaximumSize(100,90);
      widgetAzimuth->setMinimumSize(240,240);
      widgetAzimuth->setMaximumSize(330,330);


    gridLayout.setRowMinimumHeight(2,90);
    gridLayout.setRowMinimumHeight(3,280);
    //gridLayout.setRowStretch(1,1);

    gridLayout.addWidget(&butRegimLaser  ,0,1);
    gridLayout.addWidget(&butRegimIllum  ,1,1);
    gridLayout.addWidget(&butRegimWork   ,0,2);
    gridLayout.addWidget(&butRegimReady  ,1,2);
    //mainLayout2.addWidget(widgetAzimuth   ,5,1,1,-1);
    //mainLayout2.addWidget(widgetElevation ,6,1,1,-1);
    gridLayout.addWidget(widgetElevation,2,2);
    //mainLayout2.addWidget(widgetAzimuth  );
    gridLayout.addWidget(widgetAzimuth   ,3,1,-1,-1,Qt::AlignTop);

    gridLayout.addWidget(&butToHandle    ,4,1);
    gridLayout.addWidget(&butToBigControl,4,2);


      butRegimLaser.setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
      butRegimIllum.setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
       butRegimWork.setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
      butRegimReady.setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

      widgetAzimuth->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
      widgetElevation->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

      mainLayout2.addSpacerItem(new QSpacerItem(20,50,QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));

    }


}

void WidgetMainControl::linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device)
{
     widgetAzimuth->linkToDevice(Device);
     widgetElevation->linkToDevice(Device);

//    QObject::connect(&timerCheckState, &QTimer::timeout, [Device,RangeScale,this]()
//    {
//                                        Position = Device->getPos()*RangeScale; 
//    });
}


void WidgetMainControl::setName(QString name) 
{ 
};

//    float VelocityScale = 0.5;
//    float VelocityScale = 2;
//
//    std::vector<QPushButton*> ArrowButtons;
//    std::vector<QPair<float,float>> Vels;
//    ArrowButtons.push_back(ui->butRotaryX_Left);  Vels.push_back(QPair<float,float>(-VelocityScale, 0));
//    ArrowButtons.push_back(ui->butRotaryX_Right); Vels.push_back(QPair<float,float>( VelocityScale, 0));
//    ArrowButtons.push_back(ui->butRotaryY_Up);    Vels.push_back(QPair<float,float>( 0, VelocityScale));
//    ArrowButtons.push_back(ui->butRotaryY_Down);  Vels.push_back(QPair<float,float>( 0,-VelocityScale));
//
//    for(int n = 0; n < 4; n++)
//    {
//    auto Velocity = Vels[n];
//    auto button = ArrowButtons[n];
//    QObject::connect(button, &QPushButton::pressed,  [this, Device, Velocity]() { Device->moveWithVelocityManual(Velocity); timerCheckState.start(1);});
//    //QObject::connect(button, &QPushButton::pressed,  [this, Device, Velocity]() { Device->moveWithVelocity(QPair<float,float>(0.5,0.5)); timerCheckState.start(1);});
//    QObject::connect(button, &QPushButton::released, [this, Device     ]()      { Device->stopMove(); timerCheckState.stop();});
//    }
//
//    auto  MoveLimits = Device->getLimits();
//    qDebug() << "GET LIMITS : " << MoveLimits.first << MoveLimits.second;
//    float RangeScale = 2000;
//
//    ui->scrollRotaryAxisX->setRange(-MoveLimits.first*RangeScale, MoveLimits.first*RangeScale);
//    ui->scrollRotaryAxisX->setValue(0);
//    ui->scrollRotaryAxisX->setPageStep(50);
//
//    ui->scrollRotaryAxisY->setRange(-MoveLimits.second*RangeScale, MoveLimits.second*RangeScale);
//    ui->scrollRotaryAxisY->setValue(0);
//    ui->scrollRotaryAxisY->setPageStep(50);
//
//    std::vector<QScrollBar*> scrollWidgets;
//                             scrollWidgets.push_back(ui->scrollRotaryAxisX);
//                             scrollWidgets.push_back(ui->scrollRotaryAxisY);
//
//    for(auto scroll_axis: scrollWidgets)
//    {
//    QObject::connect(scroll_axis, &QScrollBar::sliderReleased, [Device, this]() 
//    { 
//        Device->moveToPos(this->Position); 
//    });
//    }