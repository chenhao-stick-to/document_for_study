Tensorrt做的工作：

**构建期**

（1）模型解析,建立，即导入ONNX导入其他模型，或者直接用原生API搭建的模型

（2）计算图优化 横向层融合，纵向层融合

![image-20230714162952632](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230714162952632.png)、

（3）节点消除 去除无用层，节点变换（Pad，Slice等）

（4）多精度支持 FP32/FP16/INT8/TF32 （可能插入reformat）

（5）优选kernel/format 核函数等涉及具体的硬件架构，需要根据具体的硬件来实现对其的优化。

（6）导入plugin  有时tensort不能原生支持的操作，需要导入cuda c++编写的额外的plugin。

（7）显存优化  tensort维护一个显存池

运行期（**运行时环境**）：

![image-20230714163624536](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230714163624536.png)



###### ![image-20230714163936301](C:\Users\33012\AppData\Roaming\Typora\typora-user-images\image-20230714163936301.png)