//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_OCLERRORS_H
#define OCLEXAMPLE_OCLERRORS_H

#include <string>
#include "OclHeader.h"

class OclErrors {

public:
    static void CheckError(cl_int erroCode,std::string errSource );
    static bool success(cl_int errCode)
    {
        return errCode==CL_SUCCESS;
    }

};


#endif //OCLEXAMPLE_OCLERRORS_H
