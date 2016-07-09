//
// Created by Sanqian on 16/7/8.
//

#ifndef OCLEXAMPLE_OCLENGINE_H
#define OCLEXAMPLE_OCLENGINE_H

#include <vector>
#include "OclHeader.h"
#include "OclContext.h"

//Max Number of platforms
//Make it 3 Intel+NV+AMD (the most maybe)
#define OCL_ENGINE_MAX_PLATFORM_ENTRIES 3

//Max number of computer units
#define OCL_DEVICE_MAX_DEVICE_NUM 128
class std::vector;
class OclEngine {

public:

    OclEngine();

    OclContext* createContext(OclContext::ContextProperties contextProperties);
    bool releaseContext(OclContext* context);
    ~OclEngine(){

        if(_contexts.size()>0)
        {
            for(OclContext* _context:_contexts)
            {

                releaseContext(_context);

            }
        }

        if(_platformInfo!= nullptr)
        {
            delete _platformInfo;
        }
    }

private:
    void init();

    bool isPlatformValid(){ return _platformInfo!= nullptr && _platformInfo->num_platforms>0;}

private:

    struct Platforms{
//        cl_platform_id platforms[OCL_ENGINE_MAX_PLATFORM_ENTRIES];
        PlatformInfo platforms[OCL_ENGINE_MAX_PLATFORM_ENTRIES];
        cl_uint num_platforms;
    };


    struct PlatformInfo{
        cl_platform_id platform_id;
        cl_device_type device_type;
        cl_uint num_entries;
        cl_device_id devices[OCL_DEVICE_MAX_DEVICE_NUM];
        cl_uint num_devices;
    };


    Platforms* _platformInfo = nullptr;
    std::vector<OclContext*> _contexts;

//    std::vector<OclDevice*> devices;

    PlatformInfo *getDefaultPlatform();
};


#endif //OCLEXAMPLE_OCLENGINE_H
