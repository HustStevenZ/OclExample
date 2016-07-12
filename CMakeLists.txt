cmake_minimum_required(VERSION 3.0)
project(OclExample)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

#####################################################################
###           QT Dependencies                                     ###
#####################################################################

find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

# Add the include directories for the Qt 5 Widgets module to
# the compile lines.
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})

# Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Gui_DEFINITIONS})
# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
#link_libraries(Qt5::Widgets)
set(SOURCE_FILES
        src/gui/CentralWidget.cpp
        src/gui/CentralWidget.h
        src/OclExample.cpp
        src/io/Meshloader.cpp
        src/io/Meshloader.h
        src/gui/ImageWidget.cpp
        src/gui/ImageWidget.h
        src/ocl/OclImage.cpp
        src/ocl/OclImage.h
        src/ocl/OclBuffer.cpp
        src/ocl/OclBuffer.h
        src/ocl/OclProgram.cpp
        src/ocl/OclProgram.h
        src/ocl/OclKernel.cpp
        src/ocl/OclKernel.h
        src/ocl/OclCommandQueue.cpp
        src/ocl/OclCommandQueue.h
        src/ocl/OclContext.cpp
        src/ocl/OclContext.h
        src/ocl/OclSampler.cpp
        src/ocl/OclSampler.h
        src/ocl/OclEvent.cpp
        src/ocl/OclEvent.h
        src/ocl/OclEngine.cpp
        src/ocl/OclEngine.h
        src/ocl/OclHeader.h
        src/ocl/OclErrors.cpp
        src/ocl/OclErrors.h
        src/imageFilters/ImageFilter.cpp
        src/imageFilters/ImageFilter.h
        src/common/Image.cpp
        src/common/Image.h
        src/io/clSource.cpp
        src/io/clSource.h)

#####################################################################
###           objloader Dependencies                             ###
#####################################################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/objloader)
SET(OBLLOADER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/objloader/src)
SET(OBLLOADER_LIBRARY_DIR /src/thirdparty/objloader/)
SET(OBJLOADER_LIBARIES objloader)

include_directories(${OBLLOADER_INCLUDE_DIR})
link_directories(${OBLLOADER_LIBRARY_DIR})


#####################################################################
###           OpenCL
#####################################################################
find_package(OpenCL REQUIRED)
#if (OpenCL_FOUND)
    include_directories(${OpenCL_INCLUDE_DIRS})
    link_directories(${OpenCL_LIBRARIES})
    link_libraries(${OpenCL_LIBRARIES})
    #elseif(NOT OpenCL_FOUND)
    #message("OpenCL is not installed!")
    #endif()


#####################################################################
###           OpenGL
#####################################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
link_libraries(${OPENGL_LIBRARIES})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)


#####################################################################
###           Copy CL Source to deploy directory
#####################################################################
set(CL_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cl/)
file(REMOVE_RECURSE ${CL_OUTPUT_DIR})
file(REMOVE ${CL_OUTPUT_DIR}/imageFilters/programes/*.cl)
file(COPY src/imageFilters/programes/ DESTINATION "${CL_OUTPUT_DIR}/imageFilters/programes/")


add_definitions("-DDEBUG")
add_executable(OclExample ${SOURCE_FILES})
target_link_libraries(OclExample Qt5::Widgets Qt5::Core Qt5::Gui)
target_link_libraries(OclExample objloader)
