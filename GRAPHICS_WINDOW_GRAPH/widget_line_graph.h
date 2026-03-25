// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef WIDGET_LINEGRAPH_H
#define WIDGET_LINEGRAPH_H

#include <QWidget>
#include <QQuickWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QQmlContext>
#include "linegraph.h"
#include "sinus_generator_class.h"
#include <QThread>
#include "widget_adjustable.h"
#include "interface_pass_coord.h"
#include "interface_pass_value.h"

class WidgetLineGraph : public WidgetAdjustable, public PassCoordClass<float>
{
    Q_OBJECT
public:
    explicit WidgetLineGraph(QWidget *parent = nullptr);
    ~WidgetLineGraph();
    GraphPointsLineInterface& operator()(int num) { if(num == 0) return *GraphPointsStorage; else return *GraphPointsStorage2;};

    void InitGraphWidget();
    QThread threadGenerator;

    std::shared_ptr<GraphPointsLineInterface> GraphPointsStorage  = nullptr;
    std::shared_ptr<GraphPointsLineInterface> GraphPointsStorage2 = nullptr;
    std::vector<std::shared_ptr<GraphPointsLineInterface>> graphs;

	void setInput(const QPair<float, float>& Coord) 
    {
        Coord.first  >> *GraphPointsStorage;
        Coord.second >> *GraphPointsStorage2;
    };

    void setSize(int size) { GraphPointsStorage->setSize(size); GraphPointsStorage2->setSize(size); };
private:
    QQuickWidget *PlotWidget;
    QHBoxLayout *Layout;

signals:
void StartGenerateSinus(bool);
void StopThreads();
};

#endif 
