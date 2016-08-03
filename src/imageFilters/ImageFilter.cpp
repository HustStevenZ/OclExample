//
// Created by Sanqian on 16/7/8.
//

#include <cassert>
#include <src/io/clSource.h>
#include <iostream>
#include "ImageFilter.h"

ImageFilter::ImageFilter() {
}
//////////////////////////
// 3*3 Filter
//[ * * * ]
//[ * * * ] * 1/factorsum
//[ * * * ]
////////////////////////
std::string imageFilterKernel="\n"
        "__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;\n"
        "\n"
        "__kernel void image_filter( __read_only image2d_t input, __write_only image2d_t output,__global float* filter) {\n"
        "\n"
        "      int width = get_global_size(0);\n"
        "      int height = get_global_size(1);\n"
        "      int2 coord = (int2)(get_global_id(0),get_global_id(1));\n"
        "\n"
        "      int2 leftupCord = (int2)(coord.x-1,coord.y-1);\n"
        "      int2 upCord = (int2)(coord.x,coord.y);\n"
        "      int2 rightupCord = (int2)(coord.x+1,coord.y-1);\n"
        "      int2 leftCord = (int2)(coord.x-1,coord.y);\n"
        "      int2 rightCord = (int2)(coord.x+1,coord.y);\n"
        "      int2 leftDowCord= (int2)(coord.x-1,coord.y+1);\n"
        "      int2 downCord = (int2)(coord.x,coord.y+1);\n"
        "      int2 rightDownCord = (int2)(coord.x+1,coord.y+1);\n"
        "\n"
        "      //BOUNARY\n"
        "      //Treat column[0]=column1,clumn[width+1]=column[width] etc\n"
        "      if(coord.x==1)\n"
        "      {\n"
        "        leftupCord.x=1;\n"
        "        leftCord.x=1;\n"
        "        leftDowCord.x=1;\n"
        "      }\n"
        "      if(coord.x==width)\n"
        "      {\n"
        "        rightCord.x=width;\n"
        "        rightDownCord.x=width;\n"
        "        rightupCord.x=width;\n"
        "      }\n"
        "      if(coord.y==1)\n"
        "      {\n"
        "        leftupCord.y=1;\n"
        "        upCord.y=1;\n"
        "        rightupCord.y=1;\n"
        "      }\n"
        "      if(coord.y==height)\n"
        "      {\n"
        "        leftDowCord.y=height;\n"
        "        downCord.y=height;\n"
        "        rightDownCord.y=height;\n"
        "      }\n"
        "      uint4 pixel = read_imageui(input,sampler,coord);\n"
        "      uint4 pleftup = read_imageui(input,sampler,leftupCord);\n"
        "      uint4 pup = read_imageui(input,sampler,upCord);\n"
        "      uint4 prightup = read_imageui(input,sampler,rightupCord);\n"
        "      uint4 pleft = read_imageui(input,sampler,leftCord);\n"
        "      uint4 pright = read_imageui(input,sampler,rightCord);\n"
        "      uint4 pleftdown = read_imageui(input,sampler,leftDowCord);\n"
        "      uint4 pdown = read_imageui(input,sampler,downCord);\n"
        "      uint4 prightdown = read_imageui(input,sampler,rightDownCord);\n"
        "\n"
        "\n"
        "       uint4 resultpixelf =convert_uint4((convert_float4(pleftup)*filter[0]+convert_float4(pup)*filter[1]+convert_float4(prightup)*filter[2]+\n"
        "                           convert_float4(pleft)*filter[3]+convert_float4(pixel)*filter[4]+convert_float4(pright)*filter[5]+\n"
        "                           convert_float4(pleftdown)*filter[6]+convert_float4(pdown)*filter[7]+convert_float4(prightdown)*filter[8]));\n"
        "\n"
        "      write_imageui(output,coord,resultpixelf);\n"
        "    }";

QImage* ImageFilter::blurImage(QImage *image)  {

    float filter[9]={
            1.0/16.0,2.0/16.0,1.0/16.0,
            2.0f/16.0, 4.0f/16.0,2.0f/16.0,
            1.0f/16.0,2.0f/16.0,1.0f/16.0};
    return filterImage3x3(image,filter);
}

QImage* ImageFilter::sharpingImage(QImage *image) {
    float filter[9]={-1.0,-1.0,-1.0,
                     -1.0,9.0,-1.0,
                     -1.0,-1.0,-1.0};
    return filterImage3x3(image,filter);
}

