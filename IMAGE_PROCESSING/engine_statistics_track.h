#ifndef STATISTICS_TRACK
#define STATISTICS_TRACK

#include "interface_pass_coord.h"
#include "interface_pass_value.h"
#include <math.h>
#include <functional>
#include <QQueue>
#include <QDebug>
#include "debug_output_filter.h"

#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>
#include <Eigen/src/Core/Matrix.h>
#include "engine_statistics.h"
#include "thread_operation_nodes.h"
#include "nodes_track_approximation.h"
#include "module_period_measure.h"

template<typename T = float>
class NodeCoordAvarageStep: public PassCoordClass<T>
{
  public:
  NodeCoordAvarageStep(int size): Statistic(size) {};
  NodeCoordPassNop<T> NopNode;

  StatisticNode<T> Statistic;
  QPair<T,T> CoordLast{0,0};
  QPair<T,T> CoordStep{0,0};

	void setInput(const QPair<T,T>& Coord) override  
  {
    CoordStep = Coord - CoordLast; CoordLast = Coord;
    CoordStep >> Statistic; 
              if(Statistic.isLoaded()) PassCoordClass<T>::OutputCoord = Statistic.GetAvarageCoord();
  };

  PassCoordClass<T>& operator>>(PassCoordClass<T>& Receiver) override 
                         { if(!Statistic.isLoaded()) return NopNode; PassCoordClass<T>::OutputCoord >> Receiver; return Receiver; }

  QPair<T,T>& operator>>(QPair<T,T>& Receiver) override  
                         { if( Statistic.isLoaded()) Receiver = PassCoordClass<T>::OutputCoord; return Receiver; };
};

template<typename T = float>
class StatisticCoordSpan: public PassCoordClass<T>
{
  public:
    StatisticCoordSpan(int SizeWindow) 
    { 
      axis1.resize(SizeWindow);
      axis2.resize(SizeWindow);

      axis1_pos = axis1.begin(); axis2_pos = axis2.begin();
    };

    std::vector<T> axis1;
    std::vector<T> axis2;
    std::vector<T>::iterator axis1_pos; 
    std::vector<T>::iterator axis2_pos; 


    void getRange1() { std::sort(axis1.begin(), axis1.end()); Range1 = std::abs(axis1.back() - axis1.front()); }
    void getRange2() { std::sort(axis2.begin(), axis2.end()); Range2 = std::abs(axis2.back() - axis2.front()); }

    T sum1 = 0;
    T sum2 = 0;
    int counter = 0;

    void getRange1Avarage() 
    { 
      //std::sort(axis1.begin(), axis1.end());
      //sum1 = 0; sum2 = 0; counter = 0;

      //for(auto& val: axis1) {counter++; if(counter < axis1.size()) sum1 += val; else sum2 +=val; }
      //Range1 = sum2/(axis1.size()/2) - sum1/(axis1.size()/2); 
    }

    void getRange2Avarage() 
    { 
      //std::sort(axis1.begin(), axis1.end());
      //sum1 = 0; sum2 = 0; counter = 0;

      //for(auto& val: axis2) {counter++; if(counter < axis2.size()) sum1 += val; else sum2 +=val; }
      //Range2 = sum2/(axis2.size()/2) - sum1/(axis2.size()/2); 
    }

  NodeCoordAvarageStep<T> NodeAvarageStep{5};
  QPair<T,T> CoordSlowed{0,0};
  StatisticCoord<T> StatFast{50}; 
  StatisticCoord<T> StatLong{200}; 


  StatisticValue<T> Stat1_1{10}; 
  StatisticValue<T> Stat1_2{10}; 
  NodeCoordPassThinning<T> Thinning;
  NodeCoordPassValue<T> PickValue;
  NodeCoordSubstract<T> Substract;
  NodeCoordGain<T> Gain;
  NodeCoordGain<T> Inversion{-1};
  NodeCoordJoinValue<T> Join;

  std::pair<T,T> RangeStepMax;
  std::pair<T,T> RangeStepCurrent;
  std::pair<T,T> Scale;
  std::pair<T,T> RangeDispersion;

  T Range1 = 0;
  T Range2 = 0;

