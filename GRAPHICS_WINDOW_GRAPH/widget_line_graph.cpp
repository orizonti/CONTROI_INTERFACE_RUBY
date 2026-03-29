#include "widget_line_graph.h"
#include <qnamespace.h>

WidgetLineGraph::WidgetLineGraph(QWidget *parent): WidgetAdjustable(parent)
{
    Q_UNUSED(parent)
    GraphPointsStorage = std::make_shared<GraphPointsLineInterface>();
    GraphPointsStorage2 = std::make_shared<GraphPointsLineInterface>();
    graphs.push_back(GraphPointsStorage);
    graphs.push_back(GraphPointsStorage2);

    PlotWidget = new QQuickWidget;

                    Layout = new QHBoxLayout;
                    Layout->addWidget(PlotWidget, 1);
    this->setLayout(Layout);

    InitGraphWidget();
}

WidgetLineGraph::~WidgetLineGraph()
{
    qDebug() << "CLOSE LINE GRAPH WIDGET";
    emit StartGenerateSinus(false);
    QThread::currentThread()->sleep(3);
    threadGenerator.quit();
    delete PlotWidget;
    delete Layout;
}


void WidgetLineGraph::InitGraphWidget()
{
    //QString qmlPath(QStringLiteral("D:/DEVELOPMENT/PROJECTS/CURRENT/PROJ_CONTROI_INTERFACE_RUBY/GRAPHICS_WINDOW_GRAPH/main.qml"));
    QString qmlPath("D:/DEVELOPMENT/PROJECTS/CURRENT/PROJ_CONTROI_INTERFACE_RUBY/GRAPHICS_WINDOW_GRAPH/");
            //qmlPath = qmlPath.arg(QGuiApplication::applicationDirPath());
    PlotWidget->engine()->addImportPath(qmlPath);
    qDebug() << "             INIT QML: " << qmlPath;

    QQmlContext *context = PlotWidget->engine()->rootContext();
    context->setContextProperty("GraphPointsStorage", GraphPointsStorage.get());
    context->setContextProperty("GraphPointsStorage2", GraphPointsStorage2.get());

    //insertSeries(int index, AbstractSeries series)
    //QObject* model = PlotWidget->findChild<QObject*>("GraphObject");
    //QMetaObject::invokeMethod(model,"insertSeries",0, GraphPointsStorage->ListPoints);

    PlotWidget->setSource(QUrl::fromLocalFile("D:/DEVELOPMENT/PROJECTS/CURRENT/PROJ_CONTROI_INTERFACE_RUBY/GRAPHICS_WINDOW_GRAPH/main.qml"));
    //qDebug() << "==========================";

    PlotWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    PlotWidget->resize(800,800);
}

