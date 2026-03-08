#ifndef DEVICE_GENERIC_CLASS_H
#define DEVICE_GENERIC_CLASS_H

#include "interface_pass_coord.h"
#include <typeinfo>


template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
class DeviceGenericInterface //: public DeviceGenericHandleControl
{
public:
    DeviceGenericInterface(std::shared_ptr<T_CONNECTION> Connection, QString Name = "[ DEVICE ]"): TAG_NAME(Name) 
	{ 
		ConnectionDevice = Connection;
	};
	QString TAG_NAME;

	~DeviceGenericInterface() {};

    virtual void putMessage(T_MESSAGE Message) {};
	virtual void setParam(uint8_t ID, uint32_t) {};
	        void setParam(uint8_t ID, bool OnOff) {if(OnOff) setParam(ID,1); else setParam(ID,0);};

	void sendCommand(QByteArray command) { ConnectionDevice->slotSendMessage(command); };
	void sendCommand(T_COMMAND& command) 
	{ 
	                                      //MessageOutputBuffer = command.castToByteArray();
	                                      command.dumpToByteArray(MessageOutputBuffer);
		ConnectionDevice->slotSendMessage(MessageOutputBuffer);
	};

	template<typename T> 
	void sendCommand(const QPair<T,T>& data) { Message.setData(data); 
	                   //MessageOutputBuffer = Message.castToByteArray();
	                   Message.dumpToByteArray(MessageOutputBuffer);
			            ConnectionDevice->slotSendMessage(MessageOutputBuffer); };
	
protected:
    std::shared_ptr<T_CONNECTION> ConnectionDevice = nullptr;
    T_COMMAND Message;
    QByteArray MessageOutputBuffer;
};

class DeviceGenericHandleControl
{
	public:
	virtual void setLevel(int Level) {};
	virtual void setValue(float Value) {};
	virtual void setCoord(std::pair<float,float> Coord) {};

	virtual std::pair<float,float> getCoord() { return std::pair<float,float>(0,0); };
	virtual                  float getValue() { return 0; };
	virtual  void setEnable(bool OnOff, int Number = 0) = 0;
};

class DeviceRotaryInterface : public PassCoordClass<float>
{
	public:

	virtual void moveToPosRelative(const QPair<float, float>& Pos) = 0;
	virtual void moveOnStep       (const QPair<float, float>& Pos) = 0;
	virtual void moveToPos        (const QPair<float, float>& Pos) = 0;
	virtual void moveWithVelocity (const QPair<float, float>& Velocity) = 0;
	virtual void moveWithVelocityManual(const QPair<float, float>& Velocity) = 0;
	virtual void stopMove() = 0;

	const QPair<float, float>& getOutput() { PassCoordClass<float>::OutputCoord = getPos(); return PassCoordClass<float>::OutputCoord;};
	void setInput(const QPair<float, float>& Coord) { moveToPos(Coord); };

	virtual const QPair<float,float>& getPos() = 0;
	virtual const QPair<float,float>& getPosDevice() = 0;
	virtual const QPair<float,float>& getVelocity() = 0;
	virtual const QPair<float,float>& getVelocityDevice() = 0;

	virtual QPair<float,float> getLimits()  = 0;
};


#endif 
