#ifndef LINE_GRAPH_H
#define LINE_GRAPH_H

#include <QObject>
#include <QLineSeries>
#include <QSplineSeries>
#include "graph_display_interface.h"

class GraphPointsLineInterface : public GraphDisplayInterface
{
    Q_OBJECT
    Q_PROPERTY(QLineSeries *SeriesPoints READ getSeries WRITE setSeries NOTIFY signalSeriesChanged FINAL)
public:
    explicit GraphPointsLineInterface(int Size = 300, QObject *parent = nullptr);
            ~GraphPointsLineInterface();
    QLineSeries* ListPoints;
             int counter = 0;
             int SizeStore = 300;

    QLineSeries *getSeries(); 
            void setSeries(QLineSeries* series);
            void setSize(int Size) override { SizeStore = Size; }

    void setPoints(const QList<QPointF>& points) override;
    void setPoints(std::span<QPair<float,float> > points) override;
    void setPoints(NodeCoordStorage<float>& point) override;
public slots:
    void slotClear() override;
    void slotAddPoint(QPair<float,float> Coord) override;
    void slotAddPoint(float Value) override;

signals:
    void signalSeriesChanged();

};


#endif 
