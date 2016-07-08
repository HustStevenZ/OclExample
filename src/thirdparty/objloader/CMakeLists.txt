cmake_minimum_required(VERSION 3.5)
project(objloader)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(LIBRARY_OUTPUT_PATH $(CMAKE_CURRENT_SOURCE_DIR)/bin)
set(SOURCE_FILES
    src/list.cpp
    src/list.h
    src/obj_parser.cpp
    src/obj_parser.h
    src/objLoader.cpp
    src/objLoader.h
    src/string_extra.cpp
    src/string_extra.h)

add_library(objloader SHARED ${SOURCE_FILES})