#ifndef WIDGET_ADJUSTABLE_H
#define WIDGET_ADJUSTABLE_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QGraphicsScene>
#include <QRegularExpression>


class WidgetAdjustable : public QWidget
{
	Q_OBJECT
    public:
	WidgetAdjustable(QWidget* parent = 0);
    void ChangeFont(int FontSize);

    int WIDGET_FONT_SIZE = 12;

public slots:
void SlotSetWindowSize(int FontSize);
void AdjustWindow();
};

class LabelAdjustable : public QLabel
{
	Q_OBJECT

    public:
	LabelAdjustable(QWidget* parent);
    void ChangeFont(int FontSize);

    int WIDGET_FONT_SIZE = 12;

public slots:
void SlotSetWindowSize(int FontSize);
void AdjustWindow();
};



#endif // WidgetAdjustable_H