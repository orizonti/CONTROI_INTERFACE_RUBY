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

template<typename T = float, int type=2>
class StatisticDispersionSpan: public PassCoordClass<T>
{
  public:
  StatisticDispersionSpan(int SizeWindow) { };

  QPair<T,T> CoordSlowed{0,0};

  NodeCoordAvarageStep<T> NodeAvarageStep{5};
  StatisticCoord<T> StatFast {3}; 
  StatisticCoord<T> StatLong {300}; 
  StatisticCoord<T> StatFast2{20}; 
  StatisticCoord<T> StatLong2{50}; 
  StatisticCoord<T> StatTest {10}; 
  StatisticCoord<T> StatTest2{20}; 

  StatisticValue<T> Stat1_1{10}; 
  StatisticValue<T> Stat1_2{10}; 

  NodeCoordPassThinning<T> Thinning;
     NodeCoordPassValue<T> PickValue;
     NodeCoordDifference<T> Substract;

  NodeCoordGain<T> Gain;
  NodeCoordGain<T> Inversion{-1};

  NodeCoordJoinValue<T> Join;
   NodeCoordAbsolute<T> Abs;
   NodeCoordMultiply<T> Multiply;

  std::pair<T,T> RangeStepMax;
  std::pair<T,T> RangeStepCurrent;
  std::pair<T,T> Scale;
  std::pair<T,T> RangeDispersion;

  std::pair<T,T> Step;
  float gain = 0.9;

	const QPair<T,T>& getOutput() override { return PassCoordClass<T>::OutputCoord;};
	void setInput(const QPair<T,T>& Coord) override  
  {
     Coord >> NodeAvarageStep >> Substract;       
                        Coord >> Substract >> CoordSlowed >> PickValue(0) >> Stat1_1; 
                                              CoordSlowed >> PickValue(1) >> Stat1_2;
     //qDebug() << "COORD SLOWED: " << CoordSlowed.first << CoordSlowed.second;
     NodeAvarageStep >> Abs >> StatLong; 
     NodeAvarageStep >> Abs >> StatFast; 

     if(Stat1_1.isLoaded())
     {

      StatFast.GetAvarageCoord() >> Substract;
      StatLong.GetMaxCoord()     >> Substract >> RangeStepCurrent;                   
                                                 RangeStepMax  = StatLong.GetRange(); StatLong.resetOnLoaded();
      Step = StatFast.GetAvarageCoord();

      RangeDispersion.first  = Stat1_1.GetRange(); Stat1_1.reset(); 
      RangeDispersion.second = Stat1_2.GetRange(); Stat1_2.reset();

                        Scale = (RangeStepCurrent/RangeStepMax)*gain + (1-gain);
      RangeDispersion = Scale*RangeDispersion; 
      RangeDispersion >> StatLong2;
      RangeDispersion = StatLong2.GetMaxCoord(); 

      if(StatLong2.isLoaded()) qDebug() << "[ COORD SPAN ]" <<  RangeDispersion.first << RangeDispersion.second;
         StatLong2.resetOnLoaded();
     }
  };
};

template<typename T>
class StatisticDispersionSpan<T,1>: public PassValueClass<T>
{
  public:
  StatisticDispersionSpan(int SizeWindow) { };

  T ValueSlowed{0};

  NodeValueAvarageStep<T> NodeAvarageStep{5};
  StatisticValue<T> StatFast {3}; 
  StatisticValue<T> StatLong {300}; 
  StatisticValue<T> StatFast2{20}; 
  StatisticValue<T> StatLong2{50}; 
  StatisticValue<T> StatTest {10}; 
  StatisticValue<T> StatTest2{20}; 

  StatisticValue<T> StatFast3{10}; 

  NodeValueDifference<T> Substract;
  NodeValueInversion<T> Inversion;

  NodeValueAbsolutization<T> Abs;
  NodeValueMultiply<T> Multiply;

  T RangeStepMax;
  T RangeStepCurrent;
  T Scale;
  T RangeDispersion;
  T Step;
  float gain = 0.9;
  T OutputValue;
  T ValueTest;

	void setValue(T Value ) override  
  {
     Value >> NodeAvarageStep >> Substract;       
                        Value >> Substract >> ValueSlowed >> StatFast3; 

     //qDebug() << "[ CALC SPAN ]" << Value << " SLOWED: " << ValueSlowed;
     NodeAvarageStep >> Abs >> StatLong; 
     NodeAvarageStep >> Abs >> StatFast; 

     if(StatFast3.isLoaded())
     {

      StatFast.GetAvarageValue() >> Substract ;
      StatLong.GetMaxValue()     >> Substract >> RangeStepCurrent;                   
                                                 RangeStepMax  = StatLong.GetRange(); StatLong.resetOnLoaded();
      Step = StatFast.GetAvarageValue();

      RangeDispersion  = StatFast3.GetRange(); StatFast3.reset(); 

                        Scale = (RangeStepCurrent/RangeStepMax)*gain + (1-gain);
      RangeDispersion = Scale*RangeDispersion; 
      RangeDispersion >> StatLong2;
      RangeDispersion = StatLong2.GetMaxValue(); 

      if(StatLong2.isLoaded()) qDebug() << "[ COORD SPAN ]" <<  RangeDispersion;
         StatLong2.resetOnLoaded();
     }
  };
};

