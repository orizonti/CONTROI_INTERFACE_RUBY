#ifndef CAMERA_REMOTE_CONTROL_CLASS_H
#define CAMERA_REMOTE_CONTROL_CLASS_H

#include "rotate_vector_class.h"
#include "state_block_enum.h"

#include <QSettings>
#include <QTimer>
#include "engine_udp_interface.h"
#include "engine_tcp_interface.h"

#include "debug_output_filter.h"
#include "engine_tcp_interface.h"
#include "device_generic_interface.h"


template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
class DeviceCameraRemoteInterface : public DeviceGenericInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>, 
                                    public DeviceGenericHandleControl
{
public:
    using DEVICE_INTERFACE = DeviceGenericInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>; 
    explicit DeviceCameraRemoteInterface(std::shared_ptr<T_CONNECTION> Connection, QString Name = "[ DEVICE ]");
	        ~DeviceCameraRemoteInterface();
	QString TAG_NAME{"[ CAM_RMT_CTRL ]"};
	QString DISPLAY_NAME{"Камера"};

	public:
    void loadSettings(){};
	void setParam(uint8_t ID, uint32_t) override;
    void putMessage(T_MESSAGE Message) override; 
	QString getName() { return DISPLAY_NAME; }

	public:
    //========================================================
    //DEVICE_GENERIC_HANDLE_CONTROL
	void setLevel(int Level)   override 
    { 
        qDebug() << TAG_NAME << "SET ROI LEVEL: " << Level;

    };
    void setValue(float Value) override {}
	void setEnable(bool OnOff, int Number = 0) override
    {
    }
    //========================================================
    T_MESSAGE messageState;
private:
    T_COMMAND Command;
};

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
DeviceCameraRemoteInterface<T_CONNECTION,T_COMMAND,T_MESSAGE>::~DeviceCameraRemoteInterface() { qDebug() << TAG_NAME << "DELETE"; }

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
void DeviceCameraRemoteInterface<T_CONNECTION,T_COMMAND,T_MESSAGE>::setParam(uint8_t ID, uint32_t Value)
{
	//Command.DATA.Command = ID;
	//Command.DATA.Param   = Value;
    //qDebug() << "CAMREA REMOTE CONTROL: " << Qt::hex << Command.DATA.Command << " PARAM: " << Qt::dec << Command.DATA.Param;
    //this->sendCommand(Command);
}


template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
void DeviceCameraRemoteInterface<T_CONNECTION,T_COMMAND,T_MESSAGE>::putMessage(T_MESSAGE message)
{
	messageState = message;
	qDebug() << "[ CAMERA MODULE GET MESSAGE ]";
}

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
DeviceCameraRemoteInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>::DeviceCameraRemoteInterface(std::shared_ptr<T_CONNECTION> Connection, QString Name): 
DeviceGenericInterface<T_CONNECTION,T_COMMAND, T_MESSAGE>(Connection, Name)
{
}

#endif 