	const QPair<T,T>& getOutput() override { return PassCoordClass<T>::OutputCoord;};
	void setInput(const QPair<T,T>& Coord) override  
  {
    Coord >> NodeAvarageStep >> Gain(2) >> Substract; 
                                  Coord >> Substract >> Inversion >> CoordSlowed >> PickValue(0) >> Stat1_1; 
                                                                                    PickValue(1) >> Stat1_2;
    NodeAvarageStep >> StatLong; 
    NodeAvarageStep >> StatFast; 

     if(Stat1_1.isLoaded())
     {

      StatLong.GetMaxCoord()     >> Substract;
      StatFast.GetAvarageCoord() >> Substract >> RangeStepCurrent;
                                                 RangeStepMax  = StatLong.GetMaxCoord();

      RangeDispersion.first  = Stat1_1.GetRange(); Stat1_1.reset(); 
      RangeDispersion.second = Stat1_2.GetRange(); Stat1_2.reset();

                        Scale = (RangeStepCurrent/RangeStepMax)*0.3;// + 0.7;
      RangeDispersion = Scale*RangeDispersion; 

     qDebug() << "[ COORD SPANS ]" << RangeDispersion.first << RangeDispersion.second;

     //if(Stat1.isLoaded()) Stat1.reset();
     //if(Stat2.isLoaded()) Stat2.reset();
     //if(Stat3.isLoaded()) Stat3.reset();
     //if(Stat4.isLoaded()) Stat4.reset();
     }
  };
};

class DetectorTrackHold: public PassCoordClass<float>
{
  public:
    DetectorTrackHold() 
    { 
      Statistic1.setModeSingle(true);
      Statistic2.setModeSingle(true);
      Statistic3.setModeSingle(true);
      Statistic4.setModeSingle(true);
    };

    QPair<float,float> CoordProlong{0,0};
    QPair<float,float> CoordAvarage{0,0};

    NodeCoordVelocity<float> NodeVelocity;
    NodeCoordVelocity<float> NodeAcceleration;

    StatisticNode<float> Statistic1{10};  //COORD 
    StatisticNode<float> Statistic2{30}; //VELOCITY 
    StatisticNode<float> Statistic3{100}; //ACCELERATIOn
    StatisticNode<float> Statistic4{10};  //COORD RELATION


      NodeCoordSubstract<float> Substract;
    NodeCoordSplitToTime<float> SplitToTime; 

     NodeCoordRelation<float> NodeRelation;
         NodeCoordGain<float> Gain;
       NodeCoordOffset<float> Offset;
     NodeCoordAbsolute<float> Abs;
    NodeCoordPassValue<float> PickValue;
    NodeCoordJoinValue<float> JoinValue;

           MeasurePeriodNode MeasurePeriod;

    PolynomApproximation<3> trackApprox1{100};
    PolynomApproximation<3> trackApprox2{100};

    QPair<float,float> Velocity; 
    QPair<float,float> Acceleration; 

    float VelocityNorm = 0; 
    float AccelerationNorm = 0; 
    float DispersionCoord = 0; 

  bool isTrackHold() { return false; };

  double SignalThreshold     = 0.9; 
     int DispersionThreshold = 20; 

  void reset() { }

	const QPair<float,float>& getOutput() override { return OutputCoord;};
	void setInput(const QPair<float,float>& Coord) override  
  {
     Coord >> SplitToTime(0) >> trackApprox1 >>  PickValue(1) >> JoinValue >> CoordProlong; 
              SplitToTime(1) >> trackApprox2 >>  PickValue(1) >> JoinValue >> Substract;
                                                                     Coord >> Substract >> Statistic1;

     CoordProlong >> NodeVelocity >> Statistic2; 
                     NodeVelocity >> NodeAcceleration >> Statistic3;

      if(!Statistic1.isLoaded()) return;

       if(Statistic1.isLoaded()){ DispersionCoord = Statistic1.GetDispersionNorm(); 
                                     CoordAvarage = Statistic1.GetAvarageCoord();   Statistic1.reset(); }
       if(Statistic2.isLoaded()){        Velocity = Statistic2.GetAvarageCoord();                       }
       if(Statistic2.isLoaded()){    VelocityNorm = Statistic2.GetDispersionNorm();  Statistic2.reset();}
       if(Statistic3.isLoaded()){    Acceleration = Statistic3.GetAvarageCoord();                       }
       if(Statistic3.isLoaded()){AccelerationNorm = Statistic3.GetDispersionNorm();  Statistic3.reset();}

       qDebug() << OutputFilter::Filter(2) << "[ TRACK STAT ]" <<"[COORD DISP]" << DispersionCoord << "[ AVG ]" << CoordAvarage.first << CoordAvarage.second
                                            << "[VEL]"          << Velocity.first << Velocity.second
                                            << "[VEL_NORM]"     << VelocityNorm
                                            << "[ACCEL]"        << Acceleration.first << Acceleration.second
                                            << "[ACCEL_NORM]"   << AccelerationNorm;
  };
};

#endif