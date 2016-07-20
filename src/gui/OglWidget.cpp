//
// Created by Sanqian on 16/7/14.
//

#include "OglWidget.h"
#include "OglExampleWidget.h"
#include <QMouseEvent>
#include <iostream>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

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
    initializeOpenGLFunctions();


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_NORMAL_ATTRIBUTE 1
#define PROGRAM_TEXCOORD_ATTRIBUTE 2

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
                    "   texc=texCoord;\n"
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
                    "    gl_FragColor = texture2D(texture, texc.st);\n"
//                    "gl_FragColor = vec4(255,0,0,255);\n"
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
//        glDisable(GL_CULL_FACE);
        reinitVbo();
        program->addShader(vshader);
        program->addShader(fshader);
        program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
        program->bindAttributeLocation("normal", PROGRAM_NORMAL_ATTRIBUTE);
        program->bindAttributeLocation("texCoord",PROGRAM_TEXCOORD_ATTRIBUTE);
        program->link();
        program->bind();
        program->setUniformValue("texture", 0);

        QMatrix4x4 m;
//        m.setToIdentity();
        m.ortho(-1.0f, +1.0f, +1.0f, -1.0f, -14.0f, 15.0f);
//        m.translate(0.0f, 0.0f, -10.0f);
        m.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
        m.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
        m.rotate(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

        program->setUniformValue("matrix", m);
        program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

        program->enableAttributeArray(PROGRAM_NORMAL_ATTRIBUTE);
        program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
//        glVertexAttribPointer(PROGRAM_VERTEX_ATTRIBUTE,3,GL_FLOAT,GL_FALSE,0,0);
        program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 8 * sizeof(GLfloat));
        program->setAttributeBuffer(PROGRAM_NORMAL_ATTRIBUTE,GL_FLOAT,3* sizeof(GLfloat),3,8*sizeof(GLfloat));
        program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE,GL_FLOAT,6*sizeof(GLfloat),2,8* sizeof(GLfloat));

        for(int i; i<textures.size();i++)
        {
            if(textures[i]!= nullptr)
            {
                textures[i]->bind();
            }
        }

        glDrawArrays(GL_TRIANGLES, 0, verticeData.size()/3);

        updateParent();
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
    verticeData.clear();
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

