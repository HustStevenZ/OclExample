//
// Created by Sanqian on 16/7/8.
//

#include <iostream>
#include "OclErrors.h"

void OclErrors::CheckError(cl_int erroCode, std::string errSource) {
    std::string errMesg="";
    switch (erroCode)
    {
        case CL_INVALID_VALUE :
            errMesg="CL_INVALID_VALUE Invalid value in parameters!";
            break;
        case CL_OUT_OF_HOST_MEMORY:
            errMesg="CL_OUT_OF_HOST_MEMORY Failed to allocate memory on host!";
            break;
        case CL_DEVICE_NOT_FOUND:
            errMesg="CL_DEVICE_NOT_FOUND Device not found!";
            break;
        case CL_OUT_OF_RESOURCES :
            errMesg="CL_OUT_OF_RESOURCES failed to allocate resources!";
        case CL_SUCCESS:
            return;

        default:
            errMesg="Unkonwn Error found!";
            break;
    }
    std::cout<<errSource<<":"<<errMesg<<std::endl;
}