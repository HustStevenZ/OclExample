//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_IMAGEFILTER_H
#define OCLEXAMPLE_IMAGEFILTER_H


#include <src/ocl/OclEngine.h>
#include <QImage>

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

    std::string testHelloWorld();
private:
    OclContext* _context = OclEngine::getEngine()->createContext();


};


#endif //OCLEXAMPLE_IMAGEFILTER_H
