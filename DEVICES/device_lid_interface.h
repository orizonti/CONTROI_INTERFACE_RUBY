#ifndef DEVICE_LID_CONTROL_CLASS_H
#define DEVICE_LID_CONTROL_CLASS_H

#include "message_command_structures.h"
#include "device_generic_interface.h"


template<typename T_CONNECTION>
class DeviceLidControl : public DeviceGenericInterface<T_CONNECTION, CommandLidControlJson, CommandLidControlJson>,
                         public DeviceLidInterface
{
public:
  DeviceLidControl(std::shared_ptr<T_CONNECTION> Connection, QString command1 , QString command2, QString name = "[LID]"
                   ): commandOpen (command1), 
                      commandClose(command2), 
                      DeviceGenericInterface<T_CONNECTION,CommandLidControlJson, CommandLidControlJson>(Connection, name) {};

	QString TAG_NAME{"[ LID ]"};

	void setParam (uint8_t ID, uint16_t Value) {}; 
  void putMessage(CommandLidControlJson Message)         {};

  void openLid () {commandOpen.printCommand(); this->sendCommand(commandOpen); };
	void closeLid() {commandClose.printCommand(); this->sendCommand(commandClose);};

  CommandLidControlJson commandOpen;
  CommandLidControlJson commandClose;
};

#endif 
