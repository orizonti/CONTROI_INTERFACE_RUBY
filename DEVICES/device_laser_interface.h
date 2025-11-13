#ifndef LASER_CONTROL_CLASS_H
#define LASER_CONTROL_CLASS_H

#include "rotate_vector_class.h"
#include "state_block_enum.h"

#include <QSettings>
#include <QTimer>
#include "engine_udp_interface.h"
#include "engine_tcp_interface.h"
#include "message_command_structures.h"
#include "message_struct_generic_ext.h"
#include "message_header_generic_ext.h"

#include "debug_output_filter.h"
#include "engine_tcp_interface.h"
#include "widget_laser_control.h"
#include "device_generic_interface.h"
#include <QDebug>
#include <QTimer>

//class DeviceLaserDelayedControl: public QObject
//{
//    public slots:
//    void slotEnableLaser();
//    void slotSetPowerHigh();
//    void slotSetPowerMiddle();
//    void slotSetPowerLow();
//};

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
class DeviceLaserInterface : public DeviceGenericInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>, public DeviceLaserGenericInterface
{
public:
    using DEVICE_BASE_TYPE = DeviceGenericInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>; 
    explicit DeviceLaserInterface(std::shared_ptr<T_CONNECTION> Connection, QString Name = "[ DEVICE ]");
	        ~DeviceLaserInterface();
	QString TAG_NAME{"[ LASER ]"};
	QString DISPLAY_NAME{"Силовой лазер"};

                             WidgetLaserControl* ControlWindow  = nullptr;
	void linkToControlWindow(WidgetLaserControl* ControlWindow ) {};

	public:
    void loadSettings(){};
	void setParam(uint8_t ID, uint16_t Value) override;
    void putMessage(T_MESSAGE Message) override; 
	QString getName() override { return DISPLAY_NAME; }

	public:

    void setCheckProcedure();
	void setEnable(bool OnOff)      override { setParam(LASER_MODULE,OnOff); };
	void setPowerEnable(bool OnOff) override { setParam(LASER_MODULE_BEAM ,OnOff); };
	void setPilotEnable(bool OnOff) override { setParam(LASER_MODULE_PILOT,OnOff); };

