#ifndef AIMING_MONITORING
#define AIMING_MONITORING
#include "device_generic_interface.h"
#include "message_command_structures.h"
#include "message_header_generic_ext.h"
#include <QTimer>
#include <thread>
#include <atomic>
#include <chrono>

using MessageAimingMonitoring = MessageGenericExt<CommandAiming<0>,MESSAGE_HEADER_GENERIC>;
template<typename T_CONNECTION>
class ModuleAimingMonitoring : public DeviceGenericInterface<T_CONNECTION, MessageAimingMonitoring,MessageAimingMonitoring>
{
public:
    using DEVICE_INTERFACE = DeviceGenericInterface<T_CONNECTION, MessageAimingMonitoring, MessageAimingMonitoring>; 
  explicit ModuleAimingMonitoring(std::shared_ptr<T_CONNECTION> Connection): DEVICE_INTERFACE(Connection, "[AIMING_MONITOR]")
	{
    qDebug() << "AIMING MONITOR INIT";
   init();
	}
	~ModuleAimingMonitoring() 
  { 
    FLAG_END = true; 
    qDebug() << "AIMING MONITOR DELETE";
  };
  std::jthread* thread_watch = nullptr;

  std::atomic_bool FLAG_END = false;
  std::atomic_bool FLAG_WORK = false;

	void setParam (uint16_t CommandID, float    CommandParam) override { }


  void startWork(bool OnOff) { FLAG_WORK = OnOff; qDebug() << TAG_NAME.toStdString().c_str() << "[ START WORK ]"; }  
  CommandAiming<0> StateAiming;

  void init()
  {
      thread_watch = new std::jthread([this] () 
          {
              while(!FLAG_END)
              {
              if(FLAG_WORK) { StateAiming.CommandType =1; StateAiming.Command.first = 2; this->sendCommand(StateAiming); }

              std::this_thread::sleep_for(std::chrono::milliseconds(1000/20));
              }
          });

    thread_watch->detach();
  }

	QString TAG_NAME{"[ AIMING_MONITOR ]"};
};
//=========================================================

#endif
