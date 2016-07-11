//
// Created by Sanqian on 16/7/8.
//

#include <cassert>
#include <src/io/clSource.h>
#include "ImageFilter.h"

ImageFilter::ImageFilter() {
}
std::string imageFilterKernel="__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |\n"
        "CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;\n"
        "__kernel void image_filter( __read_only image2d_t input, __write_only image2d_t output) {\n"
        "\n"
        "    int2 coord = (int2)(get_global_id(0),get_global_id(1));\n"
        "    uint4 pixel = read_imageui(input,sampler,coord);\n"
        "    write_imageui(output,coord,pixel);\n"
        "}";

QImage* ImageFilter::filterImage(QImage *image) {


//    clSource source("cl/imageFilters/programes/imageFilter.cl");

    OclProgram *oclProgram = _context->createProgramFromSource(imageFilterKernel);

    OclKernel *oclKernel = oclProgram->createKernel("image_filter");
    cl_image_format rgb_format;
    rgb_format.image_channel_order = CL_RGBA;
    rgb_format.image_channel_data_type = CL_UNSIGNED_INT8;

    OclImage *inputBuffer =_context->createImage2D(OclBuffer::BufferMode::OCL_BUFFER_READ_ONLY,&rgb_format,image->width(),image->size().height(),image->width()*4,NULL);
    OclImage *outputBuffer =_context->createImage2D(OclBuffer::BufferMode::OCL_BUFFER_WRITE_ONLY,&rgb_format,image->width(),image->size().height(),image->width()*4,NULL);


    QImage imageconverted = image->convertToFormat(QImage::Format_RGBX8888);
    _context->enqueueWriteImage2D(inputBuffer,image->width(),image->height(),(char*)(imageconverted.bits()));
    oclKernel->setKernelArgBuffer(0,inputBuffer);
    oclKernel->setKernelArgBuffer(1,outputBuffer);

    size_t work_size[]= {(size_t)image->width(),(size_t)image->height()};
    _context->enqueueKernel(oclKernel,2,NULL,work_size,NULL);

    _context->flush();
    char* buffer = new char[image->width()*image->height()*sizeof(int)];

    _context->enqueueReadImage2D(outputBuffer,image->width(),image->height(),buffer);
//    _context->?(outputBuffer,0,1024*1024*sizeof(int),&(buffer));

    if(strlen(buffer)>0)
    {

        return new QImage((uchar*)buffer,image->width(),image->height(),QImage::Format_RGBX8888);

    }

}


std::string ImageFilter::testHelloWorld() {

    clSource source("cl/imageFilters/programes/helloKernel.cl");

    OclProgram *oclProgram = _context->createProgramFromSource(source.getSource());

    OclKernel *oclKernel = oclProgram->createKernel("hello_kernel");

    OclBuffer *inputoclBuffer = _context->createBuffer(1024*sizeof(int),OclBuffer::BufferMode::OCL_BUFFER_WRITE_ONLY,NULL);
    oclKernel->setKernelArgBuffer(0,inputoclBuffer);

    size_t work_size= 1024;
    _context->enqueueKernel(oclKernel,1,NULL,&work_size,NULL);

    _context->flush();
    char* buffer = new char[1024*sizeof(int)];

    _context->enqueueReadBuffer(inputoclBuffer,0,1024*sizeof(int),&(buffer));
    return std::string(buffer);


}