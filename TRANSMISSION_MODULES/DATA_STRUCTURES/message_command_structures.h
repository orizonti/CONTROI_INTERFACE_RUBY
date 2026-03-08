#pragma once
#include <stdint.h>
#include <string>
#include <sstream>
#include <QDataStream>
#include <QIODevice>
#include <QByteArray>
#include <QDebug>
#include "message_command_structures_rotary.h"
#include "message_command_structures_intermediate.h"


template<int NUM_DEV>
struct CommandDevice
{
  public:
  uint16_t DeviceID = 0;
  uint16_t Command = 0;
  uint16_t Param = 0;
};


template<int NUM_DEV>
struct CommandDeviceRedux
{
  public:
  uint8_t Command = 0;
  uint8_t Param = 0;
  uint8_t Reserv1 = 0;
  uint8_t Reserv2 = 0;
  uint8_t Reserv3 = 0;
  uint8_t Reserv4 = 0;
  uint8_t Reserv5 = 0;
  uint8_t Reserv6 = 0;
  friend void operator<<(QDataStream& stream, CommandDeviceRedux& command)
  {
    stream << command.Command;
    stream << command.Param;
    stream << command.Reserv1 << command.Reserv2 << command.Reserv3 << command.Reserv4 << command.Reserv5 << command.Reserv6;
  };
};




template<int N_CHAN>
struct CommandSetPos
{
   public:
   float Param1 = 0;
   float Param2 = 0;

   template<typename T>
   void operator=(const QPair<T,T>& Pos) { Param1 = Pos.first; Param2 = Pos.second; };
   void operator=(StateRotaryControl setting) {Param1 = setting.Engine1.Position; Param2 = setting.Engine2.Position; };
   void setData(StateRotaryControl setting)   {Param1 = setting.Engine1.Position; Param2 = setting.Engine2.Position; }
   friend void operator<<(QDataStream& stream, CommandSetPos& command) { stream << command.Param1 << command.Param2; };
};


template<int N_CHAN>
struct CommandSetSpeed
{
   public:
   float Velocity1 = 0;
   float Velocity2 = 0;
};

struct CommandAiming
{
   public:
   float PosAimingRelativeX = 0.5;
   float PosAimingRelativeY = 0.5;
   void setData(QPair<float,float> Data) { PosAimingRelativeX = Data.first; 
                                           PosAimingRelativeY = Data.second; };
   void operator=(std::pair<float,float> Data) { setData(Data);};
   friend void operator<<(QDataStream& stream, CommandAiming& command)
   {
         stream << command.PosAimingRelativeX;
         stream << command.PosAimingRelativeY;
   }
};

using MessageAiming = CommandAiming;

template<int NUM_DEV>
struct MessageDevice
{
   uint8_t DeviceID = 0;
   uint8_t Module = 0;
   uint16_t Param1 = 0;
   uint16_t Param2 = 0;
   uint16_t Param3 = 0;
};

template<int N_CHAN>
struct MessageMoveState
{
   public:
   float Position1 = 0;
   float Position2 = 0;
   float Velocity1 = 1;
   float Velocity2 = 1;

template<int N_CHAN>
friend void operator<<(StateRotaryControl& Receiver, MessageMoveState<N_CHAN>& state)
{
  Receiver.Engine1.Position = state.Position1;
  Receiver.Engine2.Position = state.Position2;
  Receiver.Engine1.Velocity = state.Velocity1;
  Receiver.Engine2.Velocity = state.Velocity2;
};

};

template<int N_CHAN>
struct MessagePositionState
{
    public:
    uint16_t Position1;
    uint16_t Position2;
    uint16_t Sensor1;
    uint16_t Sensor2;
};

struct CommandCheckConnection  { uint8_t Connect  = 0xC5; uint8_t Connect2 = 0xC6; };
struct CommandCloseConnection  { uint8_t Close1   = 0xC6; uint8_t Close2   = 0xC6; };

struct CommandCalibration
{
    public:
    uint16_t  NodeType    = 0xA0;
    uint16_t  Command     = 0xA1;
    uint16_t  Channel     = 1;
    uint16_t  Amplitude   = 1;

    uint16_t  PeriodProcess = 1;
    uint16_t  TimeMeasure   = 1;
    uint16_t  NumberSteps   = 1;
    uint16_t  Reserve3      = 1;
};

using CommandDeviceController   = CommandDevice<0>;
using CommandDeviceLaserPointer = CommandDeviceRedux<0>;
using CommandDeviceLaserPower   = CommandDeviceRedux<1>;
using CommandDeviceFocusator    = CommandDeviceRedux<2>;

using MessageDeviceController   = MessageDevice<0>;
using MessageDeviceLaserPower   = MessageDevice<1>;
using MessageDeviceLaserPointer = MessageDevice<2>;
using MessageDeviceFocusator    = MessageDevice<3>;

using CommandSetPosRotary     = CommandSetPos<0>;
using CommandSetPosScanator   = CommandSetPos<1>;

using CommandSetSpeedRotary   = CommandSetSpeed<0>;
using CommandSetSpeedScanator = CommandSetSpeed<1>;

using MessageStateRotary      = MessageMoveState<0>;
using MessageStateScanator    = MessageMoveState<1>;

using MessagePosStateRotary   = MessagePositionState<0>;
using MessagePosStateScanator = MessagePositionState<1>;


#define LASER_CHECK 0x20
#define LASER_ON    0x22
#define LASER_OFF   0x26

#define LASER_SET_POWER 0x23
#define LASER_BEAM_ON   0x24
#define LASER_BEAM_OFF  0x25
#define LASER_PILOT_ON  0x27
#define LASER_PILOT_OFF 0x28

#define LASER_FAULT 0x39

#define LASER_MODULE       0 
#define LASER_MODULE_BEAM  1
#define LASER_MODULE_PILOT 2
#define LASER_MODULE_POWER 3

template<typename T> bool isAligned() { return sizeof(T) == T::getSize(); }