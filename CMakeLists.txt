cmake_minimum_required(VERSION 3.5)
project(uniubi_license)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)
if(CMAKE_BUILD_TYPE MATCHES "None")
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Protobuf REQUIRED)

add_library(caffe_encrypt SHARED caffe_encrypt.cpp caffe.pb.cc)
target_link_libraries(caffe_encrypt ${PROTOBUF_LIBRARY} glog)
add_executable(encrypt_tool encrypt_tool.cpp)
target_link_libraries(encrypt_tool caffe_encrypt)

#add_executable(test_feature test.cpp)
#target_link_libraries(test_feature blobd)