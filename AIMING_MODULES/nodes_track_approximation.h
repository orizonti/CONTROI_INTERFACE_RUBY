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

template<typename T_NODE>
class TrackRegresstionTest
{
  public:
  int NumberPoints = 300;
  float Range = 15;
  float Step = Range/NumberPoints;
  CoordTrackApproximation trackApprox{NumberPoints};

  ValueRandomization<float> Randomize{14};
  std::pair<float,float> Coord;
  std::pair<float,float> CoordNoize;

  float C1 = 0;
  float C2 = 0;
  float C3 = 0;
  float C4 = 0;

  QList<QPointF> Track;
  QList<QPointF> TrackNoize;
  QList<QPointF> TrackApprox;

  TrackQuadraticRegresstionTest(float C1, float C2, float C3, float C4 = 0, int Size)
  {
    NumberPoints = Size;
    Step = Range/Size;
    trackApprox = CoordTrackApproximation(Size);

    std::pair<float,float> Coord;
    std::pair<float,float> CoordNoize;

    for(float n = 0; n < NumberPoints; n++)
    {
      Coord.first  = n*Step;
      CoordNoize.first = Coord.first;
      Coord.second = std::pow(Coord.first,2)*C1 + Coord.first*C2 + C3;
      Coord.second >> Randomize(20) >> CoordNoize.second;

            Track.append(QPointF(Coord.first,Coord.second));
       TrackNoize.append(QPointF(CoordNoize.first,CoordNoize.second));
      TrackApprox.append(QPointF(Coord.first,Coord.second));

      CoordNoize >> trackApprox;
    }

      auto [A_RES,B_RES,C_RES] = trackApprox.getResult(); 

      for(auto& point: TrackApprox) point.setY(std::pow(point.x(),2)*A_APPROX + point.x()*B_APPROX + C_APPROX);

      //qDebug() << "[ INPUT PARAM ]" << A_PARAM << B_PARAM << C_PARAM 
      //         << "[ APPROX ]" << A_APPROX << B_APPROX << C_APPROX
      //         << "[ DIFF   ]" << abs(A_PARAM - A_APPROX) << abs(B_PARAM - B_APPROX)  << abs(C_PARAM - C_APPROX) 
      //         << "[ REUSLT ]" << isResultValid();

  }

//  bool isResultValid() { return abs(A_PARAM - A_APPROX) < A_PARAM/10 && 
//                                abs(B_PARAM - B_APPROX) < B_PARAM/10 && 
//                                abs(C_PARAM - C_APPROX) < C_PARAM/10; }

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

    std::vector<std::pair<float,float>> track;
    std::vector<std::pair<float,float>>::iterator pos_track;

    int SizeWindow = 10;

    float slope  =  1;
    float offset =  1;

    float sumX  = 0.0f; 
    float sumY  = 0.0f; 
    float sumXY = 0.0f; 
    float sumXX = 0.0f;

    const QPair<float,float>& getOutput() override { return OutputCoord;};
    void setInput(const QPair<float,float>& Coord) override  
    {
      *pos_track = Coord; pos_track++; if(pos_track == track.end()) { pos_track = track.begin(); 
                                                                      getApproximation(); } 
    };


    void getApproximation() 
    {
        for (const auto& point : track) 
        {
            sumX  += point.first;
            sumY  += point.second;
            sumXY += point.first * point.second;
            sumXX += point.first * point.first;
        }
      qDebug() << "GET LINEAR APPROXIMATION" << sumX << sumY << sumXY << sumXX << slope << offset;
        slope = (SizeWindow * sumXY - sumX * sumY) / (SizeWindow * sumXX - sumX * sumX);
        offset = (sumY - slope * sumX) / SizeWindow;
    }
};


template<int NUM_PARAM>
class CoordTrackApproximation: public PassCoordClass<float>
{
  public:


