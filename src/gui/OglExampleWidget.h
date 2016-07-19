//
// Created by Sanqian on 16/7/14.
//

#ifndef OCLEXAMPLE_OGLEXAMPLEWDIGET_H
#define OCLEXAMPLE_OGLEXAMPLEWDIGET_H


#include <QWidget>
#include <QOpenGLWidget>
#include "objLoader.h"
#include "OglWidget.h"
#include <glm.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class OglExampleWidget : public QWidget{
Q_OBJECT
public:
    OglExampleWidget(QWidget* parant = Q_NULLPTR);


private:
    void openMesh();
    OglWidget* display = nullptr;
//    objLoader* meshloader = nullptr;
//    GLMmodel* meshloader = nullptr;
    aiScene *scene = nullptr;


};


#endif //OCLEXAMPLE_OGLEXAMPLEWDIGET_H
