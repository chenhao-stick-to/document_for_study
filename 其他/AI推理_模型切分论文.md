# 1.数据并行



![image-20230630111141564](../../../AppData/Roaming/Typora/typora-user-images/image-20230630111141564.png)

如图，多GPU的数据并行需要进行数据的一个同步，上传到参数服务器再统一更新。

# 2.模型并行

![image-20230630111557860](../../../AppData/Roaming/Typora/typora-user-images/image-20230630111557860.png)

多gpu的训练过程中，不同子模型间存在依赖关系，有依赖关系的后面的子模型需要等待前面的子模型的结果，以及数据传输等需要很大的消耗。

# 3.模型切分方法

![image-20230630112354480](../../../AppData/Roaming/Typora/typora-user-images/image-20230630112354480.png)

![image-20230630112424362](../../../AppData/Roaming/Typora/typora-user-images/image-20230630112424362.png)

模型并行和数据并同时进行：

![image-20230630112514851](../../../AppData/Roaming/Typora/typora-user-images/image-20230630112514851.png)

# 4.具体实现

![image-20230630113741075](../../../AppData/Roaming/Typora/typora-user-images/image-20230630113741075.png)

master作用：

进行数据的预取，预取各个任务的batch数据。

记录W-slave注册信息，包括几张卡，显存大小，传输速度，W-slave间的传输速度。获取模型几层次关系。

根据模型结构，统计模型占用内存大小，再根据W-slave计算出最优化模型和数据并行方案。

# 5.如何构建出最优的模型切分以及数据切分的方案

可根据模型的切分度以及数据并行的度去求所预估训练花费时间的最小值，本质是一个最优化的问题。





