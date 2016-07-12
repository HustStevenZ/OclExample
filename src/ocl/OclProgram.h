//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_OCLPROGRAM_H
#define OCLEXAMPLE_OCLPROGRAM_H

#include <string>
#include "OclHeader.h"
#include "OclKernel.h"

class OclProgram {
public:
    OclProgram(cl_program program)
    {
        _program=program;
//        clBuildProgram(_program,0,NULL,NULL,NULL,NULL);
    }

    ~OclProgram(){
        clReleaseProgram(_program);
    }
    OclKernel* createKernel(std::string kernelName);
private:
    cl_program  _program;
};


#endif //OCLEXAMPLE_OCLPROGRAM_H
