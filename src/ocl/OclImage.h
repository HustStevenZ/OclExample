//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_OCLIMAGE_H
#define OCLEXAMPLE_OCLIMAGE_H


#include "OclBuffer.h"

class OclImage: public OclBuffer {
public:
    OclImage(cl_mem imageObj):OclBuffer(imageObj){};


};


#endif //OCLEXAMPLE_OCLIMAGE_H
