//
// Created by Sanqian on 16/7/14.
//

#include "OglWidget.h"
#include "OglExampleWidget.h"
#include <QMouseEvent>
#include <iostream>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <QPainter>

#endif
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
    releaseMesh();
//    for (int i = 0; i < 6; ++i)
//        delete textures[i];
    for(int i = 0;i<textures.size();i++)
    {
        delete textures[i];
    }
    if(this->scene!= nullptr)
    {
        delete scene;
    }
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

    GLuint FramebufferName = 0;

    glGenFramebuffers(1, &frameBufferName);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferName);


    glGenTextures(1,&renderTexId);

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

void OglWidget::drawMesh(OglMeshInfo *mesh) {
    if(mesh!= nullptr)
    {
        int width = 0;
        int height = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_WIDTH,&width);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_HEIGHT,&height);


        glBindTexture(GL_TEXTURE_2D,renderTexId);
//        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8UI, width, height, 0,GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glBindTexture(GL_TEXTURE_2D,0);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexId, 0);
        QMatrix4x4 transformation = mesh->transform;
//        QMap<int,QOpenGLTexture*>& textures = mesh->textures;
        QVector<GLfloat> &verticeData = mesh->verticeData;
        transformation.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
        transformation.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
        transformation.rotate(zRot / 16.0f, 0.0f, 0.0f, 1.0f);
        reinitVbo(verticeData);
        program->addShader(vshader);
        program->addShader(fshader);
        program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
        program->bindAttributeLocation("normal", PROGRAM_NORMAL_ATTRIBUTE);
        program->bindAttributeLocation("texCoord",PROGRAM_TEXCOORD_ATTRIBUTE);
        program->link();
        program->bind();
        program->setUniformValue("texture", 0);


        program->setUniformValue("matrix", transformation);
        program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

        program->enableAttributeArray(PROGRAM_NORMAL_ATTRIBUTE);
        program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
//        glVertexAttribPointer(PROGRAM_VERTEX_ATTRIBUTE,3,GL_FLOAT,GL_FALSE,0,0);
        program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 8 * sizeof(GLfloat));
        program->setAttributeBuffer(PROGRAM_NORMAL_ATTRIBUTE,GL_FLOAT,3* sizeof(GLfloat),3,8*sizeof(GLfloat));
        program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE,GL_FLOAT,6*sizeof(GLfloat),2,8* sizeof(GLfloat));


        for(int i=0; i<mesh->textures.size();i++)
        {
            if(mesh->textures[i]!= nullptr)
            {
                mesh->textures[i]->bind();
            }
        }

//        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture->textureId(), 0);

//        renderedTexture->bind();
        glDrawArrays(GL_TRIANGLES, 0, verticeData.size()/8);
    }
}

