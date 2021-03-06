//
// Created by Sanqian on 16/7/14.
//

#ifndef OCLEXAMPLE_OGLEXAMPLEWDIGET_H
#define OCLEXAMPLE_OGLEXAMPLEWDIGET_H


#include <QWidget>
#include <QOpenGLWidget>
//#include "objLoader.h"
#include "OglWidget.h"
#include "ImageWidget.h"
//#include <glm.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <src/imageFilters/ImageFilter.h>

class OglExampleWidget : public QWidget{
Q_OBJECT
public:
    OglExampleWidget(QWidget* parant = Q_NULLPTR);
    ~OglExampleWidget(){
        if(imageFilter!= nullptr)
        {
            delete imageFilter;
        }
    }
signals:
    void oglWidgetChanged(QImage* image);

public Q_SLOTS:
    void updateOclWidget(GLuint textureObj,int width,int height);
    void updateOclWidget(QOpenGLTexture* textureObj);
    void updateOclWidget(QImage* textureObj);
private:
    void openMesh();
    OglWidget* display = nullptr;
    ImageWidget* oclDisplay = nullptr;
//    objLoader* meshloader = nullptr;
//    GLMmodel* meshloader = nullptr;
    aiScene *scene = nullptr;

    //Image Filter
    ImageFilter* imageFilter = nullptr;
};


#endif //OCLEXAMPLE_OGLEXAMPLEWDIGET_H
