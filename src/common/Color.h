//
// Created by Sanqian on 16/7/13.
//

#ifndef OCLEXAMPLE_COLOR_H
#define OCLEXAMPLE_COLOR_H


class Color {

public:
    Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a )
    {
        this->_r = r;
        this->_g = g;
        this->_b = b;
        this->_a = a;
    }
    
    unsigned int r(){ return _r;}
    unsigned int g(){ return _g;}
    unsigned int b(){ return _b;}
    unsigned int a(){ return _a;}

private:

    unsigned int _r;
    unsigned int _g;
    unsigned int _b;
    unsigned int _a;
};


#endif //OCLEXAMPLE_COLOR_H
