//
// Created by Sanqian on 16/7/7.
//

#include "CentralWidget.h"
#include "ImageWidget.h"
#include <QWidget>
#include <QOpenGLWidget>

CentralWidget::CentralWidget() {
    setWindowTitle(tr("OpenCL Examples"));

    setFixedSize(820,500);
    ImageWidget *before = new ImageWidget(this);
    ImageWidget *oclFiltered = new ImageWidget(this);
    beforeW=before;
    afterW=oclFiltered;
    QLabel *beforeLabel = new QLabel(tr("Before"));
    beforeLabel->setAlignment(Qt::AlignHCenter);
    QLabel *afterLabel = new QLabel(tr("Ocl"));
    afterLabel->setAlignment(Qt::AlignHCenter);

    createMenu();

    before->setFixedSize(400,400);
    oclFiltered->setFixedSize(400,400);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(this->menuBar);
    mainLayout->setMenuBar(this->menuBar);
    QGridLayout *layout = new QGridLayout;
//    layout->addWidget(menubar,0,0);
    layout->addWidget(before, 0, 0);
    layout->addWidget(oclFiltered, 0, 1);
    layout->addWidget(beforeLabel, 1, 0);
    layout->addWidget(afterLabel, 1, 1);
    mainLayout->addLayout(layout);

}

void CentralWidget::createMenu()
{

   menuBar = new QMenuBar(this);
    menuBar->setVisible(true);
    QMenu *fileMenu = menuBar->addMenu(tr("&File"));
//    menuBar->setFixedSize(820,20);
    menuBar->setEnabled(true);
    menuBar->setNativeMenuBar(false);//Try to bypass the mac os display menu problem
    QAction *openAction = new QAction(tr("&Open"),fileMenu);
    fileMenu->addAction(openAction);
    connect(openAction,&QAction::triggered,this,&CentralWidget::open);

}

void CentralWidget::open() {
    this->objPath=QFileDialog::getOpenFileName(this,"Open Image File",QDir::homePath(),"Image files (*.png *.jpg *.jpeg)");

    if(!isFileValid())
        this->objPath="";
    if(isFileValid())
    {
        QImage* image = new QImage(objPath);
        beforeW->setFixedSize(400,image->height()*400/image->width());
        beforeW->setImage(image);
        beforeW->repaint(); //call repaint event so that the widget redraws
        processImages();
    }
}

void CentralWidget::processImages() {
    if(isFileValid())
    {
//        setWindowTitle(QString::fromStdString(this->imageFilter->testHelloWorld()));
        QImage* image = imageFilter->filterImage(new QImage(objPath));

//        image->save(QDir::homePath()+"/test.png","png");
        afterW->setFixedSize(400,image->height()*400/image->width());
        afterW->setImage(image);
        afterW->repaint();
    }
}
