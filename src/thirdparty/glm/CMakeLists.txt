cmake_minimum_required(VERSION 3.5)
project(glm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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

set(SOURCE_FILES
    glm.c
    glm.h
    glm_util.c
    glmimg.c
    glmimg_devil.c
    glmimg_jpg.c
    glmimg_png.c
    glmimg_sdl.c
    glmimg_sim.c
    glmint.h)

add_library(glm SHARED ${SOURCE_FILES})