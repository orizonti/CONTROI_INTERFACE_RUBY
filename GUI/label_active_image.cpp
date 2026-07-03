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
	//if(ev->button() != Qt::RightButton) return;
	//emit signalLabelPicked();
}

void LabelActiveImage::mousePressEvent(QMouseEvent * ev)
{

	if(ev->button() == Qt::LeftButton)
	{
	   emit signalLabelPicked();
	}

	if(ev->button() == Qt::RightButton)
	{
		const auto& LabelSize = this->size();
		PosPickedAbs.first  = ev->position().x();
		PosPickedAbs.second = ev->position().y();

		PosPicked.first  = PosPickedAbs.first/LabelSize.width();
		PosPicked.second = PosPickedAbs.second/LabelSize.height();

		if(ev->button() == Qt::LeftButton)  signalPosPicked(PosPicked);
		if(ev->button() == Qt::RightButton) signalPosPicked(PosPicked);
	}

}

void SinkDisplayLabel::setImage(QImage image) 
{
	DisplayImage = image.copy();
	LabelDisplay->setPixmap(QPixmap::fromImage(DisplayImage)); 
}

void SinkDisplayLabel::setImageFrom(SourceImageDisplayInterface* Src)
{
	Src->getImageToDisplay(DisplayImage);	

	if(!LabelDisplay   ) {  return; }; 
	if(!FlagPlotEnabled) { LabelDisplay->setPixmap(QPixmap::fromImage(DisplayImage)); return; }
	if(DisplayImage.isNull()) return;

	auto posPicked = LabelDisplay->getPos();
    QPointF centerCoord(DisplayImage.width()/2, DisplayImage.height()/2);
    QPointF corner(20,20);
	QPair<int,int> objectCoord{20,20};
	QString info = QString("%1 %2").arg(objectCoord.first).arg(objectCoord.second);
	
	QPen pen; pen.setWidth(2); pen.setColor(Qt::green);
	QPen pen2; pen2.setWidth(5); pen2.setColor(Qt::red);

	QPainter Painter;
	Painter.begin(&DisplayImage);
	Painter.setPen(pen);
	for(auto& rect_pos: Rects) 
	{ 
		Painter.drawRect(rect_pos.first-40, rect_pos.second-40, 80,80); ; 
		Painter.drawEllipse(rect_pos.first-4, rect_pos.second-4,8,8); ; 

	    Painter.setPen(pen2);
		Painter.drawPoint(centerCoord); ; 
		Painter.drawText(corner, info);
	}
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
