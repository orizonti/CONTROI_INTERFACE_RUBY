#pragma once
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>
#include <qnamespace.h>
#include <QPen>
#include <QColor>
#include <cmath>
#include <QTimer>
#include "device_generic_interface.h"

class WidgetRotaryControl : public QWidget 
{
    Q_OBJECT

public:
    enum class TypeDraw { TypeCircle = 0, TypeArc = 1 };

    WidgetRotaryControl(QWidget* parent = nullptr);

    void linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device) { ControlRotary = Device; };
    void synchronizePeer(WidgetRotaryControl* ControlDevice);
    void initWidget();
    void setType(TypeDraw Type);

    void drawObjects(QPainter* painter);
    void drawRotation();

    std::shared_ptr<DeviceGenericHandleControl> ControlRotary = nullptr;
    bool allowSignals = true;
    int ControlChannel = 0;
    //==========================================
    //CURRENT ANGLE
    float angleRotation = 0;
    float angleRotationFuture = 0;
    float angleRotationDegree = 0;
    float angleRotationFutureDegree = 0;

    //=======================================================================
    //LIMITS
    float angleRotationMinDegree = -20;
    float angleRotationMaxDegree = 20;
    float angleRotationMin = angleRotationMinDegree*M_PI/180;
    float angleRotationMax = angleRotationMaxDegree*M_PI/180;
    float angleSpanDegree = angleRotationMaxDegree - angleRotationMinDegree; 
    //=======================================================================

    private:
    //==========================================
    //SELECTED POINT
    QPoint pointDirection;
    QPoint pointDirectionFuture;
    float pos_x_radial = 0;
    float pos_y_radial = 0;
    float pos_x_radial_future = 0;
    float pos_y_radial_future = 0;

    float radius_cursor = 0;
    float radius_cursor_future = 0;
    //==========================================
    //DRAWING AREA
       int TypeWidget = 0;
       int sizeWidget = 160;
       int sizeDrawing = sizeWidget*0.6;
     QRect rectDrawing;
    QPoint pointCenterDrawing;
       int radiusDirection = sizeDrawing;

    QPen pen1{QColor(214, 136, 41)  , 3, Qt::SolidLine };
    QPen pen2{QColor(30, 221, 78)  , 2, Qt::SolidLine };
    QPen pen3{QColor(215, 50, 28), 2, Qt::DashLine};
    QPen pen4{QColor(215, 50, 28), 2, Qt::SolidLine};

      int directionMove = 1;
    float stepMove      = 0.5;
           QTimer timerMove;
           //QTimer timerCheckState;

public slots:
void slotSetAngle(float angleDegree);
void updateAngle();
void updateFutureAngle();
void slotMoveStart(int Direction);

private slots:
void slotMove();
void slotCheckState();

signals:
void signalAngleChanged(float angleDegree);

protected:
    void paintEvent(QPaintEvent* event) override { drawRotation(); }
    void resizeEvent(QResizeEvent *event) override    { sizeWidget = event->size().width(); setBaseSize(sizeWidget, sizeWidget); initWidget(); }
    void mousePressEvent(QMouseEvent* event) override { pointDirection = event->pos() - pointCenterDrawing; updateAngle(); }
    void mouseMoveEvent(QMouseEvent* event)  override { pointDirectionFuture = event->pos() - pointCenterDrawing; updateFutureAngle(); }
    void leaveEvent(QEvent* event)           override { pointDirectionFuture = pointDirection; updateFutureAngle(); }
};