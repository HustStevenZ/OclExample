//
// Created by Sanqian on 16/7/7.
//
#include <cstdio>
#include <QApplication>
#include <QWidget>
#include <src/gui/ImageFilterExampleWidget.h>
#include <src/gui/MainWidget.h>

int main(int argc, char **argv) {

    QApplication app(argc, argv);
//    ImageFilterExampleWidget widget;
//    widget.show();
    MainWidget mainWindow;
    mainWindow.show();
    return app.exec();
}
