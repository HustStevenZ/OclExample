//
// Created by Sanqian on 16/7/7.
//

#include "ImageFilterExampleWidget.h"
#include "ImageWidget.h"
#include <QWidget>
//#include <QOpenGLWidget>

ImageFilterExampleWidget::ImageFilterExampleWidget(QWidget* parent) {
    setWindowTitle(tr("OpenCL Image Filter Examples"));

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

void ImageFilterExampleWidget::createMenu()
{

   menuBar = new QMenuBar(this);
    menuBar->setVisible(true);
    QMenu *_2dMenu = menuBar->addMenu(tr("&File"));
    QMenu *_FilterMenu = menuBar->addMenu(tr("&Filter"));
//    QMenu *_filterMenu = menuBar->addMenu(tr("&Image Filter"));
//    menuBar->setFixedSize(820,20);
    menuBar->setEnabled(true);
    menuBar->setNativeMenuBar(false);//Try to bypass the mac os display menu problem
    QAction *openAction = new QAction(tr("&Open"),_2dMenu);
    _2dMenu->addAction(openAction);
    connect(openAction,&QAction::triggered,this,&ImageFilterExampleWidget::open);

    QAction *blurAction = new QAction(tr("&Blur"),_FilterMenu);
    _FilterMenu->addAction(blurAction);

    connect(blurAction,&QAction::triggered,this,&ImageFilterExampleWidget::blurImage);

    QAction *sharpAction = new QAction(tr("&Sharping"),_FilterMenu);
    _FilterMenu->addAction(sharpAction);

    connect(sharpAction,&QAction::triggered,this,&ImageFilterExampleWidget::sharpingImage);

    QAction *embossAction = new QAction(tr("&Embossing"),_FilterMenu);
    _FilterMenu->addAction(embossAction);

    connect(embossAction,&QAction::triggered,this,&ImageFilterExampleWidget::embossinImage);

}

void ImageFilterExampleWidget::sharpingImage() {
    curFilter = SHARPING;
    processImages();
}

void ImageFilterExampleWidget::blurImage() {
    curFilter = BLUR;
    processImages();
}

void ImageFilterExampleWidget::embossinImage() {
    curFilter = EMBOSSING;
    processImages();
}
void ImageFilterExampleWidget::open() {
//    this->objPath=QFileDialog::getOpenFileName(this,"Open Image File",QDir::homePath(),"Image files (*.png *.jpg *.jpeg)");

    this->objPath = "";
    QStringList filenames;
    QFileDialog* fileDialog= new QFileDialog(this,"Open Image File",QDir::homePath(),"Image files (*.png *.jpg *.jpeg)");
    if(fileDialog->exec())
    {
        filenames = fileDialog->selectedFiles();
        fileDialog->hide();
        fileDialog->setHidden(true);
        delete fileDialog;
    }
    if(!filenames.isEmpty())
    {
        this->objPath = filenames[0];
    }
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

void ImageFilterExampleWidget::processImages() {
    if(isFileValid()) {
//        setWindowTitle(QString::fromStdString(this->imageFilter->testHelloWorld()));

        QImage* image = nullptr;
        switch(curFilter)
        {
            case BLUR:
                image=imageFilter->blurImage(new QImage(objPath));
                break;
            case SHARPING:
                image=imageFilter->sharpingImage(new QImage(objPath));
                break;
            case EMBOSSING:
                image=imageFilter->embossingImage(new QImage(objPath));
                break;
            default:
                break;
        }

        if(image!= nullptr)
        {
            afterW->setFixedSize(400,image->height()*400/image->width());
            afterW->setImage(image);
            afterW->repaint();}
//    }
    }
}
