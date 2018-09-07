# a simple tool for encrypt and decrypt caffe model
**prequirement**:  
protobuf  
glog  
**how to use**:   
1.build
```
mkdir build
cd build
cmake ..
make
```  
2.encrypt a model  
```
encrypt_tool has 3 args, the first one is the model prefix with its path,for example if you have a model named mnist which has mnist.prototxt and mnist.caffemodel,then the prefix will be $PATH/mnist,the second one is the password for encrypt,the last one is the output file name, for example:

./encrypt_tool mode_prefix passwd output_name
```  
3.decrypt a model for use
```
let's say we are going to init a caffe Net with encrypt file: mnist.prototxt and mnist.caffemodel, the code will be like:
shared_ptr<Net> net_ptr;
std::string pwd="my_encrypt_path";

NetParameter proto;
caffe_encrypt::decrypt_proto(protopath,pwd,proto);//decrypt proto
net_ptr.reset(new Net{proto,caffe::TEST});

NetParameter modelparams;
caffe_encrypt::decrypt_binary(modelpath,pwd,modelparams);//decrypt params

net_ptr->CopyTrainedLayersFrom(modelparams);

then you can use the model as before
```