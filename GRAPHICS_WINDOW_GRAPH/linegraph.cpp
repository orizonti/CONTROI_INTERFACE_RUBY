#include "linegraph.h"
#include <QRandomGenerator>
#include <qlineseries.h>

GraphPointsLineInterface::GraphPointsLineInterface(int Size, QObject *parent): SizeStore(Size)
{
    Q_UNUSED(parent)
    ListPoints = new QLineSeries;
}

GraphPointsLineInterface::~GraphPointsLineInterface() { delete ListPoints; }

QLineSeries* GraphPointsLineInterface::getSeries() { return ListPoints; }


void GraphPointsLineInterface::slotClear() { ListPoints->clear(); }
void GraphPointsLineInterface::setSeries(QLineSeries *series) { }

void GraphPointsLineInterface::setPoints(const QList<QPointF>& points)
{
   ListPoints->clear();
   ListPoints->append(points);
}

void GraphPointsLineInterface::setPoints(std::span<QPair<float,float>> points)
{
   ListPoints->clear(); for(auto& point: points) { ListPoints->append(point.first, point.second); }
}

void GraphPointsLineInterface::setPoints(NodeCoordStorage<float>& points) 
{
   ListPoints->clear(); for(auto& point: points) { ListPoints->append(point.first, point.second); }
}

void GraphPointsLineInterface::slotAddPoint(QPair<float,float> Coord)
{
      ListPoints->append(counter++, Coord.first);
   if(ListPoints->points().size() > SizeStore) { counter = 0; ListPoints->clear(); }
}

void GraphPointsLineInterface::slotAddPoint(float Value)
{
      ListPoints->append(counter++,Value);
   if(ListPoints->points().size() > SizeStore) { counter = 0; ListPoints->clear(); }
}

//==================================================================
