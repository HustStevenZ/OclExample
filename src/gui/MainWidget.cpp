//
// Created by Sanqian on 16/7/13.
//

#include <QVBoxLayout>
#include <QPushButton>
#include "MainWidget.h"
#include "ImageFilterExampleWidget.h"

MainWidget::MainWidget() {

    setWindowTitle(tr("OpenCL Examples"));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->setLayout(mainLayout);

    QPushButton* imageFilterButton = new QPushButton("Image Filter Example",this);
    mainLayout->addWidget(imageFilterButton);
    connect(imageFilterButton,&QPushButton::clicked,this,&MainWidget::imageFilterExamples);

    QPushButton* oglExampleButton = new QPushButton("OpenGL Example",this);
    mainLayout->addWidget(oglExampleButton);

}

void MainWidget::imageFilterExamples() {
    ImageFilterExampleWidget* widget = new ImageFilterExampleWidget(this);
    widget->show();
//    this->hide();
}