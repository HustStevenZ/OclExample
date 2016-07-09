//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_OCLDEVICE_H
#define OCLEXAMPLE_OCLDEVICE_H

#include "OclHeader.h"

class OclEngine;
class OclContext {

public:
    OclContext(cl_context context);

public:
    struct ContextProperties{
        cl_platform_id context_platform;
        cl_bool context_interop_user_sync;
    };

private:
    cl_context _context = 0;
};


#endif //OCLEXAMPLE_OCLDEVICE_H
