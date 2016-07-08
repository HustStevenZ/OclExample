//
// Created by Sanqian on 16/7/8.
//

#include <QPainter>
#include "ImageWidget.h"
void ImageWidget::paintEvent(QPaintEvent *event) {
    if(image!=Q_NULLPTR)
    {
        QPainter painter;
        painter.begin(this);
        painter.drawImage(this->rect(),*image);
        painter.end();
    }
}