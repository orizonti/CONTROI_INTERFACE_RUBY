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
	std::pair<float,float> PosPicked;
	std::pair<float,float> PosPickedAbs;
	std::pair<float,float> getPos() { return PosPicked; }

protected:
	void mousePressEvent(QMouseEvent * ev);
    void mouseDoubleClickEvent(QMouseEvent * ev);

signals:
	void signalPosPicked(QPair<float,float> Pos);
	void signalLabelPicked();
};

class SinkDisplayLabel : public SinkDisplayNode
{
	Q_OBJECT
	public:
	SinkDisplayLabel() 
	{ 
	       Rects.push_back(std::pair<float,float>(0,0));
	}
	LabelActiveImage* LabelDisplay = nullptr;
     SinkDisplayNode* NodeLinked = nullptr;
    void linkToDevice(std::shared_ptr<DeviceGenericHandleControl> Device);    

	void setImageFrom(SourceImageDisplayInterface* Src) override;
	void setImage(QImage image) override;
	void linkToDisplay(LabelActiveImage* Display) 
	{
		 LabelDisplay = Display; 
		 QObject::connect(LabelDisplay, &LabelActiveImage::signalLabelPicked, this, &SinkDisplayLabel::slotNodePicked, Qt::QueuedConnection);
	} 
	void linkToSinkNode(SinkDisplayNode* Node) { NodeLinked = Node; } 

	void enablePlot(bool OnOff) { FlagPlotEnabled = OnOff; }
	                         bool FlagPlotEnabled = false;
       
	  QImage DisplayImage;

	std::vector<std::pair<float,float>> Rects;

	void setCoordPaint(std::pair<float,float> Coord, int channel) 
	{
		if(Rects.size() <= channel) return;
		   Rects[channel] = Coord;  
	}

	private slots:
	void slotNodePicked() { if(NodeLinked != nullptr) NodeLinked->linkToSource(this->GetSource()); }
	void slotPosPicked(std::pair<int,int> pos)  { qDebug() << "SINK DISPLAY POS: " << pos.first << pos.second;  }
}
;

#endif  
