#include "label_active_image.h"
#include "qevent.h"
#include "qdebug.h"

LabelActiveImage::LabelActiveImage(QWidget *parent)
	: LabelAdjustable(parent)
{
}

LabelActiveImage::~LabelActiveImage()
{
}

void LabelActiveImage::mouseDoubleClickEvent(QMouseEvent * ev)
{
	if(ev->button() != Qt::RightButton) return;
	emit signalLabelPicked();
}

void LabelActiveImage::mousePressEvent(QMouseEvent * ev)
{

	if(ev->button() == Qt::LeftButton)
	{
	   emit signalLabelPicked();
	}

	if(ev->button() == Qt::RightButton)
	{
		PosPicked.first  = ev->position().x()*2;
		PosPicked.second = ev->position().y()*2;
		qInfo() <<"[ LABEL ]"<< "[ POS PRESS ]" << PosPicked.first << PosPicked.second;
	    emit signalPosPicked(PosPicked);
	}

}

void SinkDisplayLabel::setImageFrom(SourceImageDisplayInterface* Src)
{
	Src->getImageToDisplay(DisplayImage);	

	if(!LabelDisplay   ) {  return; }; 
	if(!FlagPlotEnabled) { LabelDisplay->setPixmap(QPixmap::fromImage(DisplayImage)); return; }

	auto posPicked = LabelDisplay->getPos();
	
	QPen pen; pen.setWidth(2); pen.setColor(Qt::red);
	Painter.begin(&DisplayImage);
	Painter.setPen(pen);
	Painter.drawRect(CoordRect.first, CoordRect.second, SizeRect.first, SizeRect.second);
	Painter.end();

	LabelDisplay->setPixmap(QPixmap::fromImage(DisplayImage)); 
};

void SinkDisplayLabel::linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device)    
{
	QObject::connect(LabelDisplay, &LabelActiveImage::signalPosPicked, [Device](std::pair<float,float> Coord) 
	{
       Device->setPair(Coord);
	});

}