	void setPower(uint16_t Value)   override 
    { 
        qDebug() << "====================================";
        qDebug() << "SET POWER: " << Value;
        QTimer::singleShot(10,   [this]() { this->setPowerEnable(false);qDebug() << "[ LASER BEAM OFF ]";        });
        QTimer::singleShot(1000, [this]() { this->setEnable(false);     qDebug() << "[ LASER DISABLE  ]";        });
        QTimer::singleShot(2000, [this]() { this->setCheckProcedure();  qDebug() << "[ LASER ENABLE   ]";        });
        QTimer::singleShot(3000, [this]() { this->setEnable(true);      qDebug() << "[ LASER ENABLE   ]";        });
        QTimer::singleShot(4000, [this,Value]() { this->setParam(LASER_MODULE_POWER, Value);   
                                                                        qDebug() << "[ LASER SET POWER ]" << Value; });
    };
	void setPowerHigh()   { setPower(99); };
	void setPowerMiddle() { setPower(50);  };
	void setPowerLow()    { setPower(15);  };
	bool getState() { return messageState.Param1 == 0 ? false : true; }
    T_MESSAGE messageState;
private:
    std::map<uint8_t, uint8_t> KEY_MODULE;       //GET NUMBER MODULE FROM COMMAND CODE
    std::map<uint8_t, uint8_t> KEY_MODULE_PARAM; //KEY PARAM VALUE FROM COMMAND CODE
	std::map<uint8_t, std::map<uint8_t,uint8_t>> ID_PARAM_KEY; //GET COMMAND CODE FROM [COMMAND_ID PARAM]
    T_COMMAND Command;
};

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
DeviceLaserInterface<T_CONNECTION,T_COMMAND,T_MESSAGE>::~DeviceLaserInterface() { qDebug() << TAG_NAME << "DELETE"; }

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
void DeviceLaserInterface<T_CONNECTION,T_COMMAND,T_MESSAGE>::setParam(uint8_t ID, uint16_t Value)
{
	uint8_t param = Value > 0 ? 1 : 0;  
	Command.DATA.Command = ID_PARAM_KEY[ID][param];
	Command.DATA.Param    = Value;
    qDebug() << "CODE: " << Qt::hex << Command.DATA.Command << " PARAM: " << Qt::dec << Command.DATA.Param;
    this->sendCommand(Command);
}

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
void DeviceLaserInterface<T_CONNECTION,T_COMMAND,T_MESSAGE>::setCheckProcedure()
{
	Command.DATA.Command  = 0x20;
	Command.DATA.Param    = 0;
    qDebug() << "CODE: " << Qt::hex << Command.DATA.Command << " PARAM: " << Qt::dec << Command.DATA.Param;
	this->sendCommand(Command);
}

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
void DeviceLaserInterface<T_CONNECTION,T_COMMAND,T_MESSAGE>::putMessage(T_MESSAGE message)
{
    //ControlWindow->setState(1);
    //ControlWindow->setParam(0, 0);
	messageState = message;
	qDebug() << "[ LASER MODULE GET MESSAGE ]";
}

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
DeviceLaserInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>::DeviceLaserInterface(std::shared_ptr<T_CONNECTION> Connection, QString Name): 
DeviceGenericInterface<T_CONNECTION,T_COMMAND, T_MESSAGE>(Connection, Name)
{
  QTimer::singleShot(100, [this]()  { this->setCheckProcedure(); qDebug() << "[ LASER ENABLE ]";        });

  DISPLAY_NAME = Name;
  KEY_MODULE[LASER_FAULT]     = LASER_MODULE;
  KEY_MODULE[LASER_ON]        = LASER_MODULE;       KEY_MODULE[LASER_OFF]       = LASER_MODULE;
  KEY_MODULE[LASER_BEAM_ON]   = LASER_MODULE_BEAM ; KEY_MODULE[LASER_BEAM_OFF]  = LASER_MODULE_BEAM;
  KEY_MODULE[LASER_PILOT_ON]  = LASER_MODULE_PILOT; KEY_MODULE[LASER_PILOT_OFF] = LASER_MODULE_PILOT;
  KEY_MODULE[LASER_SET_POWER] = LASER_MODULE_POWER;

  KEY_MODULE_PARAM[LASER_ON]       = 1; KEY_MODULE_PARAM[LASER_OFF]       = 0; KEY_MODULE[LASER_FAULT] = 1;
  KEY_MODULE_PARAM[LASER_BEAM_ON]  = 1; KEY_MODULE_PARAM[LASER_BEAM_OFF]  = 0;
  KEY_MODULE_PARAM[LASER_PILOT_ON] = 1; KEY_MODULE_PARAM[LASER_PILOT_OFF] = 0;
  //==========================================================================
  ID_PARAM_KEY[LASER_MODULE]      [1] = LASER_ON;       ID_PARAM_KEY[LASER_MODULE]      [0] = LASER_OFF;
  ID_PARAM_KEY[LASER_MODULE_BEAM] [1] = LASER_BEAM_ON;  ID_PARAM_KEY[LASER_MODULE_BEAM] [0] = LASER_BEAM_OFF;
  ID_PARAM_KEY[LASER_MODULE_PILOT][1] = LASER_PILOT_ON; ID_PARAM_KEY[LASER_MODULE_PILOT][0] = LASER_PILOT_OFF;
  ID_PARAM_KEY[LASER_MODULE_POWER][1] = LASER_SET_POWER;ID_PARAM_KEY[LASER_MODULE_POWER][0] = LASER_SET_POWER;
}

#endif 
