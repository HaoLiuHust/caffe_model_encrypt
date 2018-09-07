//
// Created by liuhao on 9/6/18.
//

#ifndef GRPC_DATAFORMAT_CAFFE_ENCRYPT_H
#define GRPC_DATAFORMAT_CAFFE_ENCRYPT_H

#include <string>
#include <google/protobuf/text_format.h>
namespace caffe
{
    class NetParameter;
}
class caffe_encrypt
{
public:
    static void encrypt(const std::string& prefix, const std::string& passwd,const std::string& output_name);
    static void decrypt_proto(const std::string& file_name,const std::string& passwd,caffe::NetParameter& params);
    static void decrypt_binary(const std::string& file_name,const std::string& passwd,caffe::NetParameter& params);

private:
    static void encrypt_proto(const std::string& file_name,const std::string& passwd,const std::string& output_file);
    static void encrypt_binary(const std::string& file_name,const std::string& passwd,const std::string& output_file);

};

#endif //GRPC_DATAFORMAT_CAFFE_ENCRYPT_H
