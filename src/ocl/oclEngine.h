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

class OclEngine {

public:

    static OclEngine* getEngine()
    {

        if(_oclEngine == nullptr)
        {
            _oclEngine = new OclEngine();
        }
        return _oclEngine;
    }

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

    OclContext* createContext();

    bool releaseContext(OclContext* context);


private:
    OclEngine();
    void init();

    bool isPlatformValid(){ return _platformInfo!= nullptr && _platformInfo->num_platforms>0;}
    OclContext* createContext(OclContext::ContextProperties contextProperties);

private:

    static OclEngine* _oclEngine;

    struct PlatformInfo{
        cl_platform_id platform_id;
        cl_device_type device_type;
        cl_uint num_entries;
        cl_device_id devices[OCL_DEVICE_MAX_DEVICE_NUM];
        cl_uint num_devices;
    };

    struct Platforms{
//        cl_platform_id platforms[OCL_ENGINE_MAX_PLATFORM_ENTRIES];
        PlatformInfo platforms[OCL_ENGINE_MAX_PLATFORM_ENTRIES];
        cl_uint num_platforms;
    };



    Platforms* _platformInfo = nullptr;
    std::vector<OclContext*> _contexts;

//    std::vector<OclDevice*> devices;

    PlatformInfo *getDefaultPlatform();
};

#endif //OCLEXAMPLE_OCLENGINE_H
