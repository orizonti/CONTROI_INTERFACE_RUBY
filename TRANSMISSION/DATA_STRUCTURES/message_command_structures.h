#pragma once
#include <stdint.h>
#include <string>
#include <sstream>
#include <QDataStream>
#include <QIODevice>
#include <QByteArray>
#include <QDebug>

template<int NUM_DEV>
struct CommandDevice
{
  public:
  uint16_t DeviceID = 0;
  uint16_t Command = 0;
  uint16_t Param = 0;
  void clearStruct() { std::memset(this,0,sizeof(CommandDevice)); }
};

using CommandDeviceController   = CommandDevice<0>;

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
  void clearStruct() { std::memset(this,0,sizeof(CommandDevice)); }
};

using CommandDeviceLaserPointer = CommandDeviceRedux<0>;
using CommandDeviceLaserPower   = CommandDeviceRedux<1>;
using CommandDeviceFocusator    = CommandDeviceRedux<2>;

template<int NUM_DEV>
struct MessageDevice
{
   uint8_t DeviceID = 0;
   uint8_t Module = 0;
   uint16_t Param1 = 0;
   uint16_t Param2 = 0;
   uint16_t Param3 = 0;
};

using MessageDeviceController   = MessageDevice<0>;
using MessageDeviceLaserPower   = MessageDevice<1>;
using MessageDeviceLaserPointer = MessageDevice<2>;
using MessageDeviceFocusator    = MessageDevice<3>;

template<int N_CHAN>
struct CommandSetPos
{
   public:
   int16_t Param1 = 0;
   int16_t Param2 = 0;
   void clearStruct() { std::memset(this,0,sizeof(CommandSetPos)); }
   template<typename T>
   void operator=(const QPair<T,T>& Pos) { Param1 = Pos.first; Param2 = Pos.second; };
};
using CommandSetPosRotary   = CommandSetPos<0>;
using CommandSetPosScanator = CommandSetPos<1>;

template<int N_CHAN>
struct CommandSetSpeed
{
   public:
   int16_t Param1 = 0;
   int16_t Param2 = 0;
   void clearStruct() { std::memset(this,0,sizeof(CommandSetSpeed)); }
};
using CommandSetSpeedRotary   = CommandSetSpeed<0>;
using CommandSetSpeedScanator = CommandSetSpeed<1>;

template<int N_CHAN>
struct MessageMoveState
{
   public:
   int16_t Param1 = 0;
   int16_t Param2 = 0;
   uint16_t Param3 = 1;
   uint16_t Param4 = 1;
};

using MessageStateRotary   = MessageMoveState<0>;
using MessageStateScanator = MessageMoveState<1>;

template<int N_CHAN>
struct MessagePositionState
{
    public:
    uint16_t Param1;
    uint16_t Param2;
    uint16_t Param3;
    uint16_t Param4;
};

using MessagePosStateRotary   = MessagePositionState<0>;
using MessagePosStateScanator = MessagePositionState<1>;

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

#include "arduino_json.h"

class CommandJson
{
  public:
  JsonDocument command;
   std::string json_doc;
    QByteArray array;

  QByteArray toByteArray() { return array;};

};

class CommandSetPosJson : public CommandJson
{
  public:
  JsonDocument slave1;
  JsonDocument slave2;

  CommandSetPosJson()
  {
   command["id"] = 123;
   command["dtg"] = 1111;

   slave1["name"] = "joint1"; slave1["mode"] = "position"; slave1["class"] = "TDrive"; slave1["target"] = 0;
   slave2["name"] = "joint2"; slave2["mode"] = "position"; slave2["class"] = "TDrive"; slave2["target"] = 0;

   command["slaves"][0] = slave1;
   command["slaves"][1] = slave2;

    size_t size = serializeJson(command,json_doc); array.setRawData(json_doc.c_str(), size);
  }

  template<typename T> void setData(const QPair<T,T>& Pos)
  {
    slave1["target"] = Pos.first; 
    slave2["target"] = Pos.second;

    command["slaves"][0] = slave1; command["slaves"][1] = slave2;

    size_t size = serializeJson(command,json_doc);
                       array.setRawData(json_doc.c_str(), size);
  }

  template<typename T> void operator=(const QPair<T,T>& Pos)
  {
    setData(Pos);
  };

  QByteArray toByteArray() { return array;};

};

class CommandLidControlJson : public CommandJson
{
  public:
  JsonDocument command_settings;

