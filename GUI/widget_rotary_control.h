#pragma once
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>
#include <qnamespace.h>
#include <QPen>
#include <QColor>
#include <cmath>

class WidgetRotaryControl : public QWidget 
{
    Q_OBJECT

public:
    enum class TypeDraw { TypeCircle = 0, TypeArc = 1 };
    WidgetRotaryControl(QWidget* parent = nullptr) : QWidget(parent) 
    {
        setWindowTitle("Watch Widget");
        setBaseSize(sizeWidget, sizeWidget);  // Set the size of the widget
        initWidget();
        this->setMouseTracking(true);
    }
    void setType(TypeDraw Type)
    {
        TypeWidget = (int)Type;
        initWidget();
    }

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

    float length_radial = 0;
    float length_radial_future = 0;
    //==========================================
    //DRAWING AREA
       int TypeWidget = 1;
       int sizeWidget = 160;
       int sizeDrawing = sizeWidget*0.6;
     QRect rectDrawing;
    QPoint pointCenterDrawing;
       int radiusDirection = sizeDrawing;

    QPen pen1{QColor(214, 136, 41)  , 3, Qt::SolidLine };
    QPen pen2{QColor(30, 221, 78)  , 2, Qt::SolidLine };
    QPen pen3{QColor(215, 50, 28), 2, Qt::DashLine};
    QPen pen4{QColor(215, 50, 28), 2, Qt::SolidLine};

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
        qDebug() << "SET ANGLE: " << angleDegree;
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
signals:
void signalCurrentAngle(float angleDegree);

protected:
    void paintEvent(QPaintEvent* event) override { drawRotation(); }

    void resizeEvent(QResizeEvent *event) override
    {
        sizeWidget = event->size().width();
        qDebug() << "RESIZE EVENT: " << sizeWidget;
        setBaseSize(sizeWidget, sizeWidget);

        initWidget();
    }

    void mousePressEvent(QMouseEvent* event) override 
    {

        pointDirection = event->pos() - pointCenterDrawing;
        pos_x_radial = pointDirection.x();
        pos_y_radial = pointDirection.y();
        length_radial = std::hypot(pos_x_radial, pos_y_radial);
        angleRotation = std::acos(pos_x_radial/length_radial); 

        if(TypeWidget == 1)
        {
        if(angleRotation < angleRotationMin) angleRotation = angleRotationMin; 
        if(angleRotation > angleRotationMax) angleRotation = angleRotationMax;
        }

                             angleRotationDegree = angleRotation*180/M_PI;
        if(pos_y_radial > 0) angleRotationDegree = 360 - angleRotationDegree;

        update();  
    }

    void mouseMoveEvent(QMouseEvent* event) override 
    {
        pointDirectionFuture = event->pos() - pointCenterDrawing;
        pos_x_radial_future = pointDirectionFuture.x();
        pos_y_radial_future = pointDirectionFuture.y();
        length_radial_future = std::hypot(pos_x_radial_future, pos_y_radial_future);
        angleRotationFuture = std::acos(pos_x_radial_future/length_radial_future); 

        if(TypeWidget == 1)
        {
        if(angleRotationFuture < angleRotationMin) angleRotationFuture = angleRotationMin; 
        if(angleRotationFuture > angleRotationMax) angleRotationFuture = angleRotationMax;
        }

        if(pos_y_radial_future > 0) angleRotationFuture = 2*M_PI - angleRotationFuture;
                                    angleRotationFutureDegree = angleRotationFuture*180/M_PI;

        update();  
    }

    void leaveEvent(QEvent* event) override 
    {
              angleRotationFuture = angleRotation; 
        angleRotationFutureDegree = angleRotationFuture*180/M_PI;

        update();  
    }

    void keyPressEvent(QKeyEvent *event) override
    {

         if(TypeWidget == 1)
         {
            if(event->key() == Qt::Key_Up) angleRotationDegree++; 
            if(event->key() == Qt::Key_Down) angleRotationDegree--; 
         }
         else 
         {
            if(event->key() == Qt::Key_Left) angleRotationDegree++; 
            if(event->key() == Qt::Key_Right) angleRotationDegree--; 
         }

         slotSetAngle(angleRotationDegree);
    }

};