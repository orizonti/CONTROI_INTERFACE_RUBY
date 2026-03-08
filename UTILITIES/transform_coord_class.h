#ifndef TRANSFORMCOORDCLASS_H
#define TRANSFORMCOORDCLASS_H

#include "interface_pass_coord.h"
#include <QPair>
#include <functional>


class  TransformCoordClass : public PassCoordClass<float>
{
public:
    TransformCoordClass() {};
    TransformCoordClass(float Scale1, float Scale2, float Offset1 = 0, float Offset2 = 0, float Saturation = 20000)
	{
		SetParam(Scale1, Scale2, Offset1, Offset2, Saturation);
	}
	std::function<void(QPair<float,float>)> TransformCoord;

    const QPair<float,float>& getOutput() { return Output; }
    void setInput(const QPair<float,float>& Coord) { TransformCoord(Coord); }

	QPair<float,float> Output;

	float Scale1 = 1;
	float Scale2 = 1;
	float Offset1 = 0;
	float Offset2 = 0;

	float Saturation = 20000;
	TransformCoordClass& operator()(float ScaleParam1, float ScaleParam2) {Scale1 = ScaleParam1; 
		                                                                   Scale2 = ScaleParam2;
																					return *this;}
	void SetParam(float ScaleParam1, float ScaleParam2, float OffsetParam1 = 0, float OffsetParam2 = 0, float Saturation = 20000)
	{
	Scale1 = ScaleParam1;
	Scale2 = ScaleParam2;
	Offset1 = OffsetParam1;
	Offset2 = OffsetParam2;

	TransformCoord = [this](QPair<float,float> CoordError)
	{
		Output.first  = (CoordError.first  + Offset1) * Scale1;  
		Output.second = (CoordError.second + Offset2) * Scale2; 

		if(std::abs(Output.first) > this->Saturation) Output.first = this->Saturation*Output.first/std::abs(Output.first);
		if(std::abs(Output.second) > this->Saturation) Output.second = this->Saturation*Output.second/std::abs(Output.second);

	};
	}
};




#endif //TRANSFORMCOORDCLASS_H
