#ifndef DEVICE_GENERIC_CLASS_H
#define DEVICE_GENERIC_CLASS_H

#include "interface_pass_coord.h"
#include <typeinfo>
#include "engine_type_register.h"
#include "engine_keyfilter.h"

class DeviceGenericHandleControl
{
	public:
	virtual void setLevel( uint32_t Level) {setParam(0, Level);};
	virtual void setValue( float    Value) {setParam(1, Value);};
	virtual void setPair(std::pair<float,float> Coord) {};

	virtual std::pair<float,float> getPair() { return std::pair<float,float>(0,0); };
	virtual                  float getValue() { return 0; };

	virtual void setParam (uint16_t CommandID, uint32_t CommandParam) = 0;
	virtual void setParam (uint16_t CommandID, float    CommandParam)    = 0;

	virtual void setEnable(bool OnOff, uint16_t Number = 0) { setParam(Number,(uint32_t)OnOff);};
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

	void setParam (uint16_t CommandID, uint32_t CommandParam) override {};
	void setParam (uint16_t CommandID, float    CommandParam) override {} ;

	void sendCommand(QByteArray command) { ConnectionDevice->slotSendMessage(command); };

	void sendCommand(T_COMMAND& commandToSend) 
	{ 
		//qDebug()<< TAG_NAME << "[ SEND COMMAND ]" << T_COMMAND::printDataType();
				  Command = commandToSend;
				  Command.dumpToByteArray(MessageOutputBuffer);

		//qDebug() << "[ SEND COMMAND ]" << MessageOutputBuffer.toHex();
		ConnectionDevice->slotSendMessage(MessageOutputBuffer);
	                                      //MessageOutputBuffer = command.castToByteArray();
	};

	template<typename T> 
	void sendCommand(const QPair<T,T>& data) { 
		                         qDebug()<< TAG_NAME << "[ SEND COMMAND ]" << T_COMMAND::printDataType();
								 Command.setData(data); 
								 Command.dumpToByteArray(MessageOutputBuffer);
		               qDebug() << "[ SEND COMMAND ]" << MessageOutputBuffer.toHex();
			           ConnectionDevice->slotSendMessage(MessageOutputBuffer); };

	template<typename T> 
	void sendCommand(const QPair<T,T>& data, const QPair<T,T>& data2) { 
		                         qDebug()<< TAG_NAME << "[ SEND COMMAND ]" << T_COMMAND::printDataType();
								 Command.setData(data,data2); 
								 Command.dumpToByteArray(MessageOutputBuffer);
			           ConnectionDevice->slotSendMessage(MessageOutputBuffer); };
	                   //MessageOutputBuffer = Message.castToByteArray();
	
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


	void setParam (uint16_t CommandID, uint32_t CommandParam) override
	{
      qDebug() << "DEVICE SET PARAM: " << CommandParam;     
	}
	void setParam (uint16_t CommandID, float    CommandParam) override
	{

	}

	QString TAG_NAME{"[ DEVICE_ANY ]"};
	QString DISPLAY_NAME{"Устройство"};
};
//=========================================================
template<int NUM> class CommandAiming;

template<int NUM_DEVICE> class MessageAimingDevice : public MessageGenericExt<CommandAiming<NUM_DEVICE>   ,MESSAGE_HEADER_EXT> { public: };

template<typename T_CONNECTION, int NUM_DEVICE>
class DeviceGenericAiming : public DeviceGenericInterface<T_CONNECTION, MessageAimingDevice<NUM_DEVICE>, MessageAimingDevice<NUM_DEVICE>>
{
public:
    using DEVICE_INTERFACE = DeviceGenericInterface<T_CONNECTION, MessageAimingDevice<NUM_DEVICE>, MessageAimingDevice<NUM_DEVICE>>; 
    explicit DeviceGenericAiming(std::shared_ptr<T_CONNECTION> Connection, QString Name = "[ DEVICE ]") : DEVICE_INTERFACE(Connection, Name)
	{
	}

	~DeviceGenericAiming() { };

	QString DISPLAY_NAME{"Наведение"};
	QPair<float,float> CoordAim;
	QPair<float,float> CoordCorrection;

	void setPair(std::pair<float,float> Coord) override
    { 

    if(!KeyboardFilter::isControlPressed()) { CoordCorrection = QPair<float,float>(0,0); CoordAim = Coord; }
    if( KeyboardFilter::isControlPressed())   CoordCorrection = CoordAim - Coord;

    qDebug() << this->TAG_NAME << "[ COORD AIM ]"      << CoordAim.first        << CoordAim.second  
	                           << "[ SET CORRECTION ]" << CoordCorrection.first << CoordCorrection.second;

    this->sendCommand(Coord, CoordCorrection);
    };

	void setEnable(bool OnOff, uint16_t Number = 0) 
	{
		qDebug() << this->TAG_NAME << "[ ENABLE ] [ NOT IMPLEMENTED ]";
	};
};
//=========================================================

#endif 
