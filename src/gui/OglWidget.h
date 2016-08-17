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
//#include <objLoader.h>
//#include <glm.h>
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
//    void loadModel(objLoader* loader);
//    void loadModel(GLMmodel* model);
    void loadModel(aiScene* scene);
    void loadModel(QString filePath);

signals:
    void clicked();
    void displayChanged(QImage* image);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:


    struct OglMeshInfo{
        QVector<GLfloat> verticeData;
        QMap<int,QOpenGLTexture*> textures;
        QMatrix4x4 transform;
    };

    void makeObject();
//    void setVertices(objLoader* objInfo);
//    void setVertices(GLMmodel* objInfo);

    void updateParent();

    void reinitVbo();
    void reinitVbo(QVector<GLfloat>& vertices);
    void drawSingle();
    void drawSingle(QMatrix4x4& transformation);
    void drawMesh(OglMeshInfo* mesh);

    void drawAssimpScene();
    void drawAssimpNode(aiNode* node,aiMatrix4x4 transform);
    void drawAssimpMesh(aiMesh* mesh,aiMatrix4x4& transformation);

    void prepareScene();
    void prepareNodes(aiNode* node,aiMatrix4x4 transform);
    void prepareMesh(aiMesh* mesh,aiMatrix4x4& transformation);

    void releaseMesh();
    QColor clearColor;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
    bool doPaint = false;
    bool useAssImpScene = false;
    aiScene* scene = nullptr;


    QVector<GLfloat> verticeData;
    QMap<int,QOpenGLTexture*> textures;

    QVector<OglMeshInfo*> meshVec;
    QOpenGLShaderProgram *program;
    QOpenGLShader* vshader;
    QOpenGLShader* fshader;
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    QString modelFilePath;
//    QOpenGLTexture* textures;

};


#endif //OCLEXAMPLE_OGLWIDGET_H
