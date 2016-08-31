//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_OCLDEVICE_H
#define OCLEXAMPLE_OCLDEVICE_H

#include <string>
#include "OclHeader.h"
#include "OclProgram.h"
#include "OclBuffer.h"
#include "OclImage.h"
#ifdef __linux__
#include <GL/gl.h>
#endif

class OclEngine;


class OclContext {

public:
    OclContext(cl_context context);

    ~OclContext();
    cl_context getClContext(){
        return _context;
    }
    OclProgram* createProgramFromSource(std::string sourceStr);

    OclBuffer* createBuffer(int size,OclBuffer::BufferMode mode,char* host_ptr);
    OclImage* createImage2D(OclBuffer::BufferMode mode,const cl_image_format *format, size_t width, size_t height, size_t row_pitch, void *data);

    OclImage* createImage2DFromGLTex(OclBuffer::BufferMode mode,int level,GLuint texObj);
    OclImage* createImage3D(OclBuffer::BufferMode mode,const cl_image_format *format, size_t width, size_t height,
                            size_t depth, size_t row_pitch, size_t slice_pitch, void *data);

    void enqueueKernel(OclKernel* kernel,unsigned int work_dimension,size_t * work_offset,size_t * work_size,size_t * local_size);
    void enqueueKernel(OclKernel* kernel,unsigned int work_dimension,size_t * work_offset,size_t * work_size,size_t * local_size,cl_event& profilingevent);

    void enqueueReadBuffer(OclBuffer* buffer, unsigned int offset, unsigned int size,char* hostMem);

    void enqueueReadImage2D(OclImage* image, unsigned int reginx,
                            unsigned int reginy,char* buffer,  unsigned int originx =0 , unsigned int originey=0,unsigned int row_pitch = 0);
    void enqueueWriteImage2D(OclImage* image, unsigned int reginx,
                             unsigned int reginy,char* buffer,  unsigned int originx =0 , unsigned int originey=0,unsigned int row_pitch = 0);
    void enqueueWriteBuffer(OclBuffer* buffer, unsigned int offset, unsigned int size,char* hostMem);

    void mapReadBuffer(OclBuffer*buffer,unsigned int size,char *hostMeme);

    void flush();
    const cl_command_queue get_command_queue() const {
        return _command_queue;
    }

public:
    struct ContextProperties{
        bool useSharedGPUContex;
        cl_platform_id context_platform;
        cl_bool context_interop_user_sync;
    };

private:
    cl_device_id *_devices;
    cl_int _num_devices;
    cl_context _context = 0;



private:
    cl_command_queue _command_queue;
};


#endif //OCLEXAMPLE_OCLDEVICE_H
