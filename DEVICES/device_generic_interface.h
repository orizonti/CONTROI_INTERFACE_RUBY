#ifndef DEVICE_GENERIC_CLASS_H
#define DEVICE_GENERIC_CLASS_H

#include "message_command_structures.h"
#include "message_struct_generic_ext.h"
#include "message_struct_generic.h"
#include "message_header_generic_ext.h"
#include "interface_pass_coord.h"
#include <typeinfo>

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
class DeviceGenericInterface 
{
public:
    DeviceGenericInterface(std::shared_ptr<T_CONNECTION> Connection, QString Name = "[ DEVICE ]"): TAG_NAME(Name) 
	{ 
		ConnectionDevice = Connection;
		init();
	};
	QString TAG_NAME;

	~DeviceGenericInterface() {};

	virtual void setParam (uint8_t ID, uint16_t Value) = 0; 
	        void setParam (uint8_t ID, bool OnOff) { if(OnOff) setParam(ID,(uint16_t)1); else setParam(ID,(uint16_t)0);}; 

	void sendCommand(T_COMMAND& command) { ConnectionDevice->slotSendMessage(command.toByteArray());};

	template<typename T> 
	void sendCommand(const QPair<T,T>& data) { Message.setData(data); ConnectionDevice->slotSendMessage(Message.toByteArray()); };

    virtual void putMessage(T_MESSAGE Message) = 0;
	void init()
	{
//	   auto callBack = [this](MessageGeneric<void*, T_CONNECTION::HeaderType>& Message)
//	   { 
//        auto MessageData = ConnectionDevice->Dispatcher->ExtractData<T_MESSAGE>(&Message);
//		putMessage(*MessageData);
//	   }; 
//       if(ConnectionDevice) ConnectionDevice->Dispatcher->AppendCallback<T_MESSAGE>(callBack);
	}
	
protected:
    std::shared_ptr<T_CONNECTION> ConnectionDevice = nullptr;
    T_COMMAND Message;
};


class DeviceLaserGenericInterface
{
	public:
	virtual void setEnable(bool OnOff) = 0;
	virtual void setPowerEnable(bool OnOff) = 0;
	virtual void setPilotEnable(bool OnOff) = 0;
	virtual void setPower(uint16_t Value) = 0;

	virtual void setPowerHigh() = 0;
	virtual void setPowerMiddle() = 0;
	virtual void setPowerLow() = 0;
	virtual QString getName() { return QString("Силовой лазер"); }

};

class DeviceFocusGenericInterface
{
	public:
	virtual void setEnable(bool OnOff) = 0;
	virtual void setPos(uint16_t Value) = 0;
	virtual uint16_t getDistance() = 0;
};

class DeviceLidInterface
{
	public:
	virtual void openLid() = 0;
	virtual void closeLid() = 0;
};

class DeviceRotaryInterface : public PassCoordClass<float>
{
	public:

	virtual void moveToPosRelative(const QPair<int, int>& Pos) = 0;
	virtual void moveOnStep(const QPair<int, int>& Pos) = 0;
	virtual void moveToPos(const QPair<int, int>& Pos) = 0;
	virtual void moveWithVelocity(const QPair<int, int>& Velocity) = 0;
	virtual void moveWithVelocityManual(const QPair<int, int>& Velocity) = 0;
	virtual void stopMove() = 0;

	const QPair<float, float>& GetOutput() { PassCoordClass<float>::OutputCoord = getPos(); return PassCoordClass<float>::OutputCoord;};
	void SetInput(const QPair<float, float>& Coord) { moveToPos(Coord); };

	virtual const QPair<int,int>& getPos() = 0;
	virtual const QPair<float,float>& getPosDevice() = 0;
	virtual const QPair<int,int>& getVelocity() = 0;
	virtual const QPair<int,int>& getVelocityDevice() = 0;

	virtual QPair<int,int> getLimits(int axis)  = 0;
	virtual QPair<int,int> getRange() = 0 ; 
};


#endif 
