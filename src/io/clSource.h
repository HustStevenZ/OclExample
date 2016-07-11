//
// Created by Sanqian on 16/7/11.
//

#ifndef OCLEXAMPLE_CLSOURCE_H
#define OCLEXAMPLE_CLSOURCE_H


#include <string>

class clSource {

private:
    char* _data = nullptr;

    std::string _filePath;

public:
    clSource(std::string filePath);

    ~clSource(){
        if(_data!= nullptr)
        {
            delete _data;
        }
    }

    std::string getSource(){
        if(_data== nullptr)
            readFile();

        return _data;
    }

private:
    void readFile();

};


#endif //OCLEXAMPLE_CLSOURCE_H
