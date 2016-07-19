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

class OglWidget : public QOpenGLWidget{

Q_OBJECT

public:
    explicit OglWidget(QWidget *parent = 0);
    ~OglWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);
    void loadModel(objLoader* loader);

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
    void setTexCoords(objLoader* objInfo);
    void setFaces(objLoader* objInfo);
    void setMatierial(objLoader* objInfo);

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

};


#endif //OCLEXAMPLE_OGLWIDGET_H
