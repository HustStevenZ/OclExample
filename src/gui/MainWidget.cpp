//
// Created by Sanqian on 16/7/13.
//

#include <QVBoxLayout>
#include <QPushButton>
#include "MainWidget.h"
#include "ImageFilterExampleWidget.h"
#include "OglExampleWidget.h"

MainWidget::MainWidget() {

    setWindowTitle(tr("OpenCL Examples"));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->setLayout(mainLayout);

    QPushButton* imageFilterButton = new QPushButton("Image Filter Example",this);
    mainLayout->addWidget(imageFilterButton);
    connect(imageFilterButton,&QPushButton::clicked,this,&MainWidget::imageFilterExamples);

    QPushButton* oglExampleButton = new QPushButton("OpenGL Example",this);
    mainLayout->addWidget(oglExampleButton);
    connect(oglExampleButton,&QPushButton::clicked,this,&MainWidget::oglExamples);
}

void MainWidget::imageFilterExamples() {
    ImageFilterExampleWidget* widget = new ImageFilterExampleWidget(this);
    widget->show();
//    this->hide();
}

void MainWidget::oglExamples() {
    OglExampleWidget* widget = new OglExampleWidget(this);
    widget->show();
}