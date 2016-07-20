//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_IMAGEWIDGET_H
#define OCLEXAMPLE_IMAGEWIDGET_H


#include <QWidget>

class ImageWidget: public QWidget {
    Q_OBJECT
public:
    ImageWidget(QWidget* parant=Q_NULLPTR):QWidget(parant){};


    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    void setImage(QImage *image){this->image = image;}
    void clearImage(){
        if(this->image!=Q_NULLPTR)
        {
            delete this->image;
            this->image = Q_NULLPTR;
        }
    }
private:
    QImage* image = Q_NULLPTR;

};


#endif //OCLEXAMPLE_IMAGEWIDGET_H
