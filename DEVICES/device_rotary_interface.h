#ifndef SCANATOR_CONTROL_CLASS_H
#define SCANATOR_CONTROL_CLASS_H

//#include "RotateOperationContainer.h"
#include "interface_pass_coord.h"
#include "thread_operation_nodes.h"
#include "rotate_vector_class.h"
#include "state_block_enum.h"
#include "transform_coord_class.h"

#include <QSettings>
#include <QTimer>
#include "engine_udp_interface.h"
#include "engine_tcp_interface.h"
#include "message_command_structures.h"
#include "message_struct_generic_ext.h"
#include "message_header_generic_ext.h"
#include "device_generic_interface.h"
#include "debug_output_filter.h"
#include "widget_rotary_platform_control.h"
#include "transform_coord_class.h"

class DynamicModule: public QObject
{
  Q_OBJECT
  public:
  explicit DynamicModule(DeviceRotaryInterface* DeviceRotary) 
  { 
    Device = DeviceRotary; QObject::connect(&timerMove, &QTimer::timeout, this, &DynamicModule::slotMove); 
  };
 
  QPair<int,int> Velocity;
  void moveWithVelocity(QPair<int,int> Value) 
  { 
    Velocity = Value; if(timerMove.isActive()) return; timerMove.start(1); 
  }
  void stopMove() { timerMove.stop(); }

  public slots:
  void slotMove() { Device->moveOnStep(Velocity); }
  private:
  QTimer timerMove;
  DeviceRotaryInterface* Device = nullptr;
};

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
class DeviceRotaryControl : public DeviceGenericInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>, public DeviceRotaryInterface
{
public:
  using DEVICE_BASE_TYPE = DeviceGenericInterface<T_CONNECTION, T_COMMAND, T_MESSAGE>; 
  DeviceRotaryControl(std::shared_ptr<T_CONNECTION> Connection, QString Name = "[ DEVICE ]");

  GainNode<float>   Gain{1,1};
  OffsetNode<float> Offset{1,1};
  RotateVectorClass<float> Rotation;

	QString TAG_NAME{"[ ROTARY ]"};

	~DeviceRotaryControl() { qDebug() << TAG_NAME << "DELETE"; }
  //================================================
  QPair<int, int> LimitDown{-10000,-10000};
	QPair<int, int> LimitUp  { 10000, 10000};
	QPair<int, int> Range{LimitUp - LimitDown};

	QPair<int, int> PositionNull   {0,0};
	QPair<int, int> Position       {0,0};
	QPair<int, int> PositionTarget {0,0};

  //================================================
  QPair<int, int> LimitVelocityDown{-10000,-10000};
	QPair<int, int> LimitVelocityUp  { 10000, 10000};
	QPair<int, int> RangeVelocity{LimitVelocityUp - LimitVelocityDown};

	QPair<int, int> Velocity       {0,0};
	QPair<int, int> VelocityTarget {0,0};
  //================================================

	QPair<float, float> PositionDevice{0,0};
	QPair<float, float> VelocityDevice{0,0};

	void moveToPos(const QPair<int, int>& Pos)         override;
	void moveToPosRelative(const QPair<int, int>& Pos) override;
	void moveOnStep(const QPair<int, int>& Pos)        override;
	void moveWithVelocity(const QPair<int, int>& VelocityVector) override; 
	void moveWithVelocityManual(const QPair<int, int>& Vel) override { ModuleMoveVelocity.moveWithVelocity(Vel);}; 
	void stopMove()  override { ModuleMoveVelocity.stopMove(); }
  //===============================================================================================

	const QPair<int,int>&     getPos()            { return Position;      } 
	const QPair<float,float>& getPosDevice()      { return PositionDevice;}
	const QPair<int,int>&     getVelocity()       { return Velocity;      };
	const QPair<int,int>&     getVelocityDevice() { return Velocity;      };

  bool isAtLimit();
  std::function<void (QPair<int,int>&)> checkPositionOffset;
  std::function<void (QPair<int,int>&)> checkVelocityOffset;

	QPair<int,int> getLimits(int axis) override;
	QPair<int,int> getRange()  override { return Range; } 

  //===============================================================================================
	void setParam(uint8_t ID, uint16_t Value) override {};

  void putMessage(T_MESSAGE Message) override {}; 
  //===============================================================================================

	void setToNull();
  void loadSettings();
  std::shared_ptr<PortAdapter<DeviceRotaryInterface>> PortMoveRelative = nullptr;