template<typename T = float>
class EstimatorObjectVelocity: public PassCoordClass<T>
{
  public:
   EstimatorObjectVelocity() { }

	void setInput(const QPair<T,T>& Coord) override  
  {

    Coord >> Thinning1(2)  >> NodeAvarageStep1 >> Norm >> Velocity1 >> Categorizer1;
    Coord >> Thinning2(6)  >> NodeAvarageStep2 >> Norm >> Velocity2 >> Categorizer2;
    Coord >> Thinning3(12) >> NodeAvarageStep3 >> Norm >> Velocity3 >> Categorizer3;

    qDebug() << OutputFilter::Filter(20) << "[ VELOCITY ESTIMATOR ]" << Velocity1 << Velocity2 << Velocity3 
                                         << "[CAT]" << Categorizer1.getValue() 
                                                    << Categorizer2.getValue()
                                                    << Categorizer3.getValue();
  }
    T Velocity1{0};
    T Velocity2{0};
    T Velocity3{0};

    NodeCoordDifference<T> Substract;
    NodeCoordPassNorm<T> Norm;
    NodeValueAbsolutization<T> Abs;

    NodeCoordPassThinning<T> Thinning1{2};
    NodeCoordPassThinning<T> Thinning2{4};
    NodeCoordPassThinning<T> Thinning3{10};

    NodeValueCategorizer<T> Categorizer1{10,10};
    NodeValueCategorizer<T> Categorizer2{50,10};
    NodeValueCategorizer<T> Categorizer3{100,10};

    NodeCoordAvarageStep<T> NodeAvarageStep1{10};
    NodeCoordAvarageStep<T> NodeAvarageStep2{10};
    NodeCoordAvarageStep<T> NodeAvarageStep3{10};
};

class StatisticMovingParam: public PassCoordClass<float>
{
  public:
    StatisticMovingParam() 
    { 
      Statistic1.setModeSingle(true);
      Statistic2.setModeSingle(true);
      Statistic3.setModeSingle(true);
      Statistic4.setModeSingle(true);
    };

    QPair<float,float> CoordProlong{0,0};
    QPair<float,float> CoordAvarage{0,0};

    StatisticCoord<float> Statistic1{10};  
    StatisticCoord<float> Statistic2{30}; 
    StatisticCoord<float> Statistic3{100}; 
    StatisticCoord<float> Statistic4{10};  
    StatisticDispersionSpan<float,1> StatisticSpan{10};

      NodeCoordDifference<float> Substract;
        NodeCoordAbsolute<float> Abs;
        NodeCoordPassNorm<float> Norm;
       NodeCoordPassValue<float> PickValue;
       NodeCoordJoinValue<float> JoinValue;
     NodeCoordSplitToTime<float> SplitToTime; 

    NodeCoordVelocity<float> NodeVelocity;
    NodeCoordVelocity<float> NodeAcceleration;

    PolynomApproximation<3> trackApprox1{100,10,2};
    PolynomApproximation<3> trackApprox2{100,10,2};
               MeasurePeriodNode MeasurePeriod;

    QPair<float,float> Velocity; 
    QPair<float,float> Acceleration; 
                float  VelocityNorm = 0; 
                float  AccelerationNorm = 0; 
                float  DispersionCoord = 0; 

  void reset() { }

	const QPair<float,float>& getOutput() override { return OutputCoord;};

	void setInput(const QPair<float,float>& Coord) override  
  {
     Coord >> SplitToTime(0) >> trackApprox1 >>  PickValue(1) >> JoinValue >> CoordProlong; 
              SplitToTime(1) >> trackApprox2 >>  PickValue(1) >> JoinValue >> Substract;
                                                                     Coord >> Substract >> Statistic1;
     Coord >> Norm >> StatisticSpan;

     CoordProlong >> NodeVelocity >> Statistic2; 
                     NodeVelocity >> NodeAcceleration >> Statistic3;

      if(!Statistic1.isLoaded()) return;

       if(Statistic1.isLoaded()){ DispersionCoord = 0.5*Statistic1.GetDispersionNorm() + 0.5*StatisticSpan.getValue(); 
                                     CoordAvarage = Statistic1.GetAvarageCoord();   Statistic1.reset(); }
       if(Statistic2.isLoaded()){        Velocity = Statistic2.GetAvarageCoord();                       }
       if(Statistic2.isLoaded()){    VelocityNorm = Statistic2.GetDispersionNorm();  Statistic2.reset();}
       if(Statistic3.isLoaded()){    Acceleration = Statistic3.GetAvarageCoord();                       }
       if(Statistic3.isLoaded()){AccelerationNorm = Statistic3.GetDispersionNorm();  Statistic3.reset();}

       //qDebug() << OutputFilter::Filter(2) << "[ TRACK STAT ]" <<"[COORD DISP]" << DispersionCoord;
       qDebug() << "[ TRACK STAT ]" <<"[COORD DISP]" << DispersionCoord;
                                            //<< "[AVG]" << CoordAvarage.first << CoordAvarage.second
                                            //<< "[VEL]"          << Velocity.first << Velocity.second
                                            //<< "[VEL_NORM]"     << VelocityNorm
                                            //<< "[ACCEL]"        << Acceleration.first << Acceleration.second
                                            //<< "[ACCEL_NORM]"   << AccelerationNorm;
  };
};

#endif