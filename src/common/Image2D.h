//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_IMAGE_H
#define OCLEXAMPLE_IMAGE_H

#include <OpenCL/opencl.h>
#include <src/ocl/OclImage.h>
#include "Color.h"

/**
 * TODO: Intermediate datatype to store host side image data
 * Using this data type we may be able to  isolate
 * cl_image from other image sources such as QImage/CVImage etc.
 * We use char to store the data, different format corresponds to
 * different layout of the data array. However, we use row major by default
 */


class Image2D {

public:
    enum Format{
        IMAGE_FORMAT_RGBX8888,
        IMAGE_FORMAT_RGBA8888
    };

    /**
     * Create an empty image
     * Recommended!
     */
    Image2D(unsigned int width, unsigned int height,Format format);

    /**
     * Create an image with default data setting
     * Hint: we will not copy the data, instead
     * we will use it directly. do not release the original pointer,
     * it will be release by the image;
     */
    Image2D(unsigned int width, unsigned int height,Format format,char* data);

    /**
     * Use a Color array to initialize the image
     * Recommended!
     */
    Image2D(unsigned int width, unsigned int height,Format format,Color* data);
    Image2D(Image2D &src);

    ~Image2D(){
        if(data!= nullptr)
            delete data;
    }
    static bool convert(Image2D &src, Image2D &dest);

    void setColor(int indexw, int indexh,Color color);
    void getColor(int indexw, int indexh,Color &color);

    /**
     * TODO: change the store of data to column major
     */
    void transpose();

    /**
     * getBist returns the pointer which can be directly transfer to OclImage
     * as long as it's Format matches the need
     */
    char* getBits();


private:
    struct FormatMap{
        Format format;

        unsigned int dataSize;
    };
    static FormatMap formatMap;
    char* data;
    unsigned int width;
    Format format;
};


#endif //OCLEXAMPLE_IMAGE_H
