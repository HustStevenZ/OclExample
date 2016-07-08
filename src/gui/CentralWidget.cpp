//
// Created by Sanqian on 16/7/7.
//

#include "CentralWidget.h"
#include <QWidget>
#include <QOpenGLWidget>

CentralWidget::CentralWidget() {
    setWindowTitle(tr("OpenCL Examples"));

    setFixedSize(820,500);
//    menuBar()->addMenu("open");
    QWidget *before = new QWidget(this);
    QWidget *after = new QWidget(this);
    QLabel *beforeLabel = new QLabel(tr("Before"));
    beforeLabel->setAlignment(Qt::AlignHCenter);
    QLabel *afterLabel = new QLabel(tr("After"));
    afterLabel->setAlignment(Qt::AlignHCenter);

    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->setVisible(true);
    menuBar->addMenu("open");
//    menuBar->setFixedSize(820,20);
    menuBar->setEnabled(true);
    menuBar->setNativeMenuBar(false);
//    menuBar->set

    before->setFixedSize(400,400);
    after->setFixedSize(400,400);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(menuBar);
    mainLayout->setMenuBar(menuBar);
    QGridLayout *layout = new QGridLayout;
//    layout->addWidget(menubar,0,0);
    layout->addWidget(before, 0, 0);
    layout->addWidget(after, 0, 1);
    layout->addWidget(beforeLabel, 1, 0);
    layout->addWidget(afterLabel, 1, 1);
    mainLayout->addLayout(layout);

}
