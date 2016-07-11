//
// Created by Sanqian on 16/7/11.
//

#include <ios>
#include <fstream>
#include "clSource.h"
clSource::clSource(std::string filePath) {
    _filePath=filePath;
}

void clSource::readFile() {
    std::ifstream ifstream(_filePath);

    ifstream.seekg(0,ifstream.beg);
    unsigned long begin = ifstream.tellg();
    ifstream.seekg(0,ifstream.end);
    unsigned long end = ifstream.tellg();
    unsigned long filezie = end-begin;
    ifstream.seekg(0,ifstream.beg);

    if(_data!= nullptr)
    {
        delete _data;
    }
    _data=new char[filezie];
    ifstream.read(_data,filezie);
    ifstream.close();
}