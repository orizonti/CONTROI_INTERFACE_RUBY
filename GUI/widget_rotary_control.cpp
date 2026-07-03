#include "widget_rotary_control.h"
#include "debug_output_filter.h"

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
    sizeDrawing = sizeWidget*0.8;
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


void WidgetRotaryControl::slotSetState(float angleDegree)
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
        if(pos_y_radial > 0)  angleRotation = -angleRotation; 

        if(TypeWidget == 0)
        {
           if(angleRotation < 0) angleRotation = 2*M_PI + angleRotation;
                                 angleRotationDegree = angleRotation*180/M_PI; calcStep();
        }

        if(TypeWidget == 1)
        {
        if(angleRotation < angleRotationMin) angleRotation = angleRotationMin; 
        if(angleRotation > angleRotationMax) angleRotation = angleRotationMax;
                                angleRotationDegree = angleRotation*180/M_PI; calcStep();
        }

        if(TypeWidget == 0)
          angleRotationShiftedDegree = angleRotationDegree;

                            angleRotationDeviceDegree = angleRotationShiftedDegree;
        if(TypeWidget == 1) angleRotationDeviceDegree = angleRotationDegree;

        if(ControlRotary) ControlRotary->setParam(ControlChannel, angleRotationDeviceDegree);
        //if(allowSignals ) emit signalStateChanged(angleRotationDegree);
        NodeSynchronizer.synchronizePeers();

        update();  
}

void WidgetRotaryControl::slotShift()
{
                                angleRotationDegree += stepMove*directionMove;
    angleRotationFutureDegree = angleRotationDegree;
    qDebug() << "ANGLE ROTATION DEGREE" << angleRotationDegree;

            angleRotation = angleRotationDegree*M_PI/180;
      angleRotationFuture = angleRotationDegree*M_PI/180;

    if(TypeWidget == 1)
    {
    if(angleRotation < angleRotationMin) angleRotation = angleRotationMin; 
    if(angleRotation > angleRotationMax) angleRotation = angleRotationMax;
    }

    if(TypeWidget == 0)
       angleRotationShiftedDegree = angleRotationDegree;

                        angleRotationDeviceDegree = angleRotationShiftedDegree;
    if(TypeWidget == 1) angleRotationDeviceDegree = angleRotationDegree;
}

void WidgetRotaryControl::slotMove()
{
    slotShift();
    if(ControlRotary) ControlRotary->setParam(ControlChannel, angleRotationDeviceDegree); update();
}

void WidgetRotaryControl::calcStep()
{
    angleRotationDegreeStep1 = std::abs(angleRotationDegree - angleRotationLastDegree); 
    angleRotationDegreeStep2 = std::abs(360 - angleRotationDegreeStep1); 
    angleRotationDegreeStep = angleRotationDegreeStep1 < angleRotationDegreeStep2 ? angleRotationDegreeStep1 
                                                                                : angleRotationDegreeStep2;
    StepDirection = angleRotationDegree > angleRotationLastDegree ? 1 : -1; 
    StepDirection = angleRotationDegreeStep1 < angleRotationDegreeStep2 ? StepDirection
                                                                        : StepDirection*-1;
    angleRotationDegreeStep *= StepDirection;
    angleRotationLastDegree = angleRotationDegree;
}

void WidgetRotaryControl::setNull(float Null)
{
    angleRotationDegree = Null;
    angleRotationDeviceDegree = Null;
    angleRotation = angleRotationDegree*M_PI/180;

    if(ControlRotary) ControlRotary->setParam(ControlChannel, angleRotationDeviceDegree);
    update();
}

void WidgetRotaryControl::setNullDevice(float Null) { angleRotationDegree = Null; }

void WidgetRotaryControl::updateFutureAngle()
{
        pos_x_radial_future = pointDirectionFuture.x();
        pos_y_radial_future = pointDirectionFuture.y();

           radius_cursor_future = std::hypot(pos_x_radial_future, pos_y_radial_future);
        if(radius_cursor_future > sizeDrawing/2) return;

        angleRotationFuture = std::acos(pos_x_radial_future/radius_cursor_future); 
           if(pos_y_radial_future > 0) angleRotationFuture = -angleRotationFuture;

        if(TypeWidget == 1)
        {
        if(angleRotationFuture < angleRotationMin) angleRotationFuture = angleRotationMin; 
        if(angleRotationFuture > angleRotationMax) angleRotationFuture = angleRotationMax;
        }

        if(angleRotationFuture < 0) angleRotationFuture = 2*M_PI + angleRotationFuture;
                                    angleRotationFutureDegree = angleRotationFuture*180/M_PI;

        update();  
}

void WidgetRotaryControl::drawRotation()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(pen1);
    if(TypeWidget == 0) painter.drawEllipse(rectDrawing);
    if(TypeWidget == 1) painter.drawArc(rectDrawing, angleRotationMinDegree*16, angleSpanDegree*16);

    //if(TypeWidget == 0) drawObjects(&painter);

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

void WidgetRotaryControl::slotMoveStart(int Direction)
{
    directionMove = Direction;
                 if(Direction == 0) { timerMove.stop(); return; }
    timerMove.start(2);
}


void WidgetRotaryControl::slotCheckState()
{
    if(!ControlRotary) return;
    switch(ControlChannel)
    {
        case 0: slotSetState(ControlRotary->getPair().first);  break;
        case 1: slotSetState(ControlRotary->getPair().second); break;
    }
}

//void WidgetRotaryControl::synchronizePeer(WidgetRotaryControl* ControlDevice)
//{
//   connect(this, &WidgetRotaryControl::signalStateChanged, ControlDevice, &WidgetRotaryControl::slotSetState);
//}

//    QObject::connect(&timerCheckState, SIGNAL(timeout()), this, SLOT(slotCheckState()));
//    timerCheckState.start(30);