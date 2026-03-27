#include "nodes_track_approximation.h"


void PolynomApproximation<2>::setInput(const QPair<float,float>& Coord) 
{
                                        Coord >> TrackInput;
                                     if(isLoaded()) reset();
    sums[0] += Coord.first;
    sums[1] += Coord.second;
    sums[2] += Coord.first * Coord.second;
    sums[3] += Coord.first * Coord.first; if(!isLoaded()) return;

    trackPolynom[1] = (SizeWindow * sums[2] - sums[0] * sums[1]) / (SizeWindow * sums[3] - sums[0] * sums[0]);
    trackPolynom[0] = (sums[1] - trackPolynom[1] * sums[0]) / SizeWindow;

    getFuture();
}

std::vector<float> PolynomApproximation<2>::getApproximation(NodeCoordStorage<float>& track)   
{
    double sumX  = 0;
    double sumY  = 0;
    double sumXY = 0;
    double sumXX = 0;

    for (const auto& point : track) 
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

std::vector<float> PolynomApproximation<2>::getApproximation(std::span<std::pair<float,float>> track)   
{
    double sumX  = 0;
    double sumY  = 0;
    double sumXY = 0;
    double sumXX = 0;

    for (const auto& point : track) 
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
void PolynomApproximation<3>::setInput(const QPair<float,float>& Coord) 
{
                                        Coord >> TrackInput;
                                     if(isLoaded()) reset();

    //qDebug() << "[ TRACK INPUT ]" << Coord.first << Coord.second << "LOADED: " << TrackInput.isLoaded() << TrackInput.getAvailable();
    float x = Coord.first;
    A_MAT3(IndexInput, 0) = x * x;
    A_MAT3(IndexInput, 1) = x;
    A_MAT3(IndexInput, 2) = 1.0;
    Y_VEC(IndexInput) = Coord.second;   IndexInput++; 
    if(!isLoaded()) return; 
        
    MeasurePeriod++;

    Eigen::Vector3d RESULT = A_MAT3.colPivHouseholderQr().solve(Y_VEC);
    if (!RESULT.allFinite()) { isResultValid = false; qDebug() << "APPROX FAIL"; return; } // Check for numerical issues (optional but good practice)
                               isResultValid = true;
    trackPolynom[0] = RESULT(2);
    trackPolynom[1] = RESULT(1);
    trackPolynom[2] = RESULT(0);
    getFuture(); 
    MeasurePeriod++;

    qDebug() << "[ GET APPROX ]" << RESULT(0) << RESULT(1) << RESULT(2) << "TIME: " << MeasurePeriod.getMicroseconds();
}


template<>
std::vector<float> PolynomApproximation<3>::getApproximation(NodeCoordStorage<float>& track)   
{
    A_MAT3 = Eigen::MatrixXd(static_cast<Eigen::Index>(track.size()), 3);
     Y_VEC = Eigen::VectorXd(static_cast<Eigen::Index>(track.size()));

    Eigen::Index IDX = 0;
    for(auto& coord: track)
    {
        A_MAT3(IDX, 0) = coord.first * coord.first;
        A_MAT3(IDX, 1) = coord.first;
        A_MAT3(IDX, 2) = 1.0;
        Y_VEC(IDX) = coord.second; IDX++;
    }

         Eigen::Vector3d RESULT = A_MAT3.colPivHouseholderQr().solve(Y_VEC);
         if (!RESULT.allFinite()) { isResultValid = false; return {0,0,0}; } // Check for numerical issues (optional but good practice)
                                    isResultValid = true;

                 trackPolynom[0] = RESULT(0);
                 trackPolynom[1] = RESULT(1);
                 trackPolynom[2] = RESULT(2);
    return this->trackPolynom;

}

template<>
std::vector<float> PolynomApproximation<3>::getApproximation(std::span<std::pair<float,float>> track)   
{
    A_MAT3 = Eigen::MatrixXd(static_cast<Eigen::Index>(track.size()), 3);
     Y_VEC = Eigen::VectorXd(static_cast<Eigen::Index>(track.size()));

    Eigen::Index IDX = 0;
    for(auto& coord: track)
    {
        A_MAT3(IDX, 0) = coord.first * coord.first;
        A_MAT3(IDX, 1) = coord.first;
        A_MAT3(IDX, 2) = 1.0;
        Y_VEC(IDX) = coord.second; IDX++;
    }

         Eigen::Vector3d RESULT = A_MAT3.colPivHouseholderQr().solve(Y_VEC);
         if (!RESULT.allFinite()) { isResultValid = false; return {0,0,0}; } // Check for numerical issues (optional but good practice)
                                    isResultValid = true;

                 trackPolynom[0] = RESULT(0);
                 trackPolynom[1] = RESULT(1);
                 trackPolynom[2] = RESULT(2);
    return this->trackPolynom;
}


