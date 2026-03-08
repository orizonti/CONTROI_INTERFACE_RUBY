#ifndef DEVICE_FOCUS_CONTROL_INTERFACE_H
#define DEVICE_FOCUS_CONTROL_INTERFACE_H

#include "state_block_enum.h"
#include <QSettings>
#include "device_generic_interface.h"
#include <typeinfo>

//class DeviceFocusGenericInterface
//{
//	public:
//	virtual void setEnable(bool OnOff) = 0;
//	virtual void setValue(float Value) = 0;
//	virtual uint32_t getDistance() = 0;
//};

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
class DeviceFocusRangerInterface : public DeviceGenericInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>, 
                                   public DeviceGenericHandleControl
{
public:
    explicit DeviceFocusRangerInterface(std::shared_ptr<T_CONNECTION> Connection, QString Name = "[ DEVICE ]") :
             DeviceGenericInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>(Connection,Name) 
             {
              commandArray = QByteArray((char*)(&DEVICE_INTERFACE::Message.DATA), 4);
             };
        	~DeviceFocusRangerInterface() { };

    using DEVICE_INTERFACE = DeviceGenericInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>; 

	void setParam(uint8_t ID, uint32_t Param) override
    {
                QDataStream stream(&commandArray, QIODevice::WriteOnly);
                 stream << ID;
        qDebug() << "[ FOCUSATOR ] [ SEND COMMAND ]" << commandArray.toHex(); 
        DEVICE_INTERFACE::sendCommand(commandArray);
    }

	void setEnable(bool OnOff, int Number = 0)  override { if(OnOff) setParam(0,1); else setParam(0,0); }; 
    void setValue(float Value) override 
    { 
                   int8_t* DataPtr = (int8_t*)&Value+1;
                 QDataStream stream(&commandArray, QIODevice::WriteOnly);

        qDebug() << "[ FOCUSATOR ] [ SEND COMMAND ]" << this->Message.toByteArray().toHex(); 
        DEVICE_INTERFACE::sendCommand(this->Message);
    };

	float getDistance() { return (float)messageState.Param1; };
	float getValue() override { return getDistance(); };


        void putMessage(T_MESSAGE Message) { messageState = Message;};
    T_MESSAGE messageState;
    QByteArray commandArray;
};

#endif 
