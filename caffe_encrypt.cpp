//
// Created by liuhao on 9/6/18.
//

#include "caffe_encrypt.h"
#include <fstream>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <glog/logging.h>
#include <fcntl.h>
#include <sstream>
#include "caffe.pb.h"

using google::protobuf::io::FileInputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::CodedInputStream;
using google::protobuf::Message;

void caffe_encrypt::encrypt_binary(const std::string &file_name, const std::string &passwd,
                                   const std::string &output_file){
    if(passwd.empty())
        return;

    std::ifstream in_file(file_name,std::ios::binary);

    if(!in_file)
        return;

    std::streampos begin, end;
    begin = in_file.tellg();
    in_file.seekg(0, std::ios::end);
    end = in_file.tellg();
    std::size_t size = end - begin;
    fprintf(stdout, "engine file size: %d bytes\n", size);
    in_file.seekg(0, std::ios::beg);

    std::vector<unsigned char> engine_data(size);
    in_file.read((char*)engine_data.data(), size);
    in_file.close();

    std::ofstream out_proto(output_file, std::ios::out | std::ios::binary);
    if (!out_proto.is_open()) {
        fprintf(stderr, "fail to open file to write: %s\n", output_file.c_str());
        return;
    }

    //encode
    if(passwd.size()>0)
    {
        int password_len=passwd.size();
        for(int i=0;i<engine_data.size();++i)
        {
            unsigned char before_encode=engine_data[i];
            unsigned char encode_word=(before_encode)^(unsigned char)passwd[i%password_len];
            out_proto.put(encode_word);
            //outfile.write((char*)&encode_word, sizeof(char));
        }
    }

    out_proto.close();
}

void caffe_encrypt::encrypt_proto(const std::string &file_name, const std::string &passwd,
                                   const std::string &output_file){
    if(passwd.empty())
        return;

    std::ifstream in_file(file_name);

    if(!in_file)
        return;

    std::streampos begin, end;
    begin = in_file.tellg();
    in_file.seekg(0, std::ios::end);
    end = in_file.tellg();
    std::size_t size = end - begin;
    fprintf(stdout, "engine file size: %d bytes\n", size);
    in_file.seekg(0, std::ios::beg);

    std::vector<unsigned char> engine_data(size);
    in_file.read((char*)engine_data.data(), size);
    in_file.close();

    std::ofstream out_file(output_file,std::ios::binary);

    //encode
    if(passwd.size()>0)
    {
        int password_len=passwd.size();
        for(int i=0;i<engine_data.size();++i)
        {
            unsigned char before_encode=engine_data[i];
            unsigned char encode_word=(before_encode)^(unsigned char)passwd[i%password_len];
            out_file.put(encode_word);
            //outfile.write((char*)&encode_word, sizeof(char));
        }
    }

    out_file.close();
}

void caffe_encrypt::encrypt(const std::string &prefix, const std::string &passwd,
                            const std::string &output_name)
{
    if(passwd.empty())
        return;

    //encode prototxt
    encrypt_proto(prefix+".prototxt",passwd,output_name+".prototxt");

    //encode model
    encrypt_binary(prefix+".caffemodel",passwd,output_name+".caffemodel");
}

void caffe_encrypt::decrypt_binary(const std::string& file_name,const std::string& passwd,caffe::NetParameter& params)
{
    std::ifstream in_file(file_name,std::ios::binary);

    if(!in_file)
        return;

    std::streampos begin, end;
    begin = in_file.tellg();
    in_file.seekg(0, std::ios::end);
    end = in_file.tellg();
    std::size_t size = end - begin;
    fprintf(stdout, "engine file size: %d bytes\n", size);
    in_file.seekg(0, std::ios::beg);

    std::string engine_data(size,'c');
    in_file.read((char*)engine_data.data(), size);
    in_file.close();

    int pwd_len=passwd.size();

    if(pwd_len>0)
    {
        //decrypt
        for (size_t i = 0; i < engine_data.size(); ++i)
        {
            engine_data[i]=(((unsigned char)engine_data[i])^((unsigned char)passwd[i%pwd_len]));
        }
    }

    std::istringstream net_model(engine_data);
    google::protobuf::io::IstreamInputStream* model_input=new google::protobuf::io::IstreamInputStream((std::istream*)(&net_model));
    google::protobuf::io::CodedInputStream* coded_input_model=new google::protobuf::io::CodedInputStream(model_input);
    coded_input_model->SetTotalBytesLimit(INT_MAX,536870912);
    params.ParseFromCodedStream(coded_input_model);

    delete model_input;
    delete coded_input_model;

    return;
}

void caffe_encrypt::decrypt_proto(const std::string &file_name, const std::string &passwd, caffe::NetParameter& params){
    std::ifstream in_file(file_name,std::ios::binary);
    if(!in_file)
        return;

    std::streampos begin, end;
    begin = in_file.tellg();
    in_file.seekg(0, std::ios::end);
    end = in_file.tellg();
    std::size_t size = end - begin;
    fprintf(stdout, "engine file size: %d bytes\n", size);
    in_file.seekg(0, std::ios::beg);

    std::string engine_data(size,'c');
    in_file.read((char*)engine_data.data(), size);
    in_file.close();

    int pwd_len=passwd.size();

    if(pwd_len>0)
    {
        //decrypt
        for (size_t i = 0; i < engine_data.size(); ++i)
        {
            engine_data[i]=(((unsigned char)engine_data[i])^((unsigned char)passwd[i%pwd_len]));
        }
    }

    std::istringstream net_proto(engine_data);

    google::protobuf::io::IstreamInputStream* proto_input=new google::protobuf::io::IstreamInputStream((std::istream*)(&net_proto));
    google::protobuf::io::ZeroCopyInputStream* coded_input_proto=proto_input;

    google::protobuf::TextFormat::Parse(coded_input_proto,&params);

    delete proto_input;

    return;
}