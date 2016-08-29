//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_IMAGEFILTER_H
#define OCLEXAMPLE_IMAGEFILTER_H


#include <src/ocl/OclEngine.h>
#include <QImage>
#include <QOpenGLTexture>

class ImageFilter {

public:
    ImageFilter();
    ~ImageFilter(){
        if(_context!= nullptr)
        {
            delete _context;
        }
    }

    /**
     * TODO: Perhaps we may have time to isolate ImageFilter from QImage in the future
     */
    QImage* blurImage(QImage* image);

    QImage* sharpingImage(QImage* image);
    QImage* embossingImage(QImage* image);
    QImage* filterImage3x3(QImage* image,const float filter[]);


    QImage* blurImage(GLuint textureObj,int width,int height);

    QImage* sharpingImage(GLuint textureObj,int width,int height);
    QImage* embossingImage(GLuint textureObj,int width,int height);
    QImage* filterImage3x3(GLuint textureObj,const float filter[],int width,int height);


    QImage* blurImage(QOpenGLTexture* textureObj);

    QImage* sharpingImage(QOpenGLTexture*  textureObj);
    QImage* embossingImage(QOpenGLTexture*  textureObj);
    QImage* filterImage3x3(QOpenGLTexture*  textureObj,const float filter[]);
    std::string testHelloWorld();
private:
    OclContext* _context = OclEngine::getEngine()->createContext(true);

    OclProgram* _program = nullptr;

    OclKernel* _kernel = nullptr;

};


#endif //OCLEXAMPLE_IMAGEFILTER_H
