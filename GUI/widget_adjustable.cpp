#include "widget_adjustable.h"
#include <qregularexpression.h>

WidgetAdjustable::WidgetAdjustable(QWidget* parent) : QWidget(parent)
{
}

void WidgetAdjustable::ChangeFont(int FontSize)
{
  QString Style = this->styleSheet();

  Style.replace(QRegularExpression("font: \\d+pt"), QString("font: %1pt").arg(FontSize));
  this->setStyleSheet(Style);
  this->resize(0,0);
}

void WidgetAdjustable::SlotSetWindowSize(int FontSize)
{
    ChangeFont(FontSize);
    QTimer::singleShot(10,this,&WidgetAdjustable::AdjustWindow);
}

void WidgetAdjustable::AdjustWindow()
{
    this->resize(0,0);
}

LabelAdjustable::LabelAdjustable(QWidget* parent) : QLabel(parent)
{
}

void LabelAdjustable::ChangeFont(int FontSize)
{
  QString Style = this->styleSheet();

  Style.replace(QRegularExpression("font: \\d+pt"), QString("font: %1pt").arg(FontSize));
  this->setStyleSheet(Style);
  this->resize(0,0);
}

void LabelAdjustable::SlotSetWindowSize(int FontSize)
{
    ChangeFont(FontSize);
    QTimer::singleShot(10,this,&LabelAdjustable::AdjustWindow);
}

void LabelAdjustable::AdjustWindow()
{
    this->resize(0,0);
}