    int SizeWindow = 10;
    std::vector<std::pair<float,float>> track;
    std::vector<std::pair<float,float>>::iterator pos_track;

    double RMSE = std::numeric_limits<double>::quiet_NaN();
    bool isResultValid = false;

    Eigen::MatrixXd A_MAT;
    Eigen::VectorXd Y_VEC;
    Eigen::Vector3d PARAMS_FUNC;

    const QPair<float,float>& getOutput() override { return OutputCoord;};
};

template<int NUM_PARAM>
CoordTrackApproximation(int Size = 100) 
{ 
	track.resize(Size); pos_track = track.begin();
	SizeWindow = Size;

	A_MAT = Eigen::MatrixXd(static_cast<Eigen::Index>(SizeWindow), 3);
	Y_VEC = Eigen::VectorXd(static_cast<Eigen::Index>(SizeWindow));
};

template<int NUM_PARAM>
void operator=(const CoordTrackApproximation& copy)
{
	track.resize(copy.SizeWindow);
	pos_track = track.begin();
	SizeWindow = copy.SizeWindow;
	A_MAT = Eigen::MatrixXd(static_cast<Eigen::Index>(SizeWindow), 3);
	Y_VEC = Eigen::VectorXd(static_cast<Eigen::Index>(SizeWindow));
};

template<int NUM_PARAM>
void getApproximation()  
{
    qDebug() << "GET APPROXIMATION SIZE: " << track.size();
    for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(SizeWindow); ++i) 
    {
        const double x = static_cast<double> (track[static_cast<std::size_t>(i)].first);

        A_MAT(i, 0) = x * x;
        A_MAT(i, 1) = x;
        A_MAT(i, 2) = 1.0;
        Y_VEC(i) = static_cast<double>(track[static_cast<std::size_t>(i)].second);
    }

         PARAMS_FUNC = A_MAT.colPivHouseholderQr().solve(Y_VEC);
    if (!PARAMS_FUNC.allFinite()) { isResultValid = false; return; } // Check for numerical issues (optional but good practice)
                                    isResultValid = true;

    //Eigen::VectorXd residual = A_MAT * PARAMS_FUNC - Y_VEC;
    //const double mse = residual.squaredNorm() / static_cast<double>(SizeWindow);
    //RMSE = std::sqrt(mse);

}


template<int NUM_PARAM>
std::tuple<float,float,float> getApproximation(std::vector<std::pair<float,float>> track)  
{
    qDebug() << "GET APPROXIMATION";

    A_MAT = Eigen::MatrixXd(static_cast<Eigen::Index>(track.size()), 3);
    Y_VEC = Eigen::VectorXd(static_cast<Eigen::Index>(track.size()));

    for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(track.size()); ++i) 
    {
        const double x = static_cast<double> (track[static_cast<std::size_t>(i)].first);
        A_MAT(i, 0) = x * x;
        A_MAT(i, 1) = x;
        A_MAT(i, 2) = 1.0;
        Y_VEC(i) = static_cast<double>(track[static_cast<std::size_t>(i)].second);
    }

         PARAMS_FUNC = A_MAT.colPivHouseholderQr().solve(Y_VEC);
    if (!PARAMS_FUNC.allFinite()) { isResultValid = false; return std::make_tuple(0,0,0); } // Check for numerical issues (optional but good practice)
                                    isResultValid = true;

    return std::make_tuple(PARAMS_FUNC(0),PARAMS_FUNC(1),PARAMS_FUNC(2));
}

template<int NUM_PARAM>
std::tuple<float,float,float> getResult()  { return std::make_tuple(PARAMS_FUNC(0),PARAMS_FUNC(1),PARAMS_FUNC(2)); }


template<int NUM_PARAM>
void setInput(const QPair<float,float>& Coord) override  
{
	*pos_track = Coord; 
	pos_track++; if(pos_track == track.end()) { pos_track = track.begin(); getApproximation(); } 
};


#endif 