	DynamicModule ModuleMoveVelocity{this};
private:
	RotateVectorClass<int>   RotAxis;
  std::vector<WidgetRotaryPlatformControl*> ControlWindows;
};

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
DeviceRotaryControl<T_CONNECTION, T_COMMAND, T_MESSAGE>::DeviceRotaryControl(std::shared_ptr<T_CONNECTION> Connection, QString Name): 
DeviceGenericInterface<T_CONNECTION,T_COMMAND, T_MESSAGE>(Connection, Name)
{
	setToNull();

  PortMoveRelative = std::make_shared<PortAdapter<DeviceRotaryInterface>>();
  PortMoveRelative->LinkAdapter(this, &DeviceRotaryInterface::moveToPosRelative, 
                                      &DeviceRotaryInterface::getPos);


  auto Offset = QPair<int,int>(0,0);
  checkVelocityOffset = [this, Offset](QPair<int,int>& Vector) mutable 
  {
    Offset = abs_pair(Vector - LimitDown) + abs_pair(LimitUp - Vector);
       
    if(Offset.first  > Range.first  ) Vector.first  = Vector.first  < 0 ? LimitDown.first  : LimitUp.first; 
    if(Offset.second > Range.second ) Vector.second = Vector.second < 0 ? LimitDown.second : LimitUp.second; 
  };


  checkPositionOffset = [this, Offset](QPair<int,int>& Vector) mutable 
  {
    Offset = abs_pair(Vector - LimitVelocityDown) + abs_pair(LimitVelocityUp - Vector);
       
    if(Offset.first  > Range.first  ) Vector.first  = Vector.first  < 0 ? LimitVelocityDown.first  : LimitUp.first; 
    if(Offset.second > Range.second ) Vector.second = Vector.second < 0 ? LimitVelocityDown.second : LimitUp.second; 
  };

}
template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
QPair<int,int> DeviceRotaryControl<T_CONNECTION,T_COMMAND,T_MESSAGE>::getLimits(int axis)
{
   QPair<int,int> Limits(LimitDown.first, LimitUp.first);
    if(axis == 1) Limits = QPair<int,int>(LimitDown.second, LimitUp.second);
    return Limits;
}

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
bool DeviceRotaryControl<T_CONNECTION,T_COMMAND,T_MESSAGE>::isAtLimit() 
{ 
    auto Offset = abs_pair(Position - LimitVelocityDown) + abs_pair(LimitVelocityUp - Position);
       
    if(Offset.first  > Range.first  ) Position.first  = Position.first  < 0 ? LimitVelocityDown.first  : LimitUp.first; 
    if(Offset.second > Range.second ) Position.second = Position.second < 0 ? LimitVelocityDown.second : LimitUp.second; 

  return false; 
}


template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
void DeviceRotaryControl<T_CONNECTION,T_COMMAND,T_MESSAGE>::setToNull() 
{ 
                                   Position = QPair<float,float>(0,0);
                                   PositionDevice = Position + PositionNull; 
                 this->sendCommand(PositionDevice);
  qDebug() << "SET TO NULL: " << PositionDevice.first << PositionDevice.second;
}


template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
void DeviceRotaryControl<T_CONNECTION,T_COMMAND,T_MESSAGE>::moveOnStep(const QPair<int, int>& Step)
{
         PositionTarget = Position + Step; checkPositionOffset(PositionTarget);
                                                    Position = PositionTarget;
                                   PositionDevice = Position + PositionNull;
                                   PositionDevice >> Offset >> Gain >> PositionDevice;
                 this->sendCommand(PositionDevice);
  qDebug() << "MOVE STEP TO POS: " << PositionDevice.first << PositionDevice.second << "STEP: " << Step.first;
}


template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
void DeviceRotaryControl<T_CONNECTION,T_COMMAND,T_MESSAGE>::moveWithVelocity(const QPair<int, int>& Velocity)
{
                                   VelocityTarget = Velocity;
                 this->sendCommand(VelocityTarget);
  qDebug() << "MOVE WITH VELOCITY: " << PositionDevice.first << PositionDevice.second;
}

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
void DeviceRotaryControl<T_CONNECTION,T_COMMAND,T_MESSAGE>::moveToPos(const QPair<int, int>& Pos)
{
    PositionTarget = Pos; checkPositionOffset(PositionTarget);
                                   Position = PositionTarget;
                                   PositionDevice = Position + PositionNull;
                                   PositionDevice >> Offset >> Gain >> PositionDevice;
                 this->sendCommand(PositionDevice);
    qDebug() << "MOVE TO POS: " << PositionDevice.first << PositionDevice.second;
}

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
void DeviceRotaryControl<T_CONNECTION,T_COMMAND,T_MESSAGE>::moveToPosRelative(const QPair<int, int>& PosRelative) 
{
                                    auto PositionAbs = Position + PosRelative;
                     checkPositionOffset(PositionAbs);
                        PositionTarget = PositionAbs; 

                                   PositionDevice = PositionTarget + PositionNull;
                                   PositionDevice >> Offset >> Gain >> PositionDevice;
                 this->sendCommand(PositionDevice);
}

//================================================================================================================

template<typename T_CONNECTION, typename T_COMMAND, typename T_MESSAGE>
void DeviceRotaryControl<T_CONNECTION,T_COMMAND,T_MESSAGE>::loadSettings()
{
  //auto RotateParamCamEng = SettingsRegister::GetString(QString("ROTATE_CAM_ENG"));
  //auto RotateParamEngCam = SettingsRegister::GetString(QString("ROTATE_ENG_CAM"));
  //this->RotEngineToCamera.LoadRotationFromFile(RotateParamEngCam);
}


#endif 
