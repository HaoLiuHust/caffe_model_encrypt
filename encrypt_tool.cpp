//
// Created by liuhao on 9/6/18.
//

#include "caffe_encrypt.h"
#include <string>
#include <iostream>
int main(int argc,char* argv[])
{
    if(argc<4)
    {
        std::cerr<<"argument not enough"<<std::endl;
        return -1;
    }

    std::string prefix=argv[1];
    std::string pwd=argv[2];
    std::string output_name=argv[3];

    caffe_encrypt::encrypt(prefix,pwd,output_name);

    return 0;
}