//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_OCLKERNEL_H
#define OCLEXAMPLE_OCLKERNEL_H

#include <string>
#include "OclHeader.h"
#include "OclBuffer.h"

class OclKernel {
public:
    OclKernel(cl_kernel kernel){
        _kernel = kernel;
    }

    ~OclKernel(){
        clReleaseKernel(_kernel);
    }

    void setKernelArg(int index,int size,const char* value);
    void setKernelArgInt(int index,int value);
    void setKernelArgString(int index,std::string value);
    void setKernelArgFloat(int index,float value);
    void setKernelArgBuffer(int index,OclBuffer* buffer);

    cl_kernel getClKernel(){return _kernel;};
private:
    cl_kernel _kernel;
};


#endif //OCLEXAMPLE_OCLKERNEL_H
