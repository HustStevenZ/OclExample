//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_OCLCOMMANDQUEUE_H
#define OCLEXAMPLE_OCLCOMMANDQUEUE_H

#include "OclHeader.h"

class OclCommandQueue {
public:
    OclCommandQueue(cl_command_queue command_queue)
    {
        _command_queue = command_queue;
    }

    ~OclCommandQueue(){
        clReleaseCommandQueue(_command_queue);
    }

private:
    cl_command_queue _command_queue;
};


#endif //OCLEXAMPLE_OCLCOMMANDQUEUE_H
