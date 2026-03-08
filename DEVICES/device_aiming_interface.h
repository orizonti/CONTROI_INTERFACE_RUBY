#ifndef DEVICE_AIMING_CONTROL_CLASS_H
#define DEVICE_AIMING_CONTROL_CLASS_H

#include "device_generic_interface.h"

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
class DeviceAimingControl : public DeviceGenericInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>,
                            public DeviceGenericHandleControl
{
public:
  DeviceAimingControl(std::shared_ptr<T_CONNECTION> Connection, QString name = "[AIMING CONTROL]") :
  DeviceGenericInterface<T_CONNECTION,T_COMMAND, T_MESSAGE>(Connection, name) {};

	QString TAG_NAME{"[ AIMING CONTROL ]"};

	void setParam  (uint8_t ID, uint16_t Value   ) { }; 
  void putMessage(T_MESSAGE Message) { };
	void setValue(float Value) { };
	void setCoord(std::pair<float,float> Coord) 
  { 
    qDebug() << TAG_NAME << "SET COORD: " << Coord.first << Coord.second; 
    this->sendCommand(Coord);
  };

	void setEnable(bool OnOff, int Number = 0) {qDebug() << "[ DEVICE AIMING ENABLE NOT IMPLEMENTED ]";};

};

#endif 
