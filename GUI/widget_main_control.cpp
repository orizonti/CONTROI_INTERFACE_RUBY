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
     butControl1.setStyleSheet(styleButtons);
     butControl2.setStyleSheet(styleButtons);


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

        butControl1.setMinimumHeight(40);
        butControl2.setMinimumHeight(40);

                     mainLayout.addLayout(&gridLayout);
                     mainLayout.addLayout(&rightLayout);
    this->setLayout(&mainLayout);

    widgetElevation->setMaximumSize(110,110);
      widgetAzimuth->setMaximumSize(300,300);
    widgetAzimuth->setNull(90);

    gridLayout.addWidget(&butRegimLaser ,1,1);
    gridLayout.addWidget(&butRegimIllum ,2,1);
    gridLayout.addWidget(&butRegimWork  ,1,2);
    gridLayout.addWidget(&butRegimReady ,2,2);
    gridLayout.addWidget(widgetAzimuth  ,1,3,-1,1);

    rightLayout.addWidget(widgetElevation );
    rightLayout.addSpacerItem(new QSpacerItem(20,100,QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    rightLayout.addWidget(&butControl1    );
    rightLayout.addWidget(&butControl2);

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

    butControl1.setMinimumHeight(40);
    butControl2.setMinimumHeight(40);

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

    gridLayout.addWidget(&butControl1    ,4,1);
    gridLayout.addWidget(&butControl2,4,2);


      butRegimLaser.setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
      butRegimIllum.setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
       butRegimWork.setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
      butRegimReady.setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

      widgetAzimuth->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
      widgetElevation->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

      mainLayout2.addSpacerItem(new QSpacerItem(20,50,QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));

    }


}

void WidgetMainControl::linkToDeviceRotary(std::shared_ptr<DeviceGenericHandleControl> Device)
{
     widgetAzimuth->linkToDevice(Device);
     widgetElevation->linkToDevice(Device);
}

void WidgetMainControl::linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device, int Number)
{
     QVector<QPushButton*> buttons;
     buttons.push_back(&butRegimReady);
     buttons.push_back(&butRegimIllum);
     buttons.push_back(&butRegimLaser);
     buttons.push_back(&butRegimWork);
     buttons.push_back(&butRegimWork);

    QObject::connect(buttons[Number], &QPushButton::toggled, [Device,this](bool OnOff) { Device->setEnable(OnOff); });
}


void WidgetMainControl::setName(QString name) 
{ 
};

void WidgetMainControl::synchronizePeer(WidgetMainControl* widget)
{
     widgetPeer = widget;
     widget->widgetPeer = this;

//     widgetAzimuth->synchronizePeer(widgetPeer->widgetAzimuth);
//   widgetElevation->synchronizePeer(widgetPeer->widgetElevation);

   connect(&widgetPeer->butRegimIllum, SIGNAL(toggled(bool)), this, SLOT(slotPeerChanged()));
   connect(&widgetPeer->butRegimLaser, SIGNAL(toggled(bool)), this, SLOT(slotPeerChanged()));
   connect(&widgetPeer->butRegimReady, SIGNAL(toggled(bool)), this, SLOT(slotPeerChanged()));
   connect(&widgetPeer->butRegimWork , SIGNAL(toggled(bool)), this, SLOT(slotPeerChanged()));

   connect(&this->butRegimIllum, SIGNAL(toggled(bool)), widgetPeer, SLOT(slotPeerChanged()));
   connect(&this->butRegimLaser, SIGNAL(toggled(bool)), widgetPeer, SLOT(slotPeerChanged()));
   connect(&this->butRegimReady, SIGNAL(toggled(bool)), widgetPeer, SLOT(slotPeerChanged()));
   connect(&this->butRegimWork , SIGNAL(toggled(bool)), widgetPeer, SLOT(slotPeerChanged()));
}

void WidgetMainControl::slotPeerChanged()
{
     butRegimIllum.blockSignals(true); butRegimIllum.setChecked(widgetPeer->butRegimIllum.isChecked()); butRegimIllum.blockSignals(false);
     butRegimLaser.blockSignals(true); butRegimLaser.setChecked(widgetPeer->butRegimLaser.isChecked()); butRegimLaser.blockSignals(false);
     butRegimReady.blockSignals(true); butRegimReady.setChecked(widgetPeer->butRegimReady.isChecked()); butRegimReady.blockSignals(false);
      butRegimWork.blockSignals(true);  butRegimWork.setChecked(widgetPeer->butRegimWork.isChecked() ); butRegimWork.blockSignals(false);
}