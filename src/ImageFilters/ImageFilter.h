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

    /**
     * TODO: Perhaps we may have time to isolate ImageFilter from QImage in the future
     */
    void filterImage(QImage* image);
private:
    OclEngine* oclEngine = new OclEngine();


};


#endif //OCLEXAMPLE_IMAGEFILTER_H
