//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_OCLHEADER_H
#define OCLEXAMPLE_OCLHEADER_H
/****
 * MAC OS SUPPORT?
 */
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <OpenCL/cl.h>
#include <OpenGL/OpenGL.h>
#else

#include <CL/opencl.h>
#include <CL/cl.h>
#endif
/*******
 * TODO: Linux & Windows
 */
#endif //OCLEXAMPLE_OCLHEADER_H