  CommandLidControlJson(QString setting_str = "value: close, pin: 1, down: 530, top: 1750, step: 2, interval: 0.001")
  {
       QStringList settings_list = setting_str.split(",");
   for(auto setting_str: settings_list)
   {
                  auto setting_pair = setting_str.split(":");
      command_settings[setting_pair[0].trimmed().toStdString()] = setting_pair[1].trimmed().toStdString(); 
   }
                            command["unit"] = "servo";
                            command["settings"] = command_settings;
   size_t size = serializeJson(command,json_doc); 
                      array.setRawData(json_doc.c_str(), size); 

  }
  QByteArray toByteArray() { return array;};
  void printCommand() { qDebug() << "[ COMMAND LID ] " << json_doc.c_str(); }

};


struct TimeStamp
{
  uint32_t sec = 0xFFFFFFFF;  // количество секунд прошедшее с момента запуска
  uint32_t nsec =0xAAAAAAAA; // количество наносекунд прошедшее с последней секунды
};

struct SystemState
{
  TimeStamp ts;     // штамп времени
  uint16_t magic;   // идентификатор пакета
  uint16_t reserve; // резервное поле данных
   uint8_t version; // версия протокола
   uint8_t state;   // байт состояния системы
   uint8_t drives;  // количество приводов
   uint8_t temp;    // температура вычислителя в градусах Цельсия
   uint8_t temp1_0; // температура приводов в градусах Цельсия
   uint8_t temp1_1; // температура приводов в градусах Цельсия
   uint8_t temp2_0; // температура оптических клиньев в градусах Цельсия
   uint8_t temp2_1; // температура оптических клиньев в градусах Цельсия
};

struct DriveRX
{
   uint8_t status;   // статус
   uint8_t mode;     // режим управление
   uint8_t reserve;  // резервное поле данных
   uint8_t digital;  // значение на цифровых входах
    double position; // угловое положение в градусах
    double velocity; // угловая скорость в градус/сек
     float current;  // статорный ток на оси q системы координат Парка в амперах
     float voltage;  // напряжение цепи звена постоянного тока в вольтах
  uint16_t analog1;  // значение на входе АЦП 1
  uint16_t analog2;  // значение на входе АЦП 2

  friend void operator>>(QDataStream& stream, DriveRX& rec)
  {
    stream >> rec.status   >> rec.mode     >> rec.reserve >> rec.digital 
           >> rec.position >> rec.velocity >> rec.current >> rec.voltage >> rec.analog1 >> rec.analog2; 
  }
};

class ControlRX
{
  public:
  TimeStamp ts;     // штамп времени
  uint16_t magic;   // идентификатор пакета
  uint16_t reserve; // резервное поле данных
  DriveRX value0;   // массив данных, кол-во элементов равно количеству приводов
  DriveRX value1;   // массив данных, кол-во элементов равно количеству приводов
};

struct DriveTX
{
   uint8_t mode    = 0xFF;     // целевой режим управление
   uint8_t digital = 0xFF;  // значение на цифровых выходах
  uint16_t reserve = 0xFF;  // резервное поле данных
  uint32_t acc     = 0xCCCCCCCC;      // лимит по ускорению в градус/сек^2
    double target;   // задание в зависимости от режима управления

  friend QDataStream& operator>>(QDataStream& stream, DriveTX& rec)
  {
    stream >> rec.mode >> rec.digital >> rec.reserve >> rec.acc >> rec.target; 
    return stream;
  }
};

class ControlTX
{
  public:
  TimeStamp ts;    // штамп времени
  uint16_t magic = 0;  // идентификатор пакета
  uint16_t reserve = 0;// резервное поле данных
   DriveTX value0; // массив данных, кол-во элементов равно количеству приводов
   DriveTX value1; // массив данных, кол-во элементов равно количеству приводов
   QByteArray toByteArray() { return QByteArray((char*)this, sizeof(ControlTX));}

  friend void operator>>(QDataStream& stream, ControlTX& rec)
  {
    stream >> rec.ts.nsec >> rec.ts.sec >> rec.magic >> rec.reserve >> rec.value0 >> rec.value1; 
  }

  template<typename T>
  void operator=(QPair<T,T> Pos) { value0.target = Pos.first; value1.target = Pos.second; };

  template<typename T>
  void setData(QPair<T,T> Pos) { value0.target = Pos.first; value1.target = Pos.second; };
  
};


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

