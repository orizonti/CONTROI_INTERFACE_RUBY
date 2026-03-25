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

    QPair<float,float> LastCoord{0,0};

    QPair<float,float> CoordTime1{0,0};
    QPair<float,float> CoordTime2{0,0};

    QPair<float,float> CoordTime1Prolong{0,0};
    QPair<float,float> CoordTime2Prolong{0,0};
    QPair<float,float> CoordProlong{0,0};

    NodeCoordTimeDifference<float> NodeDiff;
    NodeCoordVelocity<float> NodeVelocity;
    NodeCoordVelocity<float> NodeAcceleration;

    StatisticNode<float> Statistic1{10};  //COORD 
    StatisticNode<float> Statistic2{30}; //VELOCITY 
    StatisticNode<float> Statistic3{100}; //ACCELERATIOn
    StatisticNode<float> Statistic4{10};  //COORD RELATION

    NodeValueVelocity<float> NodeVelocityValue;
    NodeCoordRelation<float> NodeRelation;
    NodeCoordGain<float> Gain;
    NodeCoordOffset<float> Offset;
    NodeCoordAbsolute<float> Abs;
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
     //Coord >> Statistic1; 
     MeasurePeriod++;
     CoordTime1.first = Coord.first; CoordTime1.second = MeasurePeriod.getMicroseconds();
     CoordTime2.first = Coord.second;CoordTime2.second = MeasurePeriod.getMicroseconds();
     CoordTime1 >> trackApprox1 >> CoordTime1Prolong;
     CoordTime2 >> trackApprox2 >> CoordTime2Prolong;
     //qDebug() << "COORD TIME: " << CoordTime1 <<"PROLONG: " << CoordTime1Prolong;
     CoordProlong.first = CoordTime1Prolong.first;
     CoordProlong.second = CoordTime2Prolong.first;

     Coord >> NodeDiff; //>> NodeAcceleration; 
              NodeDiff >> Gain(100) >> Offset(0,200) >> Abs >> Statistic2;         
          //NodeAcceleration >> Statistic3;     

       PassCoordClass<float>::OutputCoord  = CoordProlong;
       PassCoordClass<float>::passCoord();

      if(!Statistic2.isLoaded()) return;

       if(Statistic1.isLoaded()){ DispersionCoord = Statistic1.GetDispersionNorm(); Statistic1.reset(); }
       if(Statistic2.isLoaded()){        Velocity = Statistic2.GetAvarageCoord();                       }
       if(Statistic2.isLoaded()){    VelocityNorm = Statistic2.GetDispersionNorm();  Statistic2.reset();}
       if(Statistic3.isLoaded()){    Acceleration = Statistic3.GetAvarageCoord();                       }
       if(Statistic3.isLoaded()){AccelerationNorm = Statistic3.GetDispersionNorm();  Statistic3.reset();}

       //qDebug() <<"[ TRACK STAT ]" <<"[COORD DISP]" << DispersionCoord << Statistic1.GetAvarageCoord()
       //                            <<"[VEL]"        << Velocity
       //                            <<"[VEL_NORM]"   << VelocityNorm
       //                            <<"[ACCEL]"      << Acceleration
       //                            <<"[ACCEL_NORM]" << AccelerationNorm;
  };
};

class StatisticCoordSpan2: public PassCoordClass<float>
{
  public:
    StatisticCoordSpan2(int SizeWindow) 
    { 
      track.resize(SizeWindow);
      pos_track = track.begin();
    };

    std::vector<std::pair<float,float>> track;
    std::vector<std::pair<float,float>>::iterator pos_track;

    NodeCoordRelation<float> NodeRelation;
    NodeValueVelocity<float> NodeVelocityValue;
        StatisticNode<float> Statistic;


    const QPair<float,float>& getOutput() override { return OutputCoord;};

    void getRange() { };
};


class StatisticCoordSpan: public PassCoordClass<float>
{
  public:
    StatisticCoordSpan(int SizeWindow) 
    { 
      axis1.resize(SizeWindow);
      axis2.resize(SizeWindow);

      axis1_pos = axis1.begin(); axis2_pos = axis2.begin();
      axis1_end = axis1.end();   axis2_end = axis2.end();
    };

