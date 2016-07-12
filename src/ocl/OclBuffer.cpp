//
// Created by Sanqian on 16/7/8.
//

#include "OclBuffer.h"

unsigned long OclBuffer::translateBufferModeToFlags(BufferMode mode) {
    cl_mem_flags flags = 0X00000000;
    if (mode & OclBuffer::BufferMode::OCL_BUFFER_READ_WRITE) {
        flags |= CL_MEM_READ_WRITE;
    }
    if (mode & OclBuffer::BufferMode::OCL_BUFFER_READ_ONLY)
    {
        flags |= CL_MEM_READ_ONLY;
    }
    if(mode&OclBuffer::BufferMode::OCL_BUFFER_WRITE_ONLY)
    {
        flags |= CL_MEM_WRITE_ONLY;
    }
    if(mode&OclBuffer::BufferMode::OCL_BUFFER_COPY_HOST)
    {
        flags |= CL_MEM_COPY_HOST_PTR;
    }
    if(mode&OclBuffer::BufferMode::OCL_BUFFER_HOST_READ_ONLY)
    {
        flags |= CL_MEM_HOST_READ_ONLY;
    }
    if(mode&OclBuffer::BufferMode::OCL_BUFFER_USER_HOST_PTR)
    {
        flags |= CL_MEM_USE_HOST_PTR;
    }
    return flags;
}