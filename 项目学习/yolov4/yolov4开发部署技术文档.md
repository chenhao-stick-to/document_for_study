分为五大步来走。

#第一步
我们需要训练模型，根据任务，我们需要选取合适的目标检测模型，以及在选取模型时如果想在推理偷一点懒，可以选取一些众多推理框架都做了适配的模型，例如yolov5，yolov7系列模型。
在训练模型时，需要考虑到输入数据是否需要数据的预处理，需要做哪些特殊的预处理，了解模型的输入和输出，以及根据这个输出到最终结果的处理需要详细了解。

#第二步
我们需要将训练好的pt模型转换为真实的推理模型，怎样处理？
这里方向就非常多了；首先目标平台是什么呢？这里只用cpu推理，所以基本主流推理框架都可以支持。
(1)尝试了ncnn框架（移动端，手机端首选），ncnn分为两个方向转化模型。
##第一个是pt->onnx->ncnn模型，这个路线里面，pt到onnx一般情况下没有问题，这里需要注意的是我们需要导出的模型输出是什么，可以根据pytoch的model类的源码来进行修改的，这里有一个路线技巧就是，如果model里面forward函数在模型输出后，有很多自己的计算操作，
这些计算操作在后面模型转换是不支持的，那么就可以删掉这些操作，将这些操作放到后处理去实现。还要注意的是中间需要使用onnxsim来进行模型的简化，最终推理结果为nan，有可能是算子的问题导致结果溢出，也有可能是动态尺寸的问题和ncnn的batch为1的不匹配等。。
##第二个是pt->pnnx->ncnn模型，在这个路线里面，需要使用ncnn提供的转模型工具，可同时转出pnnx和ncnn模型，但是还是跑不了？需要排查原因。
（2）使用onnxruntime自带的框架。
##出现了onnxruntime自带的docker无法构建成功的错误。
##出现了onnxruntime的源码编译不发通过的问题。
（3）使用fastdeploy进行模型推理。
##构建推理成功，目前是使用基于scaleyolov4的推理，仔细对比了python版的前后处理，模型的后处理是差不多的吗，主要是模型的前处理是需要筛检一些东西的，最后成功推理，暂时还没有作全部数据的测试。

#第三步
需要将模型推理的任务封成一个微服务，需要使用三方库，这里总结一下。
cpp-jwt/nolhmn的json库，值得注意的是这两个库是header-only的库，这两个分别用于jwt的token鉴权，以及json数据的保存使用。
glog库，需要作为三方库的依赖库。
openssl库是系统缺少libcrypto，libssl等库，貌似是cpp-jwt的依赖库。
restbed库是http的三方库，用于封装http的接口。
zlib-1.3这个库主要用于实现将模型进行加密的压缩以及解压缩，目的是实现模型对外部的不可见性。
目前只实现了yolov4对cpu的推理，需要实现对gpu推理服务的封装在以后，虽然很简单，后面再做。

#第四步 对各个模块的编译命令。
mkdir build &&cd build && cmake .. -DENABLE_ORT_BACKEND=ON    -DENABLE_OPENVINO_BACKEND=ON   -DCMAKE_CXX_STANDARD=17  -DENABLE_VISION=ON -DCMAKE_INSTALL_PREFIX=${PWD}/compiled_fastdeploy_sdk && make -j32 && make install &&cd ..
echo "fastdeploy sucess"

##构建glog
cd third_party/glog && mkdir build && cd build &&cmake .. -DCMAKE_INSTALL_PREFIX=${PWD}/compiled_glog_sdk && make -j24 &&make install &&cd ..
echo "glog sucess"

##构建restbed
cd ../restbed && mkdir build && cd build && cmake .. -DCMAKE_INSTALL_PREFIX=${PWD}/compiled_resetbed_sdk && make -j24 && make install &&cd ..
echo restbed sucess""

##构建openssl
cd ../openssl && ./config shared  --prefix=${PWD}/compiled_openssl_sdk && make -j24  && make install &&cd ..
echo "openssl sucess"

##构建zlib-1.3
###从https://github.com/madler/zlib.git这个网址git clone
查看https://blog.csdn.net/m0_66220428/article/details/122463107进行源码的编译，编译zlib以及minizip生成对应的动态库。
进行本地的编译测试，生成可执行文件可以顺利的执行。

#第五步 将准备好的源码封装为docker镜像。
踩坑点，第一个是直接将宿主机编译可执行文件和对应的三方库构建dockerfile时拷贝进去直接执行，貌似是不能运行的，没有再做实验，可能是路径不对，印象中ldd能找到所有库，只是不能运行。
在这种情况下我们需要将源码拷贝到base镜像构造的容器中，然后再这个容器里将所有库编译一下，在这个编译过程中，需要下载了许多系统库，猜测可能是三方库的依赖库在宿主机系统中，所以导致直接拷贝编译好的三方库到docker无法直接执行，
##两阶段构建时不成功，貌似是在咋子第一阶段直接dockerfile编译出了问题，缺少相应的系统库后来发现。
##在这种情况下，我的解决办法是，直接复制源码到base镜像中，然后手动来构建可以运行的环境，手动编译三方库以及安装所缺少的系统库依赖，然后再一阶段构建新的只有可执行文件和所需依赖库和必要文件的镜像。
###这里需要注意的是，重点是，其实再刚一开始就一个在一个纯净的base镜像来进行项目，这样我们最终得到的是一个独立于宿主机的纯净镜像，后面再使用同一个base镜像，写dockerfile一阶段构建一个极简的镜像。


