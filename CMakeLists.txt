cmake_minimum_required(VERSION 3.5)
project(OclExample)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

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
set(SOURCE_FILES src/gui/CentralWidget.cpp src/gui/CentralWidget.h src/OclExample.cpp src/misc/Meshloader.cpp src/misc/Meshloader.h src/gui/ImageWidget.cpp src/gui/ImageWidget.h src/gui/ImageWidget.cpp src/gui/ImageWidget.h src/ocl/OclImage.cpp src/ocl/OclImage.h src/ocl/OclBuffer.cpp src/ocl/OclBuffer.h src/ocl/OclProgram.cpp src/ocl/OclProgram.h src/ocl/OclKernel.cpp src/ocl/OclKernel.h src/ocl/OclCommandQueue.cpp src/ocl/OclCommandQueue.h src/ocl/OclDevice.cpp src/ocl/OclDevice.h src/ocl/OclSampler.cpp src/ocl/OclSampler.h src/ocl/OclEvent.cpp src/ocl/OclEvent.h src/ocl/OclEngine.cpp src/ocl/OclEngine.h)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/objloader)
SET(OBLLOADER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/objloader/src)
SET(OBLLOADER_LIBRARY_DIR /src/thirdparty/objloader/)
SET(OBJLOADER_LIBARIES objloader)

include_directories(${OBLLOADER_INCLUDE_DIR})
link_directories(${OBLLOADER_LIBRARY_DIR})
add_executable(OclExample ${SOURCE_FILES})
target_link_libraries(OclExample Qt5::Widgets Qt5::Core Qt5::Gui)
target_link_libraries(OclExample objloader)