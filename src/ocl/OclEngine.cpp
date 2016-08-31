//
// Created by Sanqian on 16/7/8.
//

#include "OclEngine.h"
#include "OclErrors.h"
#include <vector>
#include <algorithm>
#ifdef __linux__
#include <GL/glx.h>
#endif

/***
 * Construction for OclEngine
 */
OclEngine::OclEngine() {
    init();
}

/***
 * Init PlatformInfo
 */
void OclEngine::init() {

    if(_platformInfo== nullptr)
        _platformInfo = new Platforms();
    cl_platform_id platform_ids[OCL_ENGINE_MAX_PLATFORM_ENTRIES];
    cl_int errCode = clGetPlatformIDs(OCL_ENGINE_MAX_PLATFORM_ENTRIES,platform_ids,&_platformInfo->num_platforms);
    OclErrors::CheckError(errCode,"OclEngine::init");
    if(errCode==CL_SUCCESS)
    {
        for(int i = 0;i<_platformInfo->num_platforms;i++)
        {
            PlatformInfo* platform=_platformInfo->platforms+i;
            platform->platform_id = platform_ids[i];
            platform->device_type=CL_DEVICE_TYPE_GPU;//we only focus on GPU
            platform->num_entries=OCL_DEVICE_MAX_DEVICE_NUM;
            errCode = clGetDeviceIDs(platform->platform_id,CL_DEVICE_TYPE_GPU,OCL_DEVICE_MAX_DEVICE_NUM,platform->devices,&platform->num_devices);
            OclErrors::CheckError(errCode,"OclEngine::init");
        }
    }
}

/**
 *Create OCL Contxt
 */
OclContext* OclEngine::createGPUContext(OclContext::ContextProperties contextProperties) {


    int contextPropertiSize = 3;
    const int PLATFORM_PROPERTY_END=2;
    if(contextProperties.useSharedGPUContex);
        contextPropertiSize+=4;
    cl_context_properties* properties = new cl_context_properties[contextPropertiSize];
    //Add the common part
    properties[0] = CL_CONTEXT_PLATFORM;
    properties[1] = (cl_context_properties)contextProperties.context_platform;
    properties[PLATFORM_PROPERTY_END] = 0;

#if defined(_WIN32)

//    // Windows
//    cl_context_properties properties[] = {
//      CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
//      CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
//      CL_CONTEXT_PLATFORM,(cl_context_properties) contextProperties.context_platform,
//      CL_CONTEXT_INTEROP_USER_SYNC,(cl_context_properties) contextProperties.context_interop_user_sync,
//      0
//    };
    if(contextProperties.useSharedGPUContex && (wglGetCurrentContext()!=nullptr)
    && wglGetCurrentDC()!=nullptr)
    {
         properties[PLATFORM_PROPERTY_END+4] = 0;
         properties[PLATFORM_PROPERTY_END] = CL_GL_CONTEXT_KHR;
         properties[PLATFORM_PROPERTY_END+1] = (cl_context_properties)wglGetCurrentContext();
         properties[PLATFORM_PROPERTY_END+2] = CL_WGL_HDC_KHR;
         properties[PLATFORM_PROPERTY_END+3] = (cl_context_properties)wglGetCurrentDC();

    }

#elif defined(__APPLE__)

    // OS X

    CGLContextObj     kCGLContext     = CGLGetCurrentContext();

    CGLShareGroupObj  kCGLShareGroup  = CGLGetShareGroup(kCGLContext);

    if(contextProperties.useSharedGPUContex && kCGLShareGroup!= nullptr)
    {
        properties[PLATFORM_PROPERTY_END+2]  = 0;
        properties[PLATFORM_PROPERTY_END] = CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE;
        properties[PLATFORM_PROPERTY_END+1] = (cl_context_properties) kCGLShareGroup;
    }
//    {
//            CL_CONTEXT_PLATFORM,(cl_context_properties) contextProperties.context_platform,
//            0
//    };
#else

//    // Linux
//    cl_context_properties properties[] = {
////      CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
// //     CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
//      CL_CONTEXT_PLATFORM,(cl_context_properties) contextProperties.context_platform,
// //     CL_CONTEXT_INTEROP_USER_SYNC,(cl_context_properties) contextProperties.context_interop_user_sync,
//      0
//    };
if(contextProperties.useSharedGPUContex && (glXGetCurrentContext()!=nullptr)
    && glXGetCurrentDisplay()!=nullptr)
    {
         properties[PLATFORM_PROPERTY_END+4] = 0;
         properties[PLATFORM_PROPERTY_END] = CL_GL_CONTEXT_KHR;
         properties[PLATFORM_PROPERTY_END+1] = (cl_context_properties)glXGetCurrentContext();
         properties[PLATFORM_PROPERTY_END+2] = CL_GLX_DISPLAY_KHR;
         properties[PLATFORM_PROPERTY_END+3] = (cl_context_properties)glXGetCurrentDisplay();

    }
#endif

    PlatformInfo* defaultPlatform = getDefaultPlatform();
    if(defaultPlatform== nullptr)
        return nullptr;
    cl_int errCode;
    cl_context clContext = clCreateContext(properties,
                                           defaultPlatform->num_devices,
                                           defaultPlatform->devices,NULL,NULL,&errCode);

    if(OclErrors::success(errCode))
    {
        OclContext *context = new OclContext(clContext);
        this->_contexts.push_back(context);
        return context;
    }
    else
        return nullptr;
}

OclEngine::PlatformInfo* OclEngine::getDefaultPlatform() {
    if(!isPlatformValid())
    return nullptr;
    return _platformInfo->num_platforms>0?_platformInfo->platforms+0: nullptr;
}

bool OclEngine::releaseContext(OclContext *context) {
    std::vector<OclContext*>::iterator it = std::find(_contexts.begin(),_contexts.end(),context);
    if(it!=_contexts.end()) {
        _contexts.erase(it);
        delete context;
        return true;
    }
    return false;
}
OclContext* OclEngine::createContext() {
    return createGPUContext({false,getDefaultPlatform()->platform_id,true});
}

OclContext* OclEngine::createContext(bool withGpuContext) {
    return createGPUContext({withGpuContext,getDefaultPlatform()->platform_id,true});
}

OclEngine* OclEngine::_oclEngine = nullptr;