    std::list<float> axis1;
    std::list<float> axis2;
    std::list<float>::iterator axis1_pos; 
    std::list<float>::iterator axis2_pos; 

    std::list<float>::iterator axis1_end; 
    std::list<float>::iterator axis2_end; 

    float Range1 = 0;
    float Range2 = 0;

    void getRange1() { axis1.sort(); Range1 = axis1.back() - axis1.front(); }
    void getRange2() { axis2.sort(); Range2 = axis2.back() - axis2.front(); }

    float sum1 = 0;
    float sum2 = 0;
    int counter = 0;

    void getRange1Avarage() 
    { 
      axis1.sort();  
      sum1 = 0; sum2 = 0; counter = 0;

      for(auto& val: axis1) {counter++; if(counter < axis1.size()) sum1 += val; else sum2 +=val; }
      Range1 = sum2/(axis1.size()/2) - sum1/(axis1.size()/2); 
    }

    void getRange2Avarage() 
    { 
      axis2.sort();  
      sum1 = 0; sum2 = 0; counter = 0;

      for(auto& val: axis2) {counter++; if(counter < axis2.size()) sum1 += val; else sum2 +=val; }
      Range2 = sum2/(axis2.size()/2) - sum1/(axis2.size()/2); 
    }

	const QPair<float,float>& getOutput() override { return OutputCoord;};
	void setInput(const QPair<float,float>& Coord) override  
  {
    *axis1_pos = Coord.first;  axis1_pos++; if(axis1_pos == axis1_end) { axis1_pos = axis1.begin(); getRange1Avarage();}
    *axis2_pos = Coord.second; axis2_pos++; if(axis2_pos == axis2_end) { axis2_pos = axis2.begin(); getRange2Avarage();}
  };
};

class TrackLinearRegression: public PassCoordClass<float>
{
  public:
    TrackLinearRegression(int Size) 
    { 
      track.resize(Size);
      pos_track = track.begin();
      SizeWindow = Size;
    };

    int SizeWindow = 10;
    std::vector<std::pair<float,float>> track;
    std::vector<std::pair<float,float>>::iterator pos_track;

    float slope  =  1;
    float offset =  1;

    const QPair<float,float>& getOutput() override { return OutputCoord;};

    float sumX  = 0.0f; 
    float sumY  = 0.0f; 
    float sumXY = 0.0f; 
    float sumXX = 0.0f;

    void getApproximation(const std::vector<std::pair<float, float>>& data) 
    {
        for (const auto& point : data) 
        {
            sumX  += point.first;
            sumY  += point.second;
            sumXY += point.first * point.second;
            sumXX += point.first * point.first;
        }
        slope = (SizeWindow * sumXY - sumX * sumY) / (SizeWindow * sumXX - sumX * sumX);
        offset = (sumY - slope * sumX) / SizeWindow;
    }
};

class TrackQuadraticRegression: public PassCoordClass<float>
{
  public:
    TrackQuadraticRegression(int Size) 
    { 
      track.resize(Size);
      pos_track = track.begin();
      SizeWindow = Size;
    };

    int SizeWindow = 10;
    std::vector<std::pair<float,float>> track;
    std::vector<std::pair<float,float>>::iterator pos_track;

    float slope  =  1;
    float offset =  1;

    const QPair<float,float>& getOutput() override { return OutputCoord;};

    float sumX  = 0.0f; 
    float sumY  = 0.0f; 
    float sumXY = 0.0f; 
    float sumXX = 0.0f;

    void getApproximation(const std::vector<std::pair<float, float>>& data) 
    {
        for (const auto& point : data) 
        {
            sumX  += point.first;
            sumY  += point.second;
            sumXY += point.first * point.second;
            sumXX += point.first * point.first;
        }
        slope = (SizeWindow * sumXY - sumX * sumY) / (SizeWindow * sumXX - sumX * sumX);
        offset = (sumY - slope * sumX) / SizeWindow;
    }
};


