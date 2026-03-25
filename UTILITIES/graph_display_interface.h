#ifndef GRAPH_INTERFACE_H
#define GRAPH_INTERFACE_H
#include "interface_pass_value.h"

#include <QObject>
#include <QList>
#include <span>
#include "thread_operation_nodes.h"

class GraphDisplayInterface: public QObject, public PassValueClass<float>
{
public:
    virtual void setPoints(const QList<QPointF>& points) = 0;
    virtual void setPoints(std::span<QPair<float,float> > points) = 0;
    virtual void setPoints(NodeCoordStorage<float>& point) = 0;
    virtual void setSize(int size) = 0;

    friend void operator>>(QPair<float,float> Coord, GraphDisplayInterface& graph) { graph.slotAddPoint(Coord); }
    friend void operator>>(float Value, GraphDisplayInterface& graph)              { graph.slotAddPoint(Value); }
    void setValue(float Value) override { slotAddPoint(Value); }
public slots:
    virtual void slotClear() = 0;
    virtual void slotAddPoint(QPair<float,float> Coord) = 0;
    virtual void slotAddPoint(float Value) = 0;
};
#endif 
