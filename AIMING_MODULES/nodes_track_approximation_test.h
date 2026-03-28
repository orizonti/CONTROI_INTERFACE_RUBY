#ifndef TRACK_APPROXIMATION_TEST_CLASS_H
#define TRACK_APPROXIMATION_TEST_CLASS_H

#include "nodes_track_approximation.h"
#include "graph_display_interface.h"

template<int NUM_PARAM>
class PolynomApproximationDynamicTest: public PassCoordClass<float>
{
  public:
  PolynomApproximationDynamicTest()
  {
    CoordStorage1.setContinousMode(true);
  };
  int NumberPoints = 140;
  PolynomApproximation<NUM_PARAM> trackApproximation{NumberPoints};

  NodeCoordRandomizer<float> Randomize{0,20};

	void setInput(const QPair<float, float>& Coord) 
  { 
    Coord >> Randomize >> trackApproximation >> CoordStorage1;

    if(trackApproximation.isLoaded()) { 
                                   //qDebug() << "STORE FUTURE: "  << CoordStorage1.getAvailable();
                                   if(displayGraph1) displayGraph1->setPoints(trackApproximation.TrackInput); 
                                   //if(displayGraph2) displayGraph2->setPoints(trackApproximation.TrackFuture); 
                                   if(displayGraph2) displayGraph2->setPoints(CoordStorage1); 
                                    // trackApproximation.flushTrack();
                                      }
  };
  NodeCoordStorage<float> CoordStorage1{NumberPoints};

  void linkToGraph(std::shared_ptr<GraphDisplayInterface> graph) { if(displayGraph1 == nullptr) displayGraph1 = graph;
                                                                   else if(displayGraph2 == nullptr) displayGraph2 = graph;}

  std::shared_ptr<GraphDisplayInterface> displayGraph1 = nullptr;
  std::shared_ptr<GraphDisplayInterface> displayGraph2 = nullptr;
};


template<int NUM_PARAM>
class PolynomApproximationTest
{
  public:
    int NumberPoints = 300;
  float Range = 400;
  float Step = Range/NumberPoints;
    int Direction = 1;
  float Dispersion = 15;

  std::pair<float,float> CoordStart{20,20};
  PolynomApproximation<NUM_PARAM> trackApproximation{NumberPoints};

  std::vector<float> param = {0,0,0,0};
  std::vector<float> params_output = {0,0,0,0};

  QList<QPointF> Track;
  QList<QPointF> TrackNoize;
  QList<QPointF> TrackApprox;

  std::vector<std::pair<float,float>> TrackCoords;
  std::vector<std::pair<float,float>> TrackCoordsNoize;

  PolynomApproximationTest(const std::vector<float>& Polynom)
  {
    generateInput(Polynom);

               TrackCoordsNoize >> trackApproximation;
    auto [C3,C2,C1,C0, IS_VALID] = trackApproximation.getResult();

    generateOutput(C3,C2,C1,C0);

      qDebug() << "[ INPUT PARAM ]" << Polynom[3] << Polynom[2] << Polynom[1] << Polynom[0]
               << "[ APPROX ]" << C3 << C2 << C1 << C0
               << "[ DIFF   ]" << abs(Polynom[3] - C3 ) << abs(Polynom[2] - C2 )  << abs(Polynom[1] - C1 ) << abs(Polynom[0] - C0)
               << "[ REUSLT ]" << isResultValid(C3,C2,C1,C0);
  }

  PolynomApproximationTest(float PC3, float PC2, float PC1, float PC0)
  {
    generateInput({PC3,PC2,PC1,PC0});

               TrackCoordsNoize >> trackApproximation;
    auto [C3,C2,C1,C0, IS_VALID] = trackApproximation.getResult();

    generateOutput(C3,C2,C1,C0);

      qDebug() << "[ INPUT PARAM ]" << PC3 << PC2 << PC1 << PC0 
               << "[ APPROX ]" << C3 << C2 << C1 << C0
               << "[ DIFF   ]" << abs(PC3 - C3 ) << abs(PC2 - C2 )  << abs(PC1 - C1 ) << abs(PC0 - C0)
               << "[ REUSLT ]" << isResultValid(C3,C2,C1,C0);
  }

  bool isResultValid(float C3,float C2,float C1,float C0) 
                               { return abs(param[0] - C0) < C0/10 && 
                                        abs(param[1] - C1) < C1/10 && 
                                        abs(param[2] - C2) < C2/10 && 
                                        abs(param[3] - C3) < C3/10; }

  void generateOutput(float C3,float C2,float C1,float C0)
  {
    TrackApprox.resize(NumberPoints+20); std::fill(TrackApprox.begin(), TrackApprox.end(),QPointF(0,0));
    std::pair<float,float> Coord;

    auto PointApprox = TrackApprox.begin();
    for(float n = 0; n < NumberPoints + 20; n++)
    {
      Coord.first  = n*Step*Direction + CoordStart.first; 
      Coord.second = std::pow(Coord.first,3)*C3 + std::pow(Coord.first,2)*C2 + Coord.first*C1 + C0;
      if(PointApprox != TrackApprox.end()) { PointApprox->setX(Coord.first); PointApprox->setY(Coord.second); PointApprox++; }
    }
  }


  void generateInput(const std::vector<float>& Polynom)
  {
        std::pair<float,float> Coord;
        std::pair<float,float> CoordNoize;
    NodeValueRandomizer<float> Randomize{10};
            std::vector<float> element{0,0,0,0};

    param = Polynom;

    for(float n = 0; n < NumberPoints; n++)
    {

      Coord.first  = n*Step*Direction; 
      element[3] = std::pow(Coord.first,3); 
      element[2] = std::pow(Coord.first,2); 
      element[1] = Coord.first;  
      element[0] = 0;  
      Coord.second = element[3]*param[3] + element[2]*param[2] + element[1]*param[1] + param[0];

      Coord = CoordStart + Coord;
                                               CoordNoize.first = Coord.first;
      Coord.second >> Randomize(Dispersion) >> CoordNoize.second;

            Track.append(QPointF(Coord.first,Coord.second));
       TrackNoize.append(QPointF(CoordNoize.first,CoordNoize.second));
       TrackCoords.push_back(Coord);
       TrackCoordsNoize.push_back(CoordNoize);

       qDebug() << "POS: " << Coord.first << Coord.second;
      //CoordNoize >> trackApproximation;
    }
  }

};

#endif 
