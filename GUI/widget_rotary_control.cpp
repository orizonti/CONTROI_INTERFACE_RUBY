#include "widget_rotary_control.h"

WidgetRotaryControl::WidgetRotaryControl(QWidget* parent) : QWidget(parent) 
{
    setBaseSize(sizeWidget, sizeWidget);  // Set the size of the widget
    initWidget();
    this->setMouseTracking(true);
    QObject::connect(&timerMove, SIGNAL(timeout()), this, SLOT(slotMove()));
    ControlChannel = TypeWidget;

}

void WidgetRotaryControl::setType(TypeDraw Type) { TypeWidget = (int)Type; initWidget(); ControlChannel = TypeWidget; }

void WidgetRotaryControl::initWidget()
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

void WidgetRotaryControl::drawObjects(QPainter* painter)
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

void WidgetRotaryControl::drawRotation()
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

void WidgetRotaryControl::slotSetAngle(float angleDegree)
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

void WidgetRotaryControl::updateAngle()
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
        if(allowSignals ) emit signalAngleChanged(angleRotationDegree);

        update();  
}

void WidgetRotaryControl::updateFutureAngle()
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

void WidgetRotaryControl::slotMoveStart(int Direction)
{
    directionMove = Direction;
                 if(Direction == 0) { timerMove.stop(); return; }
    timerMove.start(10);
}

void WidgetRotaryControl::slotMove()
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
    if(allowSignals)  emit signalAngleChanged(angleRotationDegree);
}

void WidgetRotaryControl::slotCheckState()
{
    if(!ControlRotary) return;
    switch(ControlChannel)
    {
        case 0: slotSetAngle(ControlRotary->getPair().first);  break;
        case 1: slotSetAngle(ControlRotary->getPair().second); break;
    }
}

void WidgetRotaryControl::synchronizePeer(WidgetRotaryControl* ControlDevice)
{
   connect(this, &WidgetRotaryControl::signalAngleChanged, ControlDevice, &WidgetRotaryControl::slotSetAngle);
}

//    QObject::connect(&timerCheckState, SIGNAL(timeout()), this, SLOT(slotCheckState()));
//    timerCheckState.start(30);