void OglWidget::setVertices(GLMmodel *objInfo) {
    if(objInfo!= nullptr) {
        verticeData.clear();
        int vertextCount = objInfo->numvertices;
        QVector<GLfloat> tempvertData;
        QVector<GLfloat> normData;
        QVector<GLint> indiceData;
        QVector<GLint> normIndex;
        for (int i = 0; i < vertextCount; i++) {
            tempvertData.append((objInfo->vertices[i*3]));
            tempvertData.append((objInfo->vertices[i*3+1]));
            tempvertData.append((objInfo->vertices[i*3+1]));
        }
        for (int i = 0; i < objInfo->numnormals; i++)
        {
            normData.push_back(objInfo->normals[i*3]);
            normData.push_back(objInfo->normals[i*3+1]);
            normData.push_back(objInfo->normals[i*3+2]);
        }
        for(int i = 0;i<objInfo->numtriangles;i++)
        {
            GLMtriangle* face = objInfo->triangles+i;
            indiceData.append(face->vindices[0]);
            indiceData.append(face->vindices[1]);
            indiceData.append(face->vindices[2]);

            normIndex.append(face->nindices[0]);
            normIndex.append(face->nindices[1]);
            normIndex.append(face->nindices[2]);
        }

        for(int i = 0;i<indiceData.size();i++)
        {
            verticeData.push_back(tempvertData[indiceData[i]*3]);
            verticeData.push_back(tempvertData[indiceData[i]*3+1]);
            verticeData.push_back(tempvertData[indiceData[i]*3+2]);

            if(objInfo->numnormals>0)
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

void OglWidget::setVertices(aiScene *scene) {
    if(scene!= nullptr)
    {
        verticeData.clear();
        int meshNum = scene->mNumMeshes;
        for(int i = 0;i< scene->mNumMeshes;i++)
        {
            aiMesh* mesh = scene->mMeshes[i];
            for(int j = 0;j<mesh->mNumFaces;j++)
            {
                aiFace* face = mesh->mFaces+j;
                for(int k = 0;k < face->mNumIndices;k++)
                {
                    int index = face->mIndices[k];
                    aiVector3D* vertext = mesh->mVertices+index;
                    aiVector3D* norm = mesh->mNormals+index;
                    aiVector3D* texcoord = (mesh->mTextureCoords[0])+index;
                    verticeData.append(vertext->x);
                    verticeData.append(vertext->y);
                    verticeData.append(vertext->z);
                    verticeData.push_back(norm->x);
                    verticeData.push_back(norm->y);
                    verticeData.push_back(norm->z);
                    verticeData.push_back(texcoord->x);
                    verticeData.push_back(texcoord->y);
                }
            }
        }

    }
}

void OglWidget::loadModel(objLoader *loader) {
    setVertices(loader);
    doPaint = true;
    update();
}

void OglWidget::loadModel(GLMmodel *model) {
    setVertices(model);
    doPaint = true;
    update();
}

void OglWidget::loadModel(aiScene *scene) {
    setVertices(scene);
    setTextures(scene);
    doPaint = true;
    update();
}


void OglWidget::reinitVbo() {
    if(vbo.isCreated()) {
        vbo.destroy();
    }
    vbo.create();
    glBindBuffer(GL_ARRAY_BUFFER,vbo.bufferId());
    glBufferData(GL_ARRAY_BUFFER,verticeData.size()* sizeof(GLfloat),verticeData.constData(),GL_STATIC_DRAW);
}



void OglWidget::setTextures(aiScene *scene) {
    if(scene!= nullptr)
    {

        for(int i = 0;i<scene->mNumMaterials;i++)
        {
            aiMaterial* material = scene->mMaterials[i];
            int texIndex = 0;
            aiReturn texFound = AI_SUCCESS;

            aiString path;	// filename

            while (texFound == AI_SUCCESS)
            {
                texFound = scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
                QString texpath(path.data);
                QFile imageFile(texpath);
                if(!imageFile.exists())
                {
                    //Try to fullfill image path
                    if(!modelFilePath.isEmpty())
                    {
                        texpath = modelFilePath.left(modelFilePath.lastIndexOf("/"))+"/"+texpath;
                    }
                }
                QImage image(texpath);
                QOpenGLTexture* tex = new QOpenGLTexture(image);

                textures.insert(texIndex,tex);
                texIndex++;
            }

        }
    }
}

void OglWidget::loadModel(QString filePath) {
    aiScene* scene = (aiScene*)aiImportFile(filePath.toStdString().c_str(),aiProcessPreset_TargetRealtime_MaxQuality);

    modelFilePath = filePath;
    loadModel(scene);
}

void OglWidget::updateParent() {

    int width = 0;
    int height = 0;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_WIDTH,&width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_HEIGHT,&height);
    unsigned int* buffer = (unsigned int*)malloc(sizeof(unsigned int)*width*height);
    glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,buffer);//Read from lower left

    QImage* image = new QImage(width,height,QImage::Format_RGBA8888);
    for(int i = 0;i<width;i++)
        for(int j = 0;j<height;j++)
        {
            QColor color;
            int rgba = buffer[(height-1-j)*width+i];
            color.setRed((rgba&0xff000000)>>24);
            color.setGreen((rgba&0x00ff0000)>>16);
            color.setBlue((rgba&0x0000ff00)>>8);
            color.setAlpha((rgba&0x000000ff));
            image->setPixelColor(i,j,color);
        }
    emit displayChanged(image);
}
