//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_OCLBUFFER_H
#define OCLEXAMPLE_OCLBUFFER_H

#include "OclHeader.h"

class OclBuffer {

public:
    enum BufferMode{
        OCL_BUFFER_READ_WRITE=0x01,
        OCL_BUFFER_WRITE_ONLY=0x02,
        OCL_BUFFER_READ_ONLY=0x04,
        OCL_BUFFER_COPY_HOST=0x08,
        OCL_BUFFER_HOST_READ_ONLY=0x10
        //TO-DO: CL_MEM_USE_HOST_PTR,CL_MEM_ALLOC_HOST_PTR,CL_MEM_COPY_HOST_PTR,CL_MEM_HOST_WRITE_ONLY
        //       ,CL_MEM_HOST_READ_ONLY,CL_MEM_HOST_NO_ACCESS
    };

public:
    OclBuffer(cl_mem buffer){
        _buffer=buffer;
    };

    ~OclBuffer()
    {
        clReleaseMemObject(_buffer);
    }

    cl_mem getClBuffer(){
        return _buffer;
    }

    static unsigned long translateBufferModeToFlags(BufferMode mode);
    
private:
    cl_mem _buffer;

};

inline OclBuffer::BufferMode operator|(OclBuffer::BufferMode a,OclBuffer::BufferMode b)
{
    return static_cast<OclBuffer::BufferMode >(static_cast<int>(a)| static_cast<int>(b));
}
#endif //OCLEXAMPLE_OCLBUFFER_H
