# faiss和rocksdb的编译实现
- 编译fastdeploy库
```c++
  cmake .. -DENABLE_ORT_BACKEND=ON \
         -DENABLE_OPENVINO_BACKEND=ON \
         -DENABLE_TRT_BACKEND=ON \
         -DWITH_GPU=ON \
         -DTRT_DIRECTORY=/usr/local/TensorRT-8.5.1.7 \
         -DCUDA_DIRECTORY=/usr/local/cuda-11.3 \
         -DENABLE_VISION=ON \
         -DCMAKE_INSTALL_PREFIX=${PWD}/compiled_fastdeploy_sdk \
  make -j24
  make install
```
  ==提前安装好对应的需自己安装的库，例如cuda或者tensorrt==

- 编译faiss库
  cmake .. -DCUDAToolkit_ROOT=/usr/local/cuda-11.3 -DCMAKE_CUDA_COMPILER=/usr/local/cuda-11.3/bin/nvcc -DCMAKE_INSTALL_PREFIX=${PWD}/compiled_faiss_sdk -DFAISS_ENABLE_GPU=ON -DFAISS_ENABLE_PYTHON=OFF -DBUILD_SHARED_LIBS=ON
  make -j24
  make install
  ==注意faiss库的动态编译以及静态编译==
- 编译rocksdb库
  
# faiss的简单使用
# rocksdb的简单使用
