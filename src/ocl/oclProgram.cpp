//
// Created by Sanqian on 16/7/8.
//

#include <cassert>
#include "OclProgram.h"
#include "OclErrors.h"

OclKernel* OclProgram::createKernel(std::string kernelName) {
    cl_int errCode;
    if(kernelName.empty())
        return nullptr;
    cl_kernel kernel = clCreateKernel(_program,kernelName.data(),&errCode);
#ifdef DEBUG
    cl_kernel_info clKernelInfo;
    char* funcNane = new char[1024];

    clGetKernelInfo(kernel,CL_KERNEL_FUNCTION_NAME,1024,funcNane,NULL);
    assert(kernelName.compare(funcNane)==0);
#endif
    OclErrors::CheckError(errCode,"OclProgram::createKernel");
    if(OclErrors::success(errCode))
    {
        OclKernel *oclkernel = new OclKernel(kernel);

        return oclkernel;

    }
    return nullptr;
}