#ifndef TRACK_APPROXIMATION_CLASS_H
#define TRACK_APPROXIMATION_CLASS_H
#include <QDebug>
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>
#include <Eigen/src/Core/Matrix.h>
#include <interface_pass_coord.h>
#include <initializer_list>
#include <iomanip>
#include "thread_operation_nodes.h"
#include "module_period_measure.h"

class GenericApproximation: public PassCoordClass<float>
{
  public:
  GenericApproximation(int Size = 100) 
  {
      Track.setSize(Size); 
      TrackFuture.setSize(Size); 
      SizeWindow = Size;
  }
  int SizeWindow = 10;


  NodeCoordStorage<float> Track;
  NodeCoordStorage<float> TrackFuture;

  double RMSE = std::numeric_limits<double>::quiet_NaN();
  bool isResultValid = false;

    MeasurePeriodNode MeasurePeriod;
    void setInput(const QPair<float,float>& Coord) override  
    {   
           
          Coord >> Track;
          if(Track.getAvailable() >= SizeWindow ) getApproximation();  
    };

	  const QPair<float, float>& getOutput() override { return getFuture();};

    bool isLoaded() {
        //qDebug() << "COORD AVAILABLE: " << Track.getAvailable(); 
        return Track.getAvailable() >= SizeWindow;}
    virtual std::vector<float> getApproximation(std::vector<std::pair<float,float>>) = 0;  
    virtual std::tuple<float,float,float,float,bool> getResult() = 0;
    friend void operator>>(const std::vector<std::pair<float,float>>& coords, GenericApproximation& receiver) { for(auto& coord: coords) coord >> receiver;}

    virtual std::pair<float,float> getFuture() = 0;
    virtual std::span<QPair<float,float>> getFuture(std::span<QPair<float,float>> track) = 0;
    protected:
                  virtual void getApproximation() = 0;  
};

template<int NUM_PARAM>
class PolynomApproximation: public GenericApproximation
{
  public:
    PolynomApproximation(int Size = 100) : GenericApproximation(Size)
    { 
      A_MAT1 = Eigen::MatrixXd(static_cast<Eigen::Index>(Size), 1);
      A_MAT3 = Eigen::MatrixXd(static_cast<Eigen::Index>(Size), 3);
       Y_VEC = Eigen::VectorXd(static_cast<Eigen::Index>(Size));
    };

    void operator=(const PolynomApproximation<NUM_PARAM>& copy);
    int CounterForward = 0;

    NodeCoordVelocity<float> NodeVelocity;

    Eigen::MatrixXd A_MAT1;
    Eigen::MatrixXd A_MAT3;
    Eigen::VectorXd Y_VEC;
    std::vector<float> trackPolynom{0,0,0,0};
    std::pair<float,float> posFuture;

    std::vector<float> getApproximation(std::vector<std::pair<float,float>>) override;  
    std::tuple<float,float,float,float,bool> getResult() override { return { trackPolynom[3], trackPolynom[2], trackPolynom[1], trackPolynom[0],true};};

    std::pair<float,float> getFuture() override;
    std::span<QPair<float,float>> getFuture(std::span<QPair<float,float>>) override;
    protected:
                  void getApproximation() override;  
};


template<int NUM_PARAM>
void PolynomApproximation<NUM_PARAM>::operator=(const PolynomApproximation<NUM_PARAM>& copy)
{
	SizeWindow = copy.SizeWindow;

	A_MAT1 = Eigen::MatrixXd(static_cast<Eigen::Index>(SizeWindow), 1);
	A_MAT3 = Eigen::MatrixXd(static_cast<Eigen::Index>(SizeWindow), 3);
	 Y_VEC = Eigen::VectorXd(static_cast<Eigen::Index>(SizeWindow));
};

template<int NUM_PARAM>
std::pair<float,float> PolynomApproximation<NUM_PARAM>::getFuture()
{
    //float step = 0.5;
    //posFuture.first = posLast.first + SizeWindow*step;  
    //posFuture.second = trackPolynom[2]*std::pow(posFuture.first,2) +
    //                   trackPolynom[1]*posFuture.first +
    //                   trackPolynom[0];
    //qDebug() << "POS LAST  : " << posLast.first   << posLast.second
    //         << "FUTURE_ONE: " << posFuture.first << posFuture.second << "PARAM: " << trackPolynom[2] << trackPolynom[1] << trackPolynom[0];
    return TrackFuture.getOutput();
}

template<int NUM_PARAM>
std::span<QPair<float,float>> PolynomApproximation<NUM_PARAM>::getFuture(std::span<QPair<float,float>> track)
{
    //float step = track[SizeWindow-1].first - track[SizeWindow-2].first; 
//    float step = track[1].first - track[0].first;
//    //if(track_future.size() != Number) track_future.resize(Number);
//
//    TrackFuture[0].first = track[0].first + 20*step;  
//    TrackFuture[0].second = trackPolynom[2]*std::pow(TrackFuture[0].first,2) +
//                            trackPolynom[1]*TrackFuture[0].first +
//                            trackPolynom[0];
//
//    //qDebug() << "===================================";
//    for(int n = 1; n < TrackFuture.size(); n++)
//    {
//    TrackFuture[n].first = TrackFuture[n-1].first + step;  
//    TrackFuture[n].second = trackPolynom[2]*std::pow(TrackFuture[n].first,2) +
//                             trackPolynom[1]*TrackFuture[n].first +
//                             trackPolynom[0];
//    //qDebug() << "POS LAST  : " << posLast.first   << posLast.second
//    //         << "FUTURE_SER: " << posFuture.first << posFuture.second << "PARAM: " << trackPolynom[2] << trackPolynom[1] << trackPolynom[0];
//    }
//    TrackFuture = TrackFuture.begin();

    return track;
}


#endif 

    //Eigen::VectorXd residual = A_MAT * PARAMS_FUNC - Y_VEC;
    //const double mse = residual.squaredNorm() / static_cast<double>(SizeWindow);
    //RMSE = std::sqrt(mse);
