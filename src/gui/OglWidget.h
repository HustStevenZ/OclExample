//
// Created by Sanqian on 16/7/14.
//

#ifndef OCLEXAMPLE_OGLWIDGET_H
#define OCLEXAMPLE_OGLWIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <objLoader.h>
#include <glm.h>
#include <assimp/scene.h>

class OglWidget : public QOpenGLWidget, protected QOpenGLFunctions{

Q_OBJECT

public:
    explicit OglWidget(QWidget *parent = 0);
    ~OglWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);
    void loadModel(objLoader* loader);
    void loadModel(GLMmodel* model);
    void loadModel(aiScene* scene);
    void loadModel(QString filePath);

signals:
    void clicked();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void makeObject();
    void setVertices(objLoader* objInfo);
    void setVertices(GLMmodel* objInfo);
    void setVertices(aiScene* scene);
    void setTextures(aiScene* scene);


    void reinitVbo();

    QColor clearColor;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
    bool doPaint = false;
    QVector<GLfloat> verticeData;
    QOpenGLShaderProgram *program;
    QOpenGLShader* vshader;
    QOpenGLShader* fshader;
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    QMap<int,QOpenGLTexture*> textures;
    QString modelFilePath;
//    QOpenGLTexture* textures;

};


#endif //OCLEXAMPLE_OGLWIDGET_H
