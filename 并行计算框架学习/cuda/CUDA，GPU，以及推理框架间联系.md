# CUDA整体结构，扮演的角色

CUDA和OpenCL，对比。

![image-20230710170251963](C:\Users\ndsl2023\AppData\Roaming\Typora\typora-user-images\image-20230710170251963.png)

GPGPU可以使用GPU做一些通用CPU之间需要做的事情，更高效率的进行优化.

![image-20230711144938075](C:\Users\ndsl2023\AppData\Roaming\Typora\typora-user-images\image-20230711144938075.png)



- # 第一部分

  师兄师姐ppt略读

# 多CPU系统存在问题

- 同步问题：单核cpu不存在高速缓存的一致性问题，多核cpu存在缓存一致性问题，当多核对参数进行重写时，核一写入参数时通知其他核x已发生改变，写到主存而不是高速缓存中。用更新请求替换掉x的无效请求。

- 负载均衡问题：进行工作任务的平衡分配，防止cpu过载或者过于空闲。

- 通信问题：cpu间进行共享数据以及协调任务时，高效通信协议。。

- 编程问题：并行编程技术，例如**OPENMP**，MPI等。

# 多GPU系统

cuda二进制文件会保留多个二进制镜像，对应每一代GPU。对某些功能，只支持特定计算水平设备，注意设备选择。可以调用cudagetdevicecount（）获取设备数量，调用cudagetdeviceproperties（）获取特定设备的属性，cudasetdevice（）可以自己选择device，也可以让cuda运行时自动选择。

多GPU算法注意问题：

- 数据分割：数据分块，尽量少关联性，减少数据传输的成本。

- 并行计算：并行计算算法，不同gpu上同时执行计算任务，考虑数据分割，以及最后的数据合并。

- 数据传输：GPU间传输，考虑传输带宽以及延迟，预取数据技术。数据预取技术即异步传输，考虑计算和内存GPU缓存间传输数据异步的进行，需要注意内存传输以及计算之间的依赖关系，cudastream可解决。

- 负载均衡：多GPU上执行计算任务时，保证每个GPU的计算负载尽可能的均衡。 动态调整任务分配。

# 流

流是GPU上的虚拟工作队列，用于异步操作。流是一种执行并行计算任务的机制，流可以看作是一组任务的容器，流的任务可以是计算任务也可以是内存传输的任务。流的优点在于可高效的利用GPU的并行计算能力，gpu可同时执行多个流任务。gpu中每个流有唯一的标识符，可通过CUDA的API来创建，管理和销毁流。流的任务可以顺序提交或无序提交。顺序按顺序执行，无序需按照任务优先级。

流中任务通常有：计算任务，内存传输任务，核函数调用（特殊的计算任务）。

**使用流注意问题：**

- 流之间的任务不能相互依赖，流是并行执行的，如存在依赖关系，需要使用同步机制来确保任务的正确执行顺序。

- 流间任务要进行合理的调度：使用优先级和时间戳等机制来调度不同流间的任务。

- 流需要适当的内存管理：不同流间共享GPU内存和主机内存，需要适当的内存管理。使用CUDA的API来分配和释放内存，使用内存传输任务管理数据传输。

  # 多节点系统

理想CPU核与GPU的比率取决于应用程序和代码串行部分百分比。看CPU是否过载，过载需使用多CPU。注意使用通信机制将工作调度到给定的CPU/GPU集合上，而不用关心位于网络什么位置。

![image-20230710110255508](C:\Users\ndsl2023\AppData\Roaming\Typora\typora-user-images\image-20230710110255508.png)

# zeroMQ 

zeromq是一个高性能，轻量级，开源的消息传递库。

特点：轻量级，高性能（异步IO，多路复用），跨平台，灵活性（多种传输协议TCP，UDP，inproc），可靠性，易用性。

工作流程：

建立一个连接点，主从范型。主机器将工作包分发给客户端，客户端连接到网络某一个访问点（服务器提供），zeromq为每一个来连接的客户端建立内部队列。

![image-20230710141028308](C:\Users\ndsl2023\AppData\Roaming\Typora\typora-user-images\image-20230710141028308.png)

确定一个消息传递的模式，如最简单的请求响应模式。cpu客户端维护一个工作队列，至少两项（GPU双缓冲），至少一个入队和出队的网络消息。cpu客户端连接到服务器并请求一批工作，服务器返回客户端处理范围的响应，客户端在cpu执行必要的工作来为工作包产生数据。

# MQ/MPI对比

MQ是一种基于消息通信的中间件，MQ通常用于异步通信，通过将消息写入对立队列中，来实现不同进程或者不同机器间的通信。适用于简单的分布式应用场景，如消息传递，任务分发。。

MPI是一种高性能的消息传递框架，适用于在并行计算机集群中进行通信和协作。MPI是一种同步通信，MPI需要使用专门的库和API进行编程，适用于hpc,科学计算等复杂分布式场景。

主要区别：

通信方式，应用场景，实现方式，性能层面（MPI通常比MQ有更高的性能）。

MQ应用场景：电商系统，社交网络，微服务架构间

MPI应用场景：HPC，科学计算复杂的分布式应用场景；气象学模型计算，物理学粒子模拟，深度学习使用MPI并行计算神经网络的训练和优化。

MQ实现方式： 基于中间件实现（Apache Kafka，RabbitMQ等），中间件提供基本功能：消息写入，读取，路由，过滤等。。Java开发，基于JMS规范实现MQ。

MPI实现方式：基于底层通信库实现如OpenMPI，MPICH等实现。开发人员需要编写MPI程序来实现消息的发送和接收。（cuda可以和open mpi集成，通过open mpi提供的函数以及工具，编写支持并行计算和通信的cuda mpi）。

