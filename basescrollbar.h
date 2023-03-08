#ifndef BASESCROLLBAR_H
#define BASESCROLLBAR_H

#include <QScrollBar>

class BaseScrollBar : public QScrollBar
{
public:
    BaseScrollBar(Qt::Orientation orientation,
                  QWidget *parent = nullptr);
};

#endif // BASESCROLLBAR_H
