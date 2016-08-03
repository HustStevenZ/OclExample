//
// Created by Sanqian on 16/7/14.
//

#include <QMenuBar>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include "OglExampleWidget.h"
#include <QAction>
#include "OglWidget.h"

OglExampleWidget::OglExampleWidget(QWidget *parant) {

    setWindowTitle("OpenGL Example");
    QHBoxLayout* mainlayout = new QHBoxLayout(this);
    setFixedSize(800,400);
    QMenuBar* menuBar = new QMenuBar(this);
    menuBar->setNativeMenuBar(false);
    mainlayout->setMenuBar(menuBar);
    QMenu* fileMenu = menuBar->addMenu(tr("File"));
    QAction* fileOp = new QAction("open");
    fileMenu->addAction(fileOp);
    connect(fileOp,&QAction::triggered,this,&OglExampleWidget::openMesh);

    QGroupBox* glGroupBox = new QGroupBox(this);
//
    QVBoxLayout* leftLayout = new QVBoxLayout(this);
    glGroupBox->setLayout(leftLayout);
    mainlayout->addWidget(glGroupBox);
//

    QGroupBox* controlGroupBox = new QGroupBox(this);
    QVBoxLayout* rightlayout = new QVBoxLayout(this);
    controlGroupBox->setLayout(rightlayout);
    mainlayout->addWidget(controlGroupBox);

    display = new OglWidget(this);
    QColor red(255,255,255);
    display->setClearColor(red);
    leftLayout->addWidget(display);

    oclDisplay = new ImageWidget(this);
    rightlayout->addWidget(oclDisplay);
    setLayout(mainlayout);

    connect(display,&OglWidget::displayChanged,this,&OglExampleWidget::updateOclWidget);
}

void OglExampleWidget::openMesh() {
    QString fileName = QFileDialog::getOpenFileName(this,"Open Mesh",QDir::homePath(),"*.obj");
    if(!fileName.isEmpty())
    {
        if(scene!= nullptr)
        {
            aiReleaseImport(scene);
//            delete scene;
            scene = nullptr;
        }

//        scene = (aiScene*)aiImportFile(fileName.toStdString().c_str(),aiProcessPreset_TargetRealtime_MaxQuality);
        display->loadModel(fileName);
//        meshloader = glmReadOBJ(fileName.toStdString().c_str());
//        //meshloader->load((char*)(fileName.toStdString().c_str()));
//        display->loadModel(meshloader);

    }
}



void OglExampleWidget::updateOclWidget(QImage *image) {
    if(oclDisplay!= nullptr)
    {
        oclDisplay->clearImage();
        QImage* processedimage = imageFilter->blurImage(image);

        oclDisplay->setImage(processedimage);
        oclDisplay->repaint();
    }
}