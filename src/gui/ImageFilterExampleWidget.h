//
// Created by Sanqian on 16/7/7.
//

#ifndef OCLEXAMPLE_OCLEXAMPLEGUI_H
#define OCLEXAMPLE_OCLEXAMPLEGUI_H

#include <QtWidgets>
#include "src/imageFilters/ImageFilter.h"
#include "ImageWidget.h"

class ImageFilterExampleWidget: public QWidget {
Q_OBJECT

public:
    ImageFilterExampleWidget(QWidget* parent = Q_NULLPTR);

    ~ImageFilterExampleWidget(){
        if(imageFilter!= nullptr)
        {
            delete imageFilter;
        }
    }
private:
    void createMenu();
    void processImages();
    bool isFileValid(){ return  !objPath.isEmpty()&&(objPath.endsWith(".jpg")
                                                     || objPath.endsWith(".png")
                                                     || objPath.endsWith(".jpeg"));};
private slots:
    void open();

private:


    //Gui Data
    QMenuBar* menuBar;
    QString objPath = "";
    ImageWidget* beforeW;
    ImageWidget* afterW;

    //OGL Data

    //Image Filter
    ImageFilter* imageFilter = new ImageFilter();

};


#endif //OCLEXAMPLE_OCLEXAMPLEGUI_H
