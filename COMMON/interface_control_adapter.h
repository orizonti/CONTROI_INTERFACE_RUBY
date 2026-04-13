#pragma once
#include "device_generic_interface.h"

template <class T>
class DeviceControlAdapter : public PassCoordClass<float>, public DeviceGenericHandleControl
{
public:
    DeviceControlAdapter(){};
    DeviceControlAdapter(T* Device,std::function<void(T&,QPair<float,float>)> SetFunction, std::function<QPair<float,float>(T&)> GetFunction)
    {linkAdapter(Device, SetFunction, GetFunction);};
    void linkAdapter(T* Device,std::function<void(T&,QPair<float,float>)> SetFunction, std::function<QPair<float,float>(T&)> GetFunction)
    { InputFunction = SetFunction; OutputFunction = GetFunction; DeviceLink = Device; };

    T* DeviceLink =  nullptr;
    std::pair<float,float> PosRotary{0,0};

    std::function<void(T&,QPair<float,float>)>  InputFunction = nullptr;
         std::function<QPair<float,float>(T&)> OutputFunction = nullptr;

    void setInput(const QPair<float,float>& coord) { if(InputFunction != nullptr) InputFunction(*DeviceLink,coord);};
    const QPair<float,float>& getOutput() { if(OutputFunction != nullptr) PassCoordClass<float>::OutputCoord = OutputFunction(*DeviceLink); 
                                                                   return PassCoordClass<float>::OutputCoord;};

                      //HANDLE CONTROL INTERFACE
	                    void setEnable(bool OnOff, uint16_t Number = 0) { DeviceLink->setEnable(OnOff,Number); };
	                    void setPair(std::pair<float,float> Coord) { PosRotary = Coord; setInput(PosRotary); };
	  std::pair<float,float> getPair() { return getOutput(); };
    void setParam (uint16_t CommandID, uint32_t CommandParam) {};
    void setParam (uint16_t CommandID, float    CommandParam) 
    {
      if(CommandID == 1) { PosRotary.second = CommandParam; }
      else PosRotary.first = CommandParam;   setInput(PosRotary);
    };

    float getParam(uint16_t CommandID) { return DeviceLink->getParam(CommandID); }
};