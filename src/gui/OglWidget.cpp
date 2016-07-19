//
// Created by Sanqian on 16/7/14.
//

#include "OglWidget.h"
#include <QMouseEvent>
#include <iostream>

OglWidget::OglWidget(QWidget *parent)
        : QOpenGLWidget(parent),
          clearColor(Qt::black),
          xRot(0),
          yRot(0),
          zRot(0),
          program(0)
{
//    memset(textures, 0, sizeof(textures));
}

OglWidget::~OglWidget()
{
    makeCurrent();
    vbo.destroy();
    ibo.destroy();
//    for (int i = 0; i < 6; ++i)
//        delete textures[i];
    delete program;
    doneCurrent();
}

QSize OglWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize OglWidget::sizeHint() const
{
    return QSize(200, 200);
}

void OglWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    update();
}

void OglWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    update();
}

void OglWidget::initializeGL()
{
    //initializeOpenGLFunctions();

//    makeObject();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_NORMAL_ATTRIBUTE 1
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

     vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    const char *vsrc =
            "attribute highp vec4 vertex;\n"
                    "attribute mediump vec4 texCoord;\n"
                    "attribute mediump vec4 normal;"
                    "varying mediump vec4 texc;\n"
                    "uniform mediump mat4 matrix;\n"
                    "void main(void)\n"
                    "{\n"
                    "    gl_Position = matrix * vertex;\n"
//                    "    gl_Normal = matrix * normal;\n"
                    "\n"
                    "}\n";
    vshader->compileSourceCode(vsrc);

    fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    const char *fsrc =
            "uniform sampler2D texture;\n"
                    "varying mediump vec4 texc;\n"
                    "void main(void)\n"
                    "{\n"
//                    "    gl_FragColor = texture2D(texture, texc.st);\n"
                    "gl_FragColor = vec4(255,255,0,0);\n"
                    "}\n";
    fshader->compileSourceCode(fsrc);

    program = new QOpenGLShaderProgram;

}

void OglWidget::paintGL()
{
    if(doPaint) {
        glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        glFrontFace(GL_CW);
//        glCullFace(GL_BACK);
        glDisable(GL_CULL_FACE);
        reinitVbo();
        program->addShader(vshader);
        program->addShader(fshader);
        program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
        program->bindAttributeLocation("normal", PROGRAM_NORMAL_ATTRIBUTE);
        program->link();
        program->bind();
        program->setUniformValue("texture", 0);

        QMatrix4x4 m;
        m.setToIdentity();
        m.ortho(-1.0f, +1.0f, +1.0f, -1.0f, -14.0f, 15.0f);
//        m.translate(0.0f, 0.0f, -10.0f);
        m.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
        m.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
        m.rotate(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

        program->setUniformValue("matrix", m);
        program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

        program->enableAttributeArray(PROGRAM_NORMAL_ATTRIBUTE);
        program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
        program->setAttributeBuffer(PROGRAM_NORMAL_ATTRIBUTE,GL_FLOAT,3* sizeof(GLfloat),3,6*sizeof(GLfloat));

        glDrawArrays(GL_TRIANGLES, 6, 3);
    }

}
void OglWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void OglWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void OglWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotateBy(8 * dy, 8 * dx, 0);
    } else if (event->buttons() & Qt::RightButton) {
        rotateBy(8 * dy, 0, 8 * dx);
    }
    lastPos = event->pos();
}

void OglWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit clicked();
}

void OglWidget::makeObject()
{
    static const int coords[6][4][3] = {
            { { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 } },
            { { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 } },
            { { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 } },
            { { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 } },
            { { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 } },
            { { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 } }
    };

   // for (int j = 0; j < 6; ++j)
     //   textures[j] = new QOpenGLTexture(QImage(QString(":/images/side%1.png").arg(j + 1)).mirrored());
//
    QVector<GLfloat> vertData;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            // vertex position
            verticeData.append(0.2 * coords[i][j][0]);
            verticeData.append(0.2 * coords[i][j][1]);
            verticeData.append(0.2 * coords[i][j][2]);
//            // texture coordinate
//            vertData.append(j == 0 || j == 3);
//            vertData.append(j == 0 || j == 1);
        }
    }

//    vbo.bind();

}

void OglWidget::setVertices(objLoader *objInfo) {
    if(objInfo!= nullptr) {
        verticeData.clear();
        int vertextCount = objInfo->vertexCount;
        QVector<GLfloat> tempvertData;
        QVector<GLfloat> normData;
        QVector<GLint> indiceData;
        QVector<GLint> normIndex;
        for (int i = 0; i < vertextCount; i++) {
            tempvertData.append((objInfo->vertexList[i])->e[0]);
            tempvertData.append((objInfo->vertexList[i])->e[1]);
            tempvertData.append((objInfo->vertexList[i])->e[2]);
        }
        for (int i = 0; i < objInfo->normalCount; i++)
        {
            normData.push_back(objInfo->normalList[i]->e[0]);
            normData.push_back(objInfo->normalList[i]->e[1]);
            normData.push_back(objInfo->normalList[i]->e[2]);
        }
        for(int i = 0;i<objInfo->faceCount;i++)
        {
            obj_face* face = objInfo->faceList[i];

            for(int j = 0;j<face->vertex_count;j++)
            {

                indiceData.append(face->vertex_index[j]);
            }
        }

        for(int i = 0;i<objInfo->faceCount;i++)
        {
            obj_face* face = objInfo->faceList[i];

            for(int j = 0;j<face->vertex_count;j++)
            {
                normIndex.append(face->normal_index[j]);
            }
        }
        for(int i = 0;i<indiceData.size();i++)
        {
            verticeData.push_back(tempvertData[indiceData[i]*3]);
//            std::cout<<"vertex: "<<tempvertData[indiceData[i]*3]<<","<<
//                    tempvertData[indiceData[i]*3+1]<<","<<
//                    tempvertData[indiceData[i]*3+2]<<","<<std::endl;
            verticeData.push_back(tempvertData[indiceData[i]*3+1]);
            verticeData.push_back(tempvertData[indiceData[i]*3+2]);

            if(objInfo->normalCount>0)
            {

            verticeData.push_back(normData[normIndex[i]*3]);
            verticeData.push_back(normData[normIndex[i]*3+1]);
            verticeData.push_back(normData[normIndex[i]*3+2]);
            }
            else
            {
                verticeData.push_back(1.0f);
                verticeData.push_back(0.0f);
                verticeData.push_back(0.0f);
            }

//
        }

    }
}

void OglWidget::setTexCoords(objLoader *objInfo) {

}

void OglWidget::setFaces(objLoader *objInfo) {
//
//    QVector<GLint> indiceData;
//    QVector<GLfloat> texcorData;
//
//    ibo.create();
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo.bufferId());
//    ibo.allocate(indiceData.constData(),indiceData.count()* sizeof(GLint));
}

void OglWidget::setMatierial(objLoader *objInfo) {

}
void OglWidget::loadModel(objLoader *loader) {
    setVertices(loader);
//    makeObject();
    doPaint = true;
    update();
}

void OglWidget::reinitVbo() {
    if(vbo.isCreated()) {
        vbo.destroy();
    }
    vbo.create();
    glBindBuffer(GL_ARRAY_BUFFER,vbo.bufferId());
    vbo.allocate(verticeData.constData(),verticeData.size());
}