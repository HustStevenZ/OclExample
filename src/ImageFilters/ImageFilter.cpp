//
// Created by Sanqian on 16/7/8.
//

#include <cassert>
#include "ImageFilter.h"

ImageFilter::ImageFilter() {
}

void ImageFilter::filterImage(QImage *image) {

}
std::string helloWorldKenel="__kernel void hello_kernel(__global char16 *msg) {\n"\
        "*msg = (char16)('H', 'e', 'l', 'l', 'o', ' ',\n"\
        "'k', 'e', 'r', 'n', 'e', 'l', '!', '!', '!', '\\0');\n"\
        "}";
std::string memsetKernel =
        "__kernel void memset( __global uint *dst ) \n"
                "{ \n"
                " dst[get_global_id(0)] = 0xffffffff; \n"
                "} \n";
char* helloKernel=
        "__kernel void hello(__global float *input, __global float *output)\n"\
"{\n"\
"  size_t id = get_global_id(0);\n"\
"  output[id] = input[id] * input[id];\n"\
"}\n";

#define DATA_SIZE 10

std::string ImageFilter::testHelloWorld() {
    OclProgram *oclProgram = _context->createProgramFromSource(helloWorldKenel);

    OclKernel *oclKernel = oclProgram->createKernel("hello_kernel");

    OclBuffer *inputoclBuffer = _context->createBuffer(1024*sizeof(int),OclBuffer::BufferMode::OCL_BUFFER_WRITE_ONLY,NULL);
    oclKernel->setKernelArgBuffer(0,inputoclBuffer);

    _context->enqueueKernel(oclKernel,1,1024,1,1);

    _context->flush();
    char* buffer = new char[1024*sizeof(int)];

    _context->enqueueReadBuffer(inputoclBuffer,0,1024*sizeof(int),&(buffer));
    return std::string(buffer);


}