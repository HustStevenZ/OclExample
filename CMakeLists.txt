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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/CentralWidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/CentralWidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/OclExample.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/io/Meshloader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/io/Meshloader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/ImageWidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/ImageWidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclImage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclImage.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclBuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclBuffer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclProgram.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclProgram.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclKernel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclKernel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclCommandQueue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclCommandQueue.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclContext.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclSampler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclSampler.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclEvent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclEvent.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclEngine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclEngine.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclHeader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclErrors.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocl/OclErrors.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imageFilters/ImageFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imageFilters/ImageFilter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Image.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Image.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/io/clSource.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/io/clSource.h)

#####################################################################
###           objloader Dependencies                             ###
#####################################################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/objloader)
SET(OBLLOADER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/objloader/src)
SET(OBLLOADER_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/objloader/)
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
