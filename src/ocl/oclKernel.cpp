//
// Created by Sanqian on 16/7/8.
//

#include "OclKernel.h"
#include "OclErrors.h"

void OclKernel::setKernelArg(int index, int size,const char *value) {
    cl_int errCode = clSetKernelArg(_kernel,index,size,(void*) value);
    OclErrors::CheckError(errCode,"OclKernel::setKernelArg");
}

void OclKernel::setKernelArgFloat(int index, float value) {
    setKernelArg(index,sizeof(float),(const char*)&value);
}

void OclKernel::setKernelArgInt(int index, int value) {
    setKernelArg(index, sizeof(int),(const char*)&value);
}
void OclKernel::setKernelArgString(int index, std::string value) {
    setKernelArg(index,value.length(),value.data());
}
void OclKernel::setKernelArgBuffer(int index, OclBuffer *buffer) {
    cl_mem clbuffer = buffer->getClBuffer();
    setKernelArg(index,sizeof(cl_mem),(char*)&clbuffer);
}