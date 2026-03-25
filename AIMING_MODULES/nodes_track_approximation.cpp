#include "nodes_track_approximation.h"

template<>
void PolynomApproximation<2>::getApproximation()  
{
 //   double sumX =0;
 //   double sumY =0;
 //   double sumXY =0;
 //   double sumXX =0;

 //                     Track.setAutoRollback(SizeWindow - 20);
 //   auto track_span = Track.getCoords();

 //   for (const auto& point : track_span) 
 //   {
 //       sumX  += point.first;
 //       sumY  += point.second;
 //       sumXY += point.first * point.second;
 //       sumXX += point.first * point.first;
 //   }

 //   trackPolynom[1] = (SizeWindow * sumXY - sumX * sumY) / (SizeWindow * sumXX - sumX * sumX);
 //   trackPolynom[0] = (sumY - trackPolynom[1] * sumX) / SizeWindow;
}

template<>
std::vector<float> PolynomApproximation<2>::getApproximation(std::vector<std::pair<float,float>> trackInput)  
{
    double sumX  = 0;
    double sumY  = 0;
    double sumXY = 0;
    double sumXX = 0;

    for (const auto& point : trackInput) 
    {
        sumX  += point.first;
        sumY  += point.second;
        sumXY += point.first * point.second;
        sumXX += point.first * point.first;
    }
    trackPolynom[1] = (SizeWindow * sumXY - sumX * sumY) / (SizeWindow * sumXX - sumX * sumX);
    trackPolynom[0] = (sumY - trackPolynom[1] * sumX) / SizeWindow;
      return trackPolynom;
}

//=====================================================================================
template<>
void PolynomApproximation<3>::getApproximation()  
{
//    qDebug() << "GET APPROX" << Track.getAvailable();
//                      Track.setAutoRollback(SizeWindow - 20);
//    auto track_span = Track.getCoords();
//    auto coord = track_span[SizeWindow-2];
//    qDebug() << "DONE APPROX" << Track.getAvailable();
//    for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(SizeWindow); ++i) 
//    {
//        const double x = static_cast<double> (track_span[static_cast<std::size_t>(i)].first);
//        A_MAT3(i, 0) = x * x;
//        A_MAT3(i, 1) = x;
//        A_MAT3(i, 2) = 1.0;
//        Y_VEC(i) = static_cast<double>(track_span[static_cast<std::size_t>(i)].second);
//    }
//
//         Eigen::Vector3d RESULT = A_MAT3.colPivHouseholderQr().solve(Y_VEC);
//         if (!RESULT.allFinite()) { isResultValid = false; qDebug() << "APPROX FAIL"; return; } // Check for numerical issues (optional but good practice)
//                                    isResultValid = true;
//         trackPolynom[0] = RESULT(2);
//         trackPolynom[1] = RESULT(1);
//         trackPolynom[2] = RESULT(0);
//         getFuture(track_span);
//         CounterForward= 0; 
}


template<>
std::vector<float> PolynomApproximation<3>::getApproximation(std::vector<std::pair<float,float>> track)  
{
    A_MAT3 = Eigen::MatrixXd(static_cast<Eigen::Index>(track.size()), 3);
     Y_VEC = Eigen::VectorXd(static_cast<Eigen::Index>(track.size()));

    for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(track.size()); ++i) 
    {
        const double x = static_cast<double> (track[static_cast<std::size_t>(i)].first);
        A_MAT3(i, 0) = x * x;
        A_MAT3(i, 1) = x;
        A_MAT3(i, 2) = 1.0;
        Y_VEC(i) = static_cast<double>(track[static_cast<std::size_t>(i)].second);
    }

         Eigen::Vector3d RESULT = A_MAT3.colPivHouseholderQr().solve(Y_VEC);
         if (!RESULT.allFinite()) { isResultValid = false; return {0,0,0}; } // Check for numerical issues (optional but good practice)
                                    isResultValid = true;

                 trackPolynom[0] = RESULT(0);
                 trackPolynom[1] = RESULT(1);
                 trackPolynom[2] = RESULT(2);
    return this->trackPolynom;
}

