#pragma once
#include <stdint.h>
#include <string>
#include <sstream>
#include <QDataStream>
#include <QIODevice>
#include <QByteArray>
#include <QDebug>

struct MessageCommand 
{ 
  uint16_t NodeType = 0; 
  uint16_t Command = 0; 
};

struct CommandRotaryStruct
{
   uint16_t COMMAND = 1;
   int16_t SPEED_X = 1;
   int16_t SPEED_Y = 1;
   int16_t POS_X = 0;
   int16_t POS_Y = 0;
   uint16_t RESERVE = 0;
}; 

struct AimStateStruct
{
    uint16_t Position = 20; 
    uint16_t Velocity = 20;
    uint16_t Acceleration = 20;
    uint16_t PositionRel = 20;
};

struct ScanatorControlStruct
{
    AimStateStruct StateChannel1;
    AimStateStruct StateChannel2;
};

struct CHECK_CONNECTION_REQUEST  { uint8_t Connect  = 0xC5; uint8_t Connect2 = 0xC6; };
struct CLOSE_CONNECTION_REQUEST  { uint8_t Close1   = 0xC6; uint8_t Close2   = 0xC6; }; 