QImage* ImageFilter::embossingImage(QImage *image) {

    float filter[9]={-1.0f,-0.5f,-0.0f,
                     -0.5f,0.5f,0.5f,
                     0.0f,0.5f,1.0f};
    return filterImage3x3(image,filter);
}
QImage* ImageFilter::filterImage3x3(QImage *image, const float *filter){


//    clSource source("cl/imageFilters/programes/imageFilter.cl");

    OclProgram *oclProgram = _context->createProgramFromSource(imageFilterKernel);

    OclKernel *oclKernel = oclProgram->createKernel("image_filter");
    cl_image_format rgb_format;
    rgb_format.image_channel_order = CL_RGBA;
    rgb_format.image_channel_data_type = CL_UNSIGNED_INT8;

    OclImage *inputBuffer =_context->createImage2D(OclBuffer::BufferMode::OCL_BUFFER_READ_ONLY,&rgb_format,image->width(),image->size().height(),image->width()*4,NULL);
    OclImage *outputBuffer =_context->createImage2D(OclBuffer::BufferMode::OCL_BUFFER_WRITE_ONLY,&rgb_format,image->width(),image->size().height(),image->width()*4,NULL);

    OclBuffer *filterBuffer = _context->createBuffer(9*sizeof(float),OclBuffer::BufferMode::OCL_BUFFER_READ_ONLY,NULL);


    _context->enqueueWriteBuffer(filterBuffer,0,9*sizeof(float),(char*)(filter));
    unsigned int *data = new unsigned int[image->width()*image->height()];
    for(int i = 0;i<image->width();i++)
    {
        for(int j=0;j<image->height();j++)
        {
            int index = image->width()*j+i;
            QRgb color = image->pixel(i,j);
            *(data+image->width()*j+i)=0x00000000;

            *(data+image->width()*j+i)|=(0xff000000&(qRed(color)<<24));
            *(data+image->width()*j+i)|=(0x00ff0000&(qGreen(color)<<16));
            *(data+image->width()*j+i)|=(0x0000ff00&(qBlue(color)<<8));
            *(data+image->width()*j+i)|=(0x000000ff&(qAlpha(color)));
//            *(data+image->width()*j+i)=0xff000000;
        }
    }
    _context->enqueueWriteImage2D(inputBuffer,image->width(),image->height(),(char*)(data));
    oclKernel->setKernelArgBuffer(0,inputBuffer);
    oclKernel->setKernelArgBuffer(1,outputBuffer);
    oclKernel->setKernelArgBuffer(2,filterBuffer);

    size_t local_w = 3;
    size_t local_h = 3;
    size_t work_size[]= {((size_t)image->width()/local_w)*local_w,((size_t)image->height()/local_h)*local_h};
    size_t local_size[]={local_w,local_h};
    size_t offset_size[]={work_size[0]%local_w,work_size[1]%local_h};
    _context->enqueueKernel(oclKernel,2,offset_size,work_size,local_size);

    _context->flush();
    unsigned int* buffer = new unsigned int[image->width()*image->height()];

    _context->enqueueReadImage2D(outputBuffer,image->width(),image->height(),(char*)buffer);
    QSize imageSize(image->width(),image->height());
    QImage *resultImage = new QImage(imageSize,QImage::Format_RGBX8888);
//    QRgb *imageData = new QRgb[image->width()*image->height()];
    for(int i = 0;i<image->width();i++)
    {
        for(int j=0;j<image->height();j++)
        {
            int index = image->width()*j+i;
            int red=((*(buffer+index))&0xff000000)>>24;
            int green=((*(buffer+index))&0x00ff0000)>>16;
            int blue=((*(buffer+index))&0x0000ff00)>>8;
            int alpha=((*(buffer+index))&0x000000ff);
//            *(imageData+index)=qRgba(red,green,blue,alpha);
            resultImage->setPixel(i,j,qRgba(red,green,blue,alpha));
            //resultImage->setColor(index,qRgba(red,green,blue,alpha));
        }
    }

    delete inputBuffer;
    delete outputBuffer;
    delete filterBuffer;
    delete oclKernel;
    delete oclProgram;
    delete data;
    delete buffer;

    return resultImage;


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

    _context->enqueueReadBuffer(inputoclBuffer,0,1024*sizeof(int),(buffer));
    return std::string(buffer);


}