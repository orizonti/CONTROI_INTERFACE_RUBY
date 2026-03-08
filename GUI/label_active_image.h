#ifndef LABEL_IMAGE_SINK_CONTROL_H
#define LABEL_IMAGE_SINK_CONTROL_H
#include "widget_adjustable.h"
#include <atomic>
#include <QPixmap>
#include <QImage>
#include "interface_image_source.h"
#include <QPainter>
#include "device_generic_interface.h"


class LabelActiveImage : public LabelAdjustable
{
	Q_OBJECT

public:
	LabelActiveImage(QWidget *parent);
	~LabelActiveImage();
	int posX = 0;
	int posY = 0;
	std::pair<int,int> PosPicked;
	std::pair<int,int> getPos() { return std::pair<int,int>(posX,posY); }

protected:
	void mousePressEvent(QMouseEvent * ev);
    void mouseDoubleClickEvent(QMouseEvent * ev);

signals:
	void signalPosPicked(QPair<int,int> Pos);
	void signalLabelPicked();
};

class SinkDisplayLabel : public SinkDisplayNode
{
	Q_OBJECT
	public:
	LabelActiveImage* LabelDisplay = nullptr;
     SinkDisplayNode* NodeLinked = nullptr;
    void linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device);    

	void setImageFrom(SourceImageDisplayInterface* Src) override;
	void linkToDisplay(LabelActiveImage* Display) 
	{
		 LabelDisplay = Display; 
		 QObject::connect(LabelDisplay, &LabelActiveImage::signalLabelPicked, this, &SinkDisplayLabel::slotNodePicked, Qt::QueuedConnection);
	} 
	void linkToSinkNode(SinkDisplayNode* Node) { NodeLinked = Node; } 

	void enablePlot(bool OnOff) { FlagPlotEnabled = OnOff; }
	bool FlagPlotEnabled = false;

	  QImage DisplayImage;
	QPainter Painter;

	std::pair<float,float> CoordRect;
	std::pair<int  ,int  > SizeRect ;
	void setCoordPaint(std::pair<float,float> Coord) { CoordRect = Coord; }

	//std::vector<std::pair<float,float>> CoordsObjects;

	private slots:
	void slotNodePicked() { if(NodeLinked != nullptr) NodeLinked->linkToSource(this->GetSource()); }
	void slotPosPicked(std::pair<int,int> pos)  { qDebug() << "SINK DISPLAY POS: " << pos.first << pos.second;  }
}
;

#endif  
