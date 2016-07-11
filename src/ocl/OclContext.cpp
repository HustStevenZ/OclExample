//
// Created by Sanqian on 16/7/8.
//

#include "OclContext.h"
#include "OclErrors.h"
#include "OclProgram.h"
OclContext::OclContext(cl_context context) {
    _context=context;

    cl_int errCode;
    clGetContextInfo(_context,CL_CONTEXT_NUM_DEVICES,sizeof(cl_int),&_num_devices,NULL);

    _devices = new cl_device_id[_num_devices];
    clGetContextInfo(_context,CL_CONTEXT_DEVICES, sizeof(cl_device_id)*_num_devices,_devices,NULL);

    _command_queue = clCreateCommandQueue(_context,_devices[0],0,&errCode);
}

OclProgram* OclContext::createProgramFromSource(std::string sourceStr) {
    if(sourceStr.empty())
        return nullptr;
//    char buffer[1024*128];
//    memmove(buffer,sourceStr.data(),sourceStr.length());
    const char * source={sourceStr.data()};
    cl_int errCode;
//    size_t sourceSize = sourceStr.length();
    cl_program program = clCreateProgramWithSource(_context,1,&source,NULL,&errCode);
    OclErrors::CheckError(errCode,"OclContext::createProgramFromSource");
    errCode =  clBuildProgram(program,1,&_devices[0],NULL,NULL,NULL);
    OclErrors::CheckError(errCode,"OclContext::createProgramFromSource");
    if(OclErrors::success(errCode))
    {

        OclProgram* oclProgram= new OclProgram(program);

        return oclProgram;
    }
    return nullptr;
}

OclContext::~OclContext() {
    clReleaseContext(_context);
}
OclBuffer* OclContext::createBuffer(int size, OclBuffer::BufferMode mode, char *host_ptr) {
    //It is necessary to refine the following code in a fuction in OclBuffer in the future
    //without expose cl_mem_flags
    cl_mem_flags flags =OclBuffer::translateBufferModeToFlags(mode);
    cl_int errCode;
    if(size<=0)
        return nullptr;
    cl_mem buffer=clCreateBuffer(_context,CL_MEM_WRITE_ONLY,size,host_ptr,&errCode);
    if(OclErrors::success(errCode))
    {

        OclBuffer* oclbuffer = new OclBuffer(buffer);
        return oclbuffer;

    }
    return nullptr;
}

void OclContext::enqueueKernel(OclKernel *kernel, unsigned int work_dim, unsigned int work_offset, unsigned int work_size,
                               unsigned int local_size) {
    size_t size=work_size;
    cl_int errCode = clEnqueueNDRangeKernel(_command_queue,kernel->getClKernel(),work_dim,NULL,&size,NULL,0,NULL,NULL);

    OclErrors::CheckError(errCode,"OclContext::enqueueKernel");

}

void OclContext::enqueueReadBuffer(OclBuffer *buffer, unsigned int offset, unsigned int size, char **hostMem) {
    cl_int errCode = clEnqueueReadBuffer(_command_queue,buffer->getClBuffer(),CL_TRUE,offset,size,(void*)*hostMem,0,NULL,NULL);
    OclErrors::CheckError(errCode,"OclContext::enqueueReadBuffer");
}

void OclContext::flush() {
    cl_int errCode = clFinish(_command_queue);
    OclErrors::CheckError(errCode,"OclContext::flush");

}

void OclContext::mapReadBuffer(OclBuffer *buffer, unsigned int size, char *hostMeme) {
    cl_int errCode;
    char* data = (char*)clEnqueueMapBuffer(_command_queue,buffer->getClBuffer(),CL_TRUE,CL_MAP_READ,0,size,0,NULL,NULL,&errCode);
    if(data!= nullptr)
    {
        for(int i = 0;i<size;i++)
            *(hostMeme+i)=*(data+i);
    }

}