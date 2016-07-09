//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_IMAGE_H
#define OCLEXAMPLE_IMAGE_H

/**
 * TODO: Intermediate datatype to store host side image data
 * Using this datatyp we may be able to  isolate
 * cl_image from other image sources such as QImage/CVImage etc.
 */
class Image {

public:
    enum {
        IMAGE_FORMAT_RGBX8888
    };
    Image();

private:
    char** data;
};


#endif //OCLEXAMPLE_IMAGE_H
