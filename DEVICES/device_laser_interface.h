#ifndef LASER_CONTROL_CLASS_H
#define LASER_CONTROL_CLASS_H

#include "rotate_vector_class.h"
#include "state_block_enum.h"

#include <QSettings>
#include <QTimer>
#include "engine_udp_interface.h"
#include "engine_tcp_interface.h"

#include "debug_output_filter.h"
#include "engine_tcp_interface.h"
#include "device_generic_interface.h"
#include <QDebug>
#include <QTimer>


template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
class DeviceLaserInterface : public DeviceGenericInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>, 
                             public DeviceGenericHandleControl
{
public:
    using DEVICE_INTERFACE = DeviceGenericInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>; 
    explicit DeviceLaserInterface(std::shared_ptr<T_CONNECTION> Connection, QString Name = "[ DEVICE ]");
	        ~DeviceLaserInterface();
	QString TAG_NAME{"[ LASER ]"};
	QString DISPLAY_NAME{"Силовой лазер"};

	public:
    void loadSettings(){};
	void setParam(uint8_t ID, uint32_t) override;
    void putMessage(T_MESSAGE Message) override; 
	QString getName() { return DISPLAY_NAME; }

	public:

    void setCheckProcedure();
	void setReady(bool OnOff)       { setParam(LASER_MODULE,OnOff); };
	void setPowerEnable(bool OnOff) { setParam(LASER_MODULE_BEAM ,OnOff); };
	void setPilotEnable(bool OnOff) { setParam(LASER_MODULE_PILOT,OnOff); };

    //========================================================
    //DEVICE_GENERIC_HANDLE_CONTROL
	void setLevel(int Level) override { if(Level == 0) setPowerLow(); if(Level == 1) setPowerHigh(); };
    void setValue(float Value) override { setPower(90*Value); }
	void setEnable(bool OnOff, int Number = 0) override
    {
        if(Number == 0) setPowerEnable(OnOff); 
        if(Number == 1) setPilotEnable(OnOff); 
    }
    //========================================================

	void setPower(uint16_t Value)   
    { 
        qDebug() << "====================================";
        qDebug() << "SET POWER: " << Value;
        QTimer::singleShot(10,   [this]() { this->setPowerEnable(false); qDebug() << "[ LASER BEAM OFF ]";        });
        QTimer::singleShot(1000, [this]() { this->setReady(false);       qDebug() << "[ LASER DISABLE  ]";        });
        QTimer::singleShot(2000, [this]() { this->setCheckProcedure();   qDebug() << "[ LASER ENABLE   ]";        });
        QTimer::singleShot(3000, [this]() { this->setReady(true);        qDebug() << "[ LASER ENABLE   ]";        });
        QTimer::singleShot(4000, [this,Value]() { this->setParam(LASER_MODULE_POWER, Value);   
                                                                        qDebug() << "[ LASER SET POWER ]" << Value; });
    };
	void setPowerHigh()   { setPower(90); };
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
void DeviceLaserInterface<T_CONNECTION,T_COMMAND,T_MESSAGE>::setParam(uint8_t ID, uint32_t Value)
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