MQ的性能：消息队列涉及序列化和反序列化，中间件的网路通信等，速度受限。异步通信使消息延迟和不确定性，高性能和实时性的场景中，MQ可能并不是最佳选择。

MPI的性能：MPI通常用于高性能计算和科学计算等复杂的分布式应用场景。MPI的同步通信高效，确定，还可以使用高效的通信协议，如RDMA。

多节点GPU系统面临的主要问题包括：通信延迟，负载均衡，数据传输和同步，可靠性和容错性（单点故障等）。

优化方案：RDMA优化通信，负载均衡（静态负载均衡和动态负载均衡等），数据传输与同步（异步数据传输，数据压缩，数据分片），可靠性和容错性（数据备份，冗余计算节点，GPU卡故障检测和自动切换）。



- # 第二部分

  cuda编程模型解析

# GPU体系结构和CUDA编程模型关联：

![image-20230713093041165](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713093041165.png)



CPU是低延迟设计，（1）ALU强大，时钟频率高。（2）容量较大的cache，占据较大部分空间。（3）CPU有复杂控制

逻辑。

![image-20230713094521350](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713094521350.png)



GPU为高吞吐设计，（1）GPU有大量的ALU.（2）cache很小，为theread服务。（3）无复杂的控制逻辑。



![image-20230713095005340](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713095005340.png)



CPU和GPU的数据交互

![image-20230713095813640](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713095813640.png)



### CUDA编程模型：

CUDA程序分为：CPU上的Host程序，在GPU上的Device程序。GPU上运行的函数又叫核函数，__gloabal__关键字声明，

##### 线程层次：

CUDA的线程组织分为Grid，Block，Thread三个层次。每个Block中包含的线程数量是有限制的，因为目前每个块内的所有线程都在一个物理的处理器核中，并且共享这个核有限的内存资源。  

![image-20230713100123899](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713100123899.png)

threadidx的计算：

![image-20230713103326770](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713103326770.png)

![image-20230713101634043](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713101634043.png)

如上所示的cuda编程示例，其将原始数据N'x'N的数据，划分为16*16的小块，块为一个二维block矩阵，线程块也为一个16‘x’16的的线程矩阵，这样可以将原始数据的每个元素分配到具体的线程中去计算，而且这个线程组织具有数组结构性，天然支持并发的矩阵分块运算。

**GPU的线程映射关系**：

一个kernel函数程序，会组织一个grid，并且需要指定block块以及每个block块的线程数量；然后由SM来进行wrap分组：

![image-20230713152634775](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713152634775.png)



线程束warp是CUDA中SM的最小执行单位：

![image-20230713153807678](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713153807678.png)

![image-20230713153718261](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713153718261.png)

#### 内存层次

![image-20230713104401121](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713104401121.png)

 

CUDA thread在执行时，可以访问多个内存空间。每个线程自己的寄存器空间，block有共享内存空间；block之间，或者grid之间通过全局内存空间进行通信。

内存访问速度：寄存器>共享>全局

cudamalloc开辟的是全局内存，核函数用__shared__修饰的变量为共享内存变量，核函数自定义变量为寄存器内存变量。

- # 第三部分

#### cuda体系在深度学习推理中的作用，**以及生态位分析**。

cuda在nvidia体系中，扮演着非常重要的作用。如图：

![image-20230713110251475](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713110251475.png)



![DL_software_chart_001.png (1200×643)](https://developer.nvidia.com/sites/default/files/akamai/cuda/images/deeplearning/Thumbnails/DL_software_chart_001.png)

其中cudnn深度学习训练加速算子库，tensorrt的推理加速核函数库，cudaBLAS等等多个基于CUDA的NVIDIA自家开发的各种加速核函数库更是构成一道技术壁垒。以tensort为例，这是一款nvidia推出的针对nvidia的GPU的深度学习推理框架，其上可以将神经网络模型转换为具体的计算图，在其内部对深度学习的各种算子进行了分解优化，将算子划分为多个CUDA核函数，并合理组织分配线程块，块数进行数据计算。但是其并不开源。。

#### CUDA和OpenCL的对比：

![image-20230713113219800](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713113219800.png)

CUDA和OPenCL均可以进行硬件的并行计算，其对比如图：

![image-20230713113608556](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713113608556.png)



对于TVM，CUDA和OPenCL等联系？

![image-20230713105905220](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230713105905220.png)

问题：

（1）CUDA的线程束组织，并行度和SM有关还是线程束等。**已解决**

（2）OPenCL底层怎样实现的兼容多个硬件厂商硬件，貌似是提供统一的OPenCL编程规范，不同硬件厂商开发自己的OPenCL编译器来将统一规范代码编译为该硬件平台可执行代码。**已解决**，opencl包含运行时库和底层驱动程序。运行时库接受opencl的内核函数，并根据具体的硬件平台实现对应的编译器将该代码编译成硬件平台上的可执行代码。OPencl集成了对应厂商的编译器以及驱动。。

（3）TVM上可以兼容多种模型格式，将多种模型格式转换为TVM输入格式，中可进行计算图优化，下是直接对接OPenCL，CUDA等并行计算框架，及转化为框架对应的代码，如CUDA的核函数；还是可以直接编译为可执行在硬件层面直接驱动硬件的代码？？？**已解决**。TVM前端进行深度学习模型的解析，中端进行计算图的优化，后端和已经和各硬件平台对接的并行计算框架对接。。

（4）TVM貌似只是进行模型的推理计算，那么分布式协同是否需要其他的分布式框架，进行数据的协同，同步处理。CUDA MPI，gRPC，MQ等等？？？？？

第四部分 Tensort和CUDA的联系/TVM和cuda的联系

