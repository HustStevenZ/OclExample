//
// Created by Sanqian on 16/7/7.
//
#include <cstdio>
#include <QApplication>
#include <QWidget>
#include <src/gui/CentralWidget.h>

int main(int argc, char **argv) {

    QApplication app(argc, argv);
    CentralWidget widget;
    widget.show();
    return app.exec();
}
