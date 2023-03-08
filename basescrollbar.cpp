#include "basescrollbar.h"

BaseScrollBar::BaseScrollBar(Qt::Orientation orientation,
                             QWidget *parent)
    : QScrollBar(parent)
{
    this->setOrientation(orientation);
}
