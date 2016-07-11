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
            break;
        case CL_INVALID_PROGRAM_EXECUTABLE:
            errMesg="CL_INVALID_PROGRAM_EXECUTABLE no successfully built program\n"\
                    "executable available!";
            break;
        case CL_INVALID_COMMAND_QUEUE:
            errMesg="CL_INVALID_COMMAND_QUEUE command_queue is not valid";
            break;
        case CL_INVALID_KERNEL:
            errMesg="CL_INVALID_KERNEL kernel is not valid!";
            break;
        case CL_INVALID_CONTEXT:
            errMesg="CL_INVALID_CONTEXT context is not valid!";
            break;
        case CL_INVALID_KERNEL_ARGS:
            errMesg="CL_INVALID_KERNEL_ARGS kernel arguments are not correctly specified!";
            break;
        case CL_INVALID_WORK_DIMENSION:
            errMesg="CL_INVALID_WORK_DIMENSION work dimension must be in 1,2 or 3";
            break;
        case CL_INVALID_GLOBAL_WORK_SIZE:
            errMesg="CL_INVALID_GLOBAL_WORK_SIZE global_work_size is neither NULL nor valid values!";
            break;
        case CL_INVALID_GLOBAL_OFFSET:
            errMesg="CL_INVALID_GLOBAL_OFFSET the value specified in global_work_size + the\n"
                    "corresponding values in global_work_offset for any dimensions is greater than the\n"
                    "sizeof(size t) for the device ";
            break;
        case CL_INVALID_WORK_GROUP_SIZE:
            errMesg="CL_INVALID_WORK_GROUP_SIZE local_work_size is specified and number of workitems specified by global_work_size is not evenly divisible by size of work-group given\n"
                    "by local_work_size or does not match the work-group size specified for kernel ";
            break;
        case CL_INVALID_WORK_ITEM_SIZE:
            errMesg="CL_INVALID_WORK_ITEM_SIZE  the number of work-items specified in any of\n"
                    "local_work_size[0], … local_work_size[work_dim – 1] is greater than the\n"
                    "corresponding values specified by CL_DEVICE_MAX_WORK_ITEM_SIZES[0], ….\n"
                    "CL_DEVICE_MAX_WORK_ITEM_SIZES[work_dim – 1].";
            break;
        case CL_MISALIGNED_SUB_BUFFER_OFFSET:
            errMesg="CL_MISALIGNED_SUB_BUFFER_OFFSET  a sub-buffer object is specified as the value\n"
                    "for an argument that is a buffer object and the offset specified when the sub-buffer object\n"
                    "is created is not aligned to CL_DEVICE_MEM_BASE_ADDR_ALIGN value for device\n"
                    "associated with queue.";
            break;
        case CL_INVALID_IMAGE_SIZE :
            errMesg="CL_INVALID_IMAGE_SIZE the image dimension sare not supported by device!";
            break;
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:
            errMesg="CL_IMAGE_FORMAT_NOT_SUPPORTED image format is not supported by device!";
            break;
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            errMesg="CL_MEM_OBJECT_ALLOCATION_FAILURE failed to allocate memory!";
            break;
        case CL_INVALID_EVENT_WAIT_LIST:
            errMesg="CL_INVALID_EVENT_WAIT_LIST event_wait_list are not valid events!";
            break;
        case CL_SUCCESS:
            return;

        default:
            errMesg="Unkonwn Error found!";
            break;
    }
    std::cout<<errSource<<":"<<errMesg<<std::endl;
}