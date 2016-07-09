//
// Created by Sanqian on 16/7/8.
//

#include "OclEngine.h"
#include "OclErrors.h"
#include <Vector>

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
OclContext* OclEngine::createContext(OclContext::ContextProperties contextProperties) {
#if defined(_WIN32)

    // Windows
    cl_context_properties properties[] = {
      CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
      CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
      CL_CONTEXT_PLATFORM,(cl_context_properties) contextProperties.context_platform,
      CL_CONTEXT_INTEROP_USER_SYNC,(cl_context_properties) contextProperties.context_interop_user_sync,
      0
    };

#elif defined(__APPLE__)

    // OS X
    CGLContextObj     kCGLContext     = CGLGetCurrentContext();
    CGLShareGroupObj  kCGLShareGroup  = CGLGetShareGroup(kCGLContext);

    cl_context_properties properties[] = {
            CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
            (cl_context_properties) kCGLShareGroup,
            CL_CONTEXT_PLATFORM,(cl_context_properties) contextProperties.context_platform,
            CL_CONTEXT_INTEROP_USER_SYNC,(cl_context_properties) contextProperties.context_interop_user_sync,
            0
    };

#else

    // Linux
    cl_context_properties properties[] = {
      CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
      CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
      CL_CONTEXT_PLATFORM,(cl_context_properties) contextProperties.context_platform,
      CL_CONTEXT_INTEROP_USER_SYNC,(cl_context_properties) contextProperties.context_interop_user_sync,
      0
    };

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
        return true;
    }
    return false;
}
OclContext* OclEngine::createContext() {
    return createContext({getDefaultPlatform()->platform_id,false});
}

OclEngine* OclEngine::_oclEngine = nullptr;

