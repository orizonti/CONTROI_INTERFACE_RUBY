#ifndef DEVICE_GENERIC_CLASS_H
#define DEVICE_GENERIC_CLASS_H

#include "interface_pass_coord.h"
#include <typeinfo>
#include "engine_type_register.h"
#include "engine_keyfilter.h"
#include "message_command_structures.h"


class DeviceGenericHandleControl
{
	public:
	virtual void setValue ( float    Value) { setParam(0,Value); };
	virtual void setLevel ( uint32_t Level) { setParam(1,Level); };
	virtual void setPair  ( std::pair<float,float> Coord)    {};
	virtual void setEnable(bool OnOff, uint16_t Number = 0) {};

	virtual std::pair<float,float> getPair()  { return std::pair<float,float>(0,0); };
	virtual                  float getValue() { return 0; };

	virtual void  setParam (uint16_t CommandID, float    CommandParam) {} ;
	virtual float getParam (uint16_t CommandID) { return 0;} ;
};


template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
class DeviceGenericInterface : public DeviceGenericHandleControl
{
public:
    DeviceGenericInterface(std::shared_ptr<T_CONNECTION> Connection, QString Name = "[ DEVICE ]"): TAG_NAME(Name) 
	{ 
		ConnectionDevice = Connection;
	};
	QString TAG_NAME;

	~DeviceGenericInterface() {};

    virtual void putMessage(T_MESSAGE Message) {};

	void transmitMessage(const char* Message, int size, uint16_t param = 0) { ConnectionDevice->slotSendMessage(Message, size, param); }

	void  setParam (uint16_t CommandID, float    CommandParam) override {} ;
	float getParam (uint16_t CommandID) override { return 0;} ;

	void sendCommand(QByteArray command) { ConnectionDevice->slotSendMessage(command); };


	void sendCommand(T_COMMAND& commandToSend) 
	{ 
		qDebug()<< TAG_NAME << "[ SEND COMMAND ]" << T_COMMAND::printDataType();
				  Command = commandToSend;
				  Command.dumpToByteArray(MessageOutputBuffer);
		ConnectionDevice->slotSendMessage(MessageOutputBuffer);
	};

	template<typename T> 
	void sendCommand(const QPair<T,T>& data) 
	{ 
		//qDebug()<< TAG_NAME << "[ SEND SET PAIR COMMAND ]" << T_COMMAND::printDataType();
		Command.setData(data); 
		Command.dumpToByteArray(MessageOutputBuffer);
		ConnectionDevice->slotSendMessage(MessageOutputBuffer);
	};

	template<typename T> 
	void sendCommand(const QPair<T,T>& data, const QPair<T,T>& data2) 
	{ 
		qDebug()<< TAG_NAME << "[ SEND SET TWO PAIR COMMAND ]" << T_COMMAND::printDataType();
		Command.setData(data,data2); 
		Command.dumpToByteArray(MessageOutputBuffer);
		ConnectionDevice->slotSendMessage(MessageOutputBuffer);
	};

	template<COMMAND_STANDART T_COMMAND>
	void sendCommand(T_COMMAND& commandToSend) 
	{ 
		qDebug()<< TAG_NAME << "[ SEND STANDART COMMAND ]" << typeid(commandToSend).name();
				  Command.DATA = commandToSend;
		ConnectionDevice->slotSendMessage(Command.castToByteArray());
	};

	
protected:
    std::shared_ptr<T_CONNECTION> ConnectionDevice = nullptr;
    T_COMMAND Command;
    T_MESSAGE Message;

    QByteArray MessageOutputBuffer;
};



//=========================================================
template<typename T, typename H> class MessageGenericExt; 
                                 class MESSAGE_HEADER_EXT; 

template<int NUM_DEVICE> struct CommandDevice;
template<int NUM_DEVICE> struct RequestDevice;
template<int NUM_DEVICE>  class MessageDeviceGeneric : public MessageGenericExt<CommandDevice<NUM_DEVICE>   ,MESSAGE_HEADER_EXT> { public: };

template<typename T_CONNECTION, int NUM_DEVICE>
class DeviceGenericControl : public DeviceGenericInterface<T_CONNECTION, MessageDeviceGeneric<NUM_DEVICE> ,MessageDeviceGeneric<NUM_DEVICE> >
{
public:
    using DEVICE_INTERFACE = DeviceGenericInterface<T_CONNECTION, MessageDeviceGeneric<NUM_DEVICE>, MessageDeviceGeneric<NUM_DEVICE>>; 
    explicit DeviceGenericControl(std::shared_ptr<T_CONNECTION> Connection, QString Name = "[ DEVICE ]") : DEVICE_INTERFACE(Connection, Name)
	{

	}
	~DeviceGenericControl()
	{

	};

	QString TAG_NAME{"[ DEVICE_ANY ]"};
	QString DISPLAY_NAME{"Устройство"};
};
//=========================================================


using MessageAiming = MessageGenericExt<CommandAiming<0> ,MESSAGE_HEADER_EXT>; 
template<typename T_CONNECTION>
class DeviceGenericAiming : public DeviceGenericInterface<T_CONNECTION, MessageAiming, MessageAiming>
{
public:
    using DEVICE_INTERFACE = DeviceGenericInterface<T_CONNECTION, MessageAiming, MessageAiming>; 
    explicit DeviceGenericAiming(std::shared_ptr<T_CONNECTION> Connection, QString Name = "[ DEVICE ]") : DEVICE_INTERFACE(Connection, Name)
	{
	}

	~DeviceGenericAiming() { };

	QString DISPLAY_NAME{"Наведение"};
	QPair<float,float> CoordAim;
	CommandAiming<0> Command;

	void setPair(std::pair<float,float> Coord) override
    { 
    if(!KeyboardFilter::isControlPressed()) { Command.CommandType = 0; CoordAim = Coord; } 
    if( KeyboardFilter::isControlPressed()) { Command.CommandType = 1; CoordAim = Coord - CoordAim; } 

    if(!KeyboardFilter::isControlPressed()) qDebug() << this->TAG_NAME << "[ COORD AIM ]"        << CoordAim.first << CoordAim.second ; 
    if( KeyboardFilter::isControlPressed()) qDebug() << this->TAG_NAME << "[ COORD CORRECTION ]" << CoordAim.first << CoordAim.second ; 

						  Command.Command = CoordAim;
        this->sendCommand(Command);
    };

	void setEnable(bool OnOff, uint16_t Number = 0) 
	{
		qDebug() << this->TAG_NAME << "[ ENABLE ] [ NOT IMPLEMENTED ]";
	};
};
//=========================================================

#endif 
