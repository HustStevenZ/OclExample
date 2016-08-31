//
// Created by Sanqian on 16/7/8.
//

#include "OclContext.h"
#include "OclErrors.h"
#include "OclProgram.h"
#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif
#ifdef __linux__
#include <GL/gl.h>
#endif
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
    cl_mem buffer=clCreateBuffer(_context,flags,size,host_ptr,&errCode);
    if(OclErrors::success(errCode))
    {

        OclBuffer* oclbuffer = new OclBuffer(buffer);
        return oclbuffer;

    }
    return nullptr;
}

void OclContext::enqueueKernel(OclKernel *kernel,unsigned int work_dimension, size_t * work_offset, size_t * work_size,
                               size_t * local_size) {
    cl_int errCode = clEnqueueNDRangeKernel(_command_queue,kernel->getClKernel(),work_dimension,work_offset,work_size,local_size,0,NULL,NULL);

    OclErrors::CheckError(errCode,"OclContext::enqueueKernel");

}

void OclContext::enqueueReadBuffer(OclBuffer *buffer, unsigned int offset, unsigned int size, char *hostMem) {
    cl_int errCode = clEnqueueReadBuffer(_command_queue,buffer->getClBuffer(),CL_TRUE,offset,size,(void*)hostMem,0,NULL,NULL);
    OclErrors::CheckError(errCode,"OclContext::enqueueReadBuffer");
}

void OclContext::enqueueWriteBuffer(OclBuffer *buffer, unsigned int offset, unsigned int size, char *hostMem) {
    cl_int errCode = clEnqueueWriteBuffer(_command_queue,buffer->getClBuffer(),CL_TRUE,offset,size,(void*)hostMem,0,NULL,NULL);
    OclErrors::CheckError(errCode,"OclContext::enqueueWriteBuffer");
}

OclImage* OclContext::createImage2D(OclBuffer::BufferMode  mode, const cl_image_format *format, size_t width, size_t height, size_t row_pitch,
                               void *data) {
    cl_int errCode = 0;
    cl_image_desc image_desc = {0};
    image_desc.image_type = CL_MEM_OBJECT_IMAGE2D;
    image_desc.image_width = width;
    image_desc.image_height = height;
    if(data!=NULL)
    {
        image_desc.image_row_pitch = row_pitch;
    }

    cl_mem clMem = clCreateImage(_context,OclBuffer::translateBufferModeToFlags(mode),format,&image_desc,NULL,&errCode);
//    cl_mem clMem= clCreateImage2D(_context,OclBuffer::translateBufferModeToFlags(mode),format,width,height,row_pitch,data,&errCode);
    OclErrors::CheckError(errCode,"OclContext::createImage2D");

    if(OclErrors::success(errCode))
    {
        OclImage* image = new OclImage(clMem);
        return image;
    }
    return nullptr;
}

OclImage* OclContext::createImage3D(OclBuffer::BufferMode mode, const cl_image_format *format, size_t width,
                                    size_t height, size_t depth, size_t row_pitch, size_t slice_pitch, void *data) {
    cl_int errCode = 0;
    cl_mem clMem= clCreateImage3D(_context,OclBuffer::translateBufferModeToFlags(mode),format,width,height,depth,row_pitch,slice_pitch,data,&errCode);
    OclErrors::CheckError(errCode,"OclContext::createImage3D");
    if(OclErrors::success(errCode))
    {
        OclImage* image = new OclImage(clMem);
        return image;
    }
    return nullptr;
}
void OclContext::enqueueReadImage2D(OclImage *image, unsigned int reginx, unsigned int reginy, char *buffer, unsigned int originx,
                                    unsigned int originey, unsigned int row_pitch) {
    size_t regin[3]={reginx,reginy,1};
    size_t orgin[3]={originx,originey,0};
    cl_int errCode=clEnqueueReadImage(_command_queue,
                                      image->getClBuffer(),
                                      CL_TRUE,
                                      orgin,
                                      regin,
                                      row_pitch,
                                      0,
                                      (void*)buffer,
                                      0,
                                      NULL,
                                      NULL);
    OclErrors::CheckError(errCode,"OclContext::enqueueReadImage2D");
}



void OclContext::enqueueWriteImage2D(OclImage *image, unsigned int reginx, unsigned int reginy, char *buffer,
                                     unsigned int originx, unsigned int originey, unsigned int row_pitch) {
    size_t regin[3]={reginx,reginy,1};
    size_t orgin[3]={originx,originey,0};
    cl_int errCode=clEnqueueWriteImage(_command_queue,
                                      image->getClBuffer(),
                                      CL_TRUE,
                                      orgin,
                                      regin,
                                      row_pitch,
                                      0,
                                      (void*)buffer,
                                      0,
                                      NULL,
                                      NULL);
    OclErrors::CheckError(errCode,"OclContext::enqueueWriteImage2D");
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

OclImage* OclContext::createImage2DFromGLTex(OclBuffer::BufferMode mode, int level, GLuint texObj) {
    cl_int errCode = 0;
//    glBindTexture(GL_TEXTURE_2D,texObj);
    cl_mem clMem = clCreateFromGLTexture(_context,OclBuffer::translateBufferModeToFlags(mode),GL_TEXTURE_2D,level,texObj,&errCode);
//    cl_mem clMem= clCreateImage2D(_context,OclBuffer::translateBufferModeToFlags(mode),format,width,height,row_pitch,data,&errCode);
    OclErrors::CheckError(errCode,"OclContext::createImage2D");

    if(OclErrors::success(errCode))
    {
        OclImage* image = new OclImage(clMem);
        return image;
    }
    return nullptr;
}
