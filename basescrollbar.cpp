#include "basescrollbar.h"

BaseScrollBar::BaseScrollBar(QWidget *parent)
    : QScrollBar(parent)
{
    this->setOrientation(Qt::Vertical);
}
