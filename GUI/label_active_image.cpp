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
	if(ev->button() != Qt::LeftButton) return;
	posX = ev->position().x()*2;
	posY = ev->position().y()*2;
	qInfo() <<"[ LABEL ]"<< "[ POS PRESS ]" << posX << posY;
	//qDebug() << "[LABEL SIZE]" << this->width() << this->height();

	emit signalLabelPicked();
	emit signalPosPicked(posX,posY);
}

void SinkDisplayLabel::setImageFrom(SourceImageDisplayInterface* Src)
{
	Src->getImageToDisplay(DisplayImage);	

	if(!LabelDisplay   ) {  return; }; 
	if(!FlagPlotEnabled) { LabelDisplay->setPixmap(QPixmap::fromImage(DisplayImage)); return; }

	auto posPicked = LabelDisplay->getPos();
	
	Painter.begin(&DisplayImage);
	Painter.end();

	LabelDisplay->setPixmap(QPixmap::fromImage(DisplayImage)); 
};

