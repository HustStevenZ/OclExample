//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_OCLDEVICE_H
#define OCLEXAMPLE_OCLDEVICE_H

#include <string>
#include "OclHeader.h"
#include "OclProgram.h"
#include "OclBuffer.h"

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

    void enqueueKernel(OclKernel* kernel,unsigned int work_dim,unsigned int work_offset,unsigned int work_size,unsigned int local_size);

    void enqueueReadBuffer(OclBuffer* buffer, unsigned int offset, unsigned int size,char** hostMem);

    void mapReadBuffer(OclBuffer*buffer,unsigned int size,char *hostMeme);

    void flush();
    const cl_command_queue get_command_queue() const {
        return _command_queue;
    }

public:
    struct ContextProperties{
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
