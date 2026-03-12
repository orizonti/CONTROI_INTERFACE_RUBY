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

    std::shared_ptr<DeviceGenericHandleControl> ControlRotary = nullptr;
    void linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device) { ControlRotary = Device; };
    int ControlChannel = 0;

    WidgetRotaryControl(QWidget* parent = nullptr) : QWidget(parent) 
    {
        setWindowTitle("Watch Widget");
        setBaseSize(sizeWidget, sizeWidget);  // Set the size of the widget
        initWidget();
        this->setMouseTracking(true);
        QObject::connect(&timerMove, SIGNAL(timeout()), this, SLOT(slotMove()));
        ControlChannel = TypeWidget;
    }

    void setType(TypeDraw Type) { TypeWidget = (int)Type; initWidget(); ControlChannel = TypeWidget; }

    void initWidget()
    {

        pointDirection = mapFromGlobal(QCursor::pos());
        sizeDrawing = sizeWidget*0.7;
        pointCenterDrawing = rect().center();

        if(TypeWidget == 1)
        {
        sizeDrawing = 2*sizeDrawing;
        pointCenterDrawing = rect().center(); pointCenterDrawing.setX(15); 
        }

        radiusDirection = sizeDrawing/2;
        rectDrawing = QRect(pointCenterDrawing.x() - sizeDrawing/2, pointCenterDrawing.y() - sizeDrawing/2, sizeDrawing, sizeDrawing);
        update();
    }

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

    void drawObjects(QPainter* painter)
    {
        float angle = 0;
        int number_step = 12;
        int step = 360/number_step;

        QPoint pointObject;
         QRect rectObject;
        float radiusObject = radiusDirection + 20;

        for(int n = 0; n < number_step; n++)
        {
          pointObject =  QPoint(radiusObject * std::cos(angle), radiusObject * std::sin(angle));
          pointObject += pointCenterDrawing;
           rectObject =  QRect(pointObject.x() - 10, pointObject.y() - 10, 20,20);
           painter->drawEllipse(rectObject);
            angle += step*M_PI/180;
        }
    }

    void drawRotation()
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setPen(pen1);
        if(TypeWidget == 0) painter.drawEllipse(rectDrawing);
        if(TypeWidget == 1) painter.drawArc(rectDrawing, angleRotationMinDegree*16, angleSpanDegree*16);

        if(TypeWidget == 0) drawObjects(&painter);

        painter.setPen(pen2);
        painter.drawPie(rectDrawing, angleRotationDegree * 16 - 10*16, 20*16);

        painter.setPen(pen3);
        painter.drawLine(pointCenterDrawing, 
                         pointCenterDrawing + QPoint(radiusDirection * std::cos(angleRotationFuture), 
                         -radiusDirection * std::sin(angleRotationFuture)) );

        painter.setPen(pen4);
        painter.drawEllipse(pointCenterDrawing, 4,4);
        painter.end();
    }


public slots:
void slotSetAngle(float angleDegree)
{
              angleRotationDegree = angleDegree ; 
        angleRotationFutureDegree = angleDegree;

              angleRotation = angleDegree*M_PI/180;
        angleRotationFuture = angleDegree*M_PI/180;

        if(TypeWidget == 1)
        {
        if(angleRotation < angleRotationMin) angleRotation = angleRotationMin; 
        if(angleRotation > angleRotationMax) angleRotation = angleRotationMax;
        }

        update();  
}

void updateAngle()
{
        pos_x_radial = pointDirection.x();
        pos_y_radial = pointDirection.y();
           radius_cursor = std::hypot(pos_x_radial, pos_y_radial);
        if(radius_cursor > sizeDrawing/2) return;

        angleRotation = std::acos(pos_x_radial/radius_cursor); 

        if(TypeWidget == 1)
        {
        if(angleRotation < angleRotationMin) angleRotation = angleRotationMin; 
        if(angleRotation > angleRotationMax) angleRotation = angleRotationMax;
        }

                             angleRotationDegree = angleRotation*180/M_PI;
        if(pos_y_radial > 0) angleRotationDegree = 360 - angleRotationDegree;

        if(ControlRotary) ControlRotary->setParam(ControlChannel, angleRotationDegree);

        update();  
}

void updateFutureAngle()
{
        pos_x_radial_future = pointDirectionFuture.x();
        pos_y_radial_future = pointDirectionFuture.y();

           radius_cursor_future = std::hypot(pos_x_radial_future, pos_y_radial_future);
        if(radius_cursor_future > sizeDrawing/2) return;

        angleRotationFuture = std::acos(pos_x_radial_future/radius_cursor_future); 
        if(TypeWidget == 1)
        {
        if(angleRotationFuture < angleRotationMin) angleRotationFuture = angleRotationMin; 
        if(angleRotationFuture > angleRotationMax) angleRotationFuture = angleRotationMax;
        }

        if(pos_y_radial_future > 0) angleRotationFuture = 2*M_PI - angleRotationFuture;
                                    angleRotationFutureDegree = angleRotationFuture*180/M_PI;
        update();  
}
public slots:
void slotMoveStart(int Direction)
{
    directionMove = Direction;
                 if(Direction == 0) { timerMove.stop(); return; }
    timerMove.start(10);
}

private slots:
void slotMove()
{
                                angleRotationDegree += stepMove*directionMove;
    angleRotationFutureDegree = angleRotationDegree;

            angleRotation = angleRotationDegree*M_PI/180;
      angleRotationFuture = angleRotationDegree*M_PI/180;

    if(TypeWidget == 1)
    {
    if(angleRotation < angleRotationMin) angleRotation = angleRotationMin; 
    if(angleRotation > angleRotationMax) angleRotation = angleRotationMax;
    }
    update();

    if(ControlRotary) ControlRotary->setParam(ControlChannel, angleRotationDegree);
}

signals:
void signalAngleChanged(float angleDegree);

protected:
    void paintEvent(QPaintEvent* event) override { drawRotation(); }

    void resizeEvent(QResizeEvent *event) override    { sizeWidget = event->size().width(); setBaseSize(sizeWidget, sizeWidget); initWidget(); }
    void mousePressEvent(QMouseEvent* event) override { pointDirection = event->pos() - pointCenterDrawing; updateAngle(); }
    void mouseMoveEvent(QMouseEvent* event)  override { pointDirectionFuture = event->pos() - pointCenterDrawing; updateFutureAngle(); }
    void leaveEvent(QEvent* event)           override { pointDirectionFuture = pointDirection; updateFutureAngle(); }


};