void OglWidget::drawSingle(QMatrix4x4 &transformation) {

//        glFrontFace(GL_CW);
//        glCullFace(GL_BACK);
//        glDisable(GL_CULL_FACE);

    transformation.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    transformation.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    transformation.rotate(zRot / 16.0f, 0.0f, 0.0f, 1.0f);
    reinitVbo(verticeData);
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("normal", PROGRAM_NORMAL_ATTRIBUTE);
    program->bindAttributeLocation("texCoord",PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();
    program->bind();
    program->setUniformValue("texture", 0);


    program->setUniformValue("matrix", transformation);
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

    glDrawArrays(GL_TRIANGLES, 0, verticeData.size()/8);
}

void OglWidget::drawSingle() {

    QMatrix4x4 m;
//        m.setToIdentity();
    m.ortho(-1.0f, +1.0f, +1.0f, -1.0f, -14.0f, 15.0f);
//        m.translate(0.0f, 0.0f, -10.0f);



    drawSingle(m);

}


void OglWidget::drawAssimpNode(aiNode *node,aiMatrix4x4 transform) {

    if(node == nullptr)
        return;

    for(int i =0;i<node->mNumMeshes;i++)
    {
        int meshIndex = node->mMeshes[i];
        aiMesh* mesh = scene->mMeshes[meshIndex];
        aiMatrix4x4 meshTransform = transform*node->mTransformation.Transpose();

        drawAssimpMesh(mesh,meshTransform);
    }

    for(int i = 0;i<node->mNumChildren;i++)
    {
        drawAssimpNode(node->mChildren[i],transform*node->mTransformation.Transpose());
    }
}


void OglWidget::drawAssimpMesh(aiMesh *mesh,aiMatrix4x4& transformation) {
    if(mesh!= nullptr)
    {

        int materiaIndex = mesh->mMaterialIndex;
        aiMaterial* material = scene->mMaterials[materiaIndex];
        //Prepare vertice
        verticeData.clear();
        for(int i = 0;i<mesh->mNumFaces;i++)
        {
             //Process each face
            aiFace* face= mesh->mFaces+i;
            for(int j = 0;j<face->mNumIndices;j++) {
                //Process each Vertex
                int verIndex = face->mIndices[j];

                //Position
                aiVector3D vertexPosition = mesh->mVertices[verIndex];
                verticeData.append(vertexPosition.x);
                verticeData.append(vertexPosition.y);
                verticeData.append(vertexPosition.z);

                //Normal
                aiVector3D vertexNorm = mesh->mNormals[verIndex];
                verticeData.append(vertexNorm.x);
                verticeData.append(vertexNorm.y);
                verticeData.append(vertexNorm.z);

                //TexCoord
                //Assume that we only use tex 0
                if (mesh->mTextureCoords[0] != nullptr)
                {
                    aiVector3D vertexTexCoord = mesh->mTextureCoords[0][verIndex];
                    verticeData.append(vertexTexCoord.x);//u
                    verticeData.append(vertexTexCoord.y);//v
                }
                else
                {
                    verticeData.append(0.0f);//u
                    verticeData.append(0.0f);//v
                }

            }

        }

        //Preapre texture
        for(int i = 0;i<textures.size();i++)
        {
            delete textures[i];
            textures.remove(i);
        }

        if(scene->mMaterials!= nullptr)
        {
            int texIndex = 0;
            aiReturn texFound = AI_SUCCESS;

            aiString path;	// filename

            while (texFound == AI_SUCCESS)
            {
                texFound = material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
                if(texFound==AI_SUCCESS)
                {
                    QString filepath(path.data);
                    QFile file(filepath);
                    if(!file.exists())
                    {
                        filepath = this->modelFilePath.left(modelFilePath.lastIndexOf("/"))+"/"+path.data;
                    }
                    QImage *image = new  QImage(filepath);
                    textures.insert(texIndex,new QOpenGLTexture(*image)); //fill map with textures, pointers still NULL yet
                    textures[texIndex]->create();
                    texIndex++;
                }
            }
        }

        QMatrix4x4 m(transformation.a1,transformation.a2,transformation.a3,transformation.a4,
                     transformation.b1,transformation.b2,transformation.b3,transformation.b4,
                     transformation.c1,transformation.c2,transformation.c3,transformation.c4,
                     transformation.d1,transformation.d2,transformation.d3,transformation.d4);


        drawSingle(m);
    }
}
void OglWidget::drawAssimpScene() {
    if(scene!= nullptr)
    {

        aiMatrix4x4 identity;
        drawAssimpNode(scene->mRootNode,identity);
    }
}
void OglWidget::paintGL()
{

    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(doPaint) {

        if(useAssImpScene = true && meshVec.size()>0)
        {
            for(OglMeshInfo* meshInfo:meshVec)
            {
                drawMesh(meshInfo);
            }
//            drawAssimpScene();
        }
        else
        {
            drawSingle();
        }

    }

    updateParent();
}
void OglWidget::resizeGL(int width, int height)
{
    if (height==0)
    {
        // Making Height Equal One
        height=1;
    }

    int side = qMin(width, height);
//    glViewport((width - side) / 2, (height - side) / 2, side, side);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();							// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
    glLoadIdentity();
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

void OglWidget::loadModel(aiScene *scene) {
//    setVertices(scene);
//    setTextures(scene);
    useAssImpScene = true;

//    prepareMesh();
    if(this->scene!= nullptr)
    {
        delete this->scene;
        this->scene = nullptr;
    }
    this->scene = scene;
    prepareScene();
    doPaint = true;
    update();
}

void OglWidget::prepareScene() {
    if(scene!= nullptr)
    {

        releaseMesh();
        aiMatrix4x4 identity;
        prepareNodes(scene->mRootNode,identity);
    }
}

void OglWidget::prepareNodes(aiNode *node, aiMatrix4x4 transform) {

    if(node == nullptr)
        return;

    for(int i =0;i<node->mNumMeshes;i++)
    {
        int meshIndex = node->mMeshes[i];
        aiMesh* mesh = scene->mMeshes[meshIndex];
        aiMatrix4x4 meshTransform = transform*node->mTransformation.Transpose();

        prepareMesh(mesh,meshTransform);
    }

    if(node->mNumChildren==0)
        return;
    for(int i = 0;i<node->mNumChildren;i++)
    {
        prepareNodes(node->mChildren[i],transform*node->mTransformation.Transpose());
    }
}

void OglWidget::prepareMesh(aiMesh *mesh, aiMatrix4x4 &transformation) {

    if(mesh!= nullptr)
    {

        OglMeshInfo* oglMesh = new OglMeshInfo;
        int materiaIndex = mesh->mMaterialIndex;
        aiMaterial* material = scene->mMaterials[materiaIndex];

        QMap<int,QOpenGLTexture*>& meshTextures = oglMesh->textures;
        //Prepare vertice
        QVector<GLfloat>& meshverticeData = oglMesh->verticeData;

        for(int i = 0;i<mesh->mNumFaces;i++)
        {
            //Process each face
            aiFace* face= mesh->mFaces+i;
            for(int j = 0;j<face->mNumIndices;j++) {
                //Process each Vertex
                int verIndex = face->mIndices[j];

                //Position
                aiVector3D vertexPosition = mesh->mVertices[verIndex];
                meshverticeData.append(vertexPosition.x);
                meshverticeData.append(vertexPosition.y);
                meshverticeData.append(vertexPosition.z);

                //Normal
                aiVector3D vertexNorm = mesh->mNormals[verIndex];
                meshverticeData.append(vertexNorm.x);
                meshverticeData.append(vertexNorm.y);
                meshverticeData.append(vertexNorm.z);

                //TexCoord
                //Assume that we only use tex 0
                if (mesh->mTextureCoords[0] != nullptr)
                {
                    aiVector3D vertexTexCoord = mesh->mTextureCoords[0][verIndex];
                    meshverticeData.append(vertexTexCoord.x);//u
                    meshverticeData.append(vertexTexCoord.y);//v
                }
                else
                {
                    meshverticeData.append(0.0f);//u
                    meshverticeData.append(0.0f);//v
                }

            }

        }

        //Preapre texture
        for(int i = 0;i<textures.size();i++)
        {
            delete textures[i];
            textures.remove(i);
        }

        if(scene->mMaterials!= nullptr)
        {
            int texIndex = 0;
            aiReturn texFound = AI_SUCCESS;

            aiString path;	// filename

            while (texFound == AI_SUCCESS)
            {
                texFound = material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
                if(texFound==AI_SUCCESS)
                {
                    QString filepath(path.data);
                    QFile file(filepath);
                    if(!file.exists())
                    {
                        filepath = this->modelFilePath.left(modelFilePath.lastIndexOf("/"))+"/"+path.data;
                    }
                    QImage *image = new  QImage(filepath);
                    meshTextures.insert(texIndex,new QOpenGLTexture(*image)); //fill map with textures, pointers still NULL yet
                    meshTextures[texIndex]->create();
                    texIndex++;
                }
            }
        }

        QMatrix4x4 m(transformation.a1,transformation.a2,transformation.a3,transformation.a4,
                     transformation.b1,transformation.b2,transformation.b3,transformation.b4,
                     transformation.c1,transformation.c2,transformation.c3,transformation.c4,
                     transformation.d1,transformation.d2,transformation.d3,transformation.d4);

        oglMesh->transform = m;
        this->meshVec.push_back(oglMesh);
    }
}

void OglWidget::reinitVbo(QVector<GLfloat>& vertices) {
    if(vbo.isCreated()) {
        vbo.destroy();
    }
    vbo.create();
    glBindBuffer(GL_ARRAY_BUFFER,vbo.bufferId());
    glBufferData(GL_ARRAY_BUFFER,vertices.size()* sizeof(GLfloat),vertices.constData(),GL_STATIC_DRAW);
}

void OglWidget::reinitVbo() {
    if(vbo.isCreated()) {
        vbo.destroy();
    }
    vbo.create();
    glBindBuffer(GL_ARRAY_BUFFER,vbo.bufferId());
    glBufferData(GL_ARRAY_BUFFER,verticeData.size()* sizeof(GLfloat),verticeData.constData(),GL_STATIC_DRAW);
}


void OglWidget::loadModel(QString filePath) {
    aiScene* scene = (aiScene*)aiImportFile(filePath.toStdString().c_str(),aiProcessPreset_TargetRealtime_MaxQuality);

    modelFilePath = filePath;
    loadModel(scene);
}

void OglWidget::updateParent() {

   if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {

        int width = 0;
        int height = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_WIDTH,&width);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_HEIGHT,&height);
        unsigned int* buffer = (unsigned int*)malloc(sizeof(unsigned int)*width*height);
        memset(buffer,0,width*height* sizeof(unsigned int));
//        if(!renderedTexture->isCreated())
//        {
//            renderedTexture->setSize(width,height,0);
//            renderedTexture->setFormat(QOpenGLTexture::TextureFormat::RGBAFormat);
//            renderedTexture->create();
//            renderedTexture->bind();
//            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
//            glBindTexture(GL_TEXTURE_2D,0);
//        }
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    glBindTexture(GL_TEXTURE_2D,renderedTexture);

        glTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_WIDTH,&width);
        glTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_HEIGHT,&height);
        int w,h;
        glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_WIDTH,&w);
        int err = glGetError();
     glBindTexture(GL_TEXTURE_2D,renderTexId);
    glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,buffer);

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
//       QPainter painter;
//       painter.begin(this);
//       painter.drawImage(this->rect(),*image);
//       painter.end();
//       emit displayChanged(renderedTexture);
       emit displayChanged(renderTexId,width,height);
//       emit displayChanged(image);
//            emit displayChanged(renderedTexture->textureId());
    }
}

void OglWidget::releaseMesh() {
    if(meshVec.size()>0)
    {
        for(OglMeshInfo* meshInfo:meshVec)
        {
            if(meshInfo!= nullptr && meshInfo->textures.size())
            {
                for(int i = 0;i<meshInfo->textures.size();i++)
                {
                    delete meshInfo->textures[i];
                }
                meshInfo->textures.clear();
            }
            delete meshInfo;
            meshInfo= nullptr;
        }
        meshVec.clear();
    }
}