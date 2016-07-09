//
// Created by Sanqian on 16/7/7.
//

#ifndef OCLEXAMPLE_OCLEXAMPLEGUI_H
#define OCLEXAMPLE_OCLEXAMPLEGUI_H

#include <QtWidgets>
#include <src/misc/Meshloader.h>
#include <src/ImageFilters/ImageFilter.h>
#include "ImageWidget.h"

class CentralWidget: public QWidget {
Q_OBJECT

public:
    CentralWidget();

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
    Meshloader* loader = new Meshloader();

    //Image Filter
    ImageFilter* imageFilter = new ImageFilter();

};


#endif //OCLEXAMPLE_OCLEXAMPLEGUI_H
