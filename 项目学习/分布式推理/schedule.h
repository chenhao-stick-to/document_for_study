#include<string>
#include<vector>
#include<map>
using namespace std;

enum TaskType{//任务类型
INFERENCE,
DEPLOY,
RECYCLE,
GETRESOURCE
};

class Param{};//参数信息

enum TaskInfo{
    WAIT,
    RUNNNING,
    COMPLETE,
};//任务状态

class TaskMeta{//任务元数据信息
public:
  int task_id;//任务id
  TaskType tasktype;//任务类型
  string  pic_data;//数据临时存储的文件名
  string model_data;//模型的存储地.
  Param* param_data;//指定的一些参数
  TaskInfo taskinfo;//任务状态 
};

class HandwareResource{//硬件资源信息，单节点
public:
  int avaliable_gpu;//该节点可用的cpu，gpu数量
  int avaliable_cpu;
  vector<int> maxmem_gpu;//最大的cpu，gpu内存
  vector<int> maxmem_cpu;
  vector<int> freemem_gpu;//剩下空闲cpu，gpu内存
  vector<int> freemem_cpu;
  vector<float> gpu_usepercent;//每个可用cpu，gpu利用百分比；可用来动态调整节点上的pipline(容器数量)
  vector<float> cpu_usepercent;
  //TODO 其他待添加内容
};

class ResourceList{//资源列表；一是节点硬件资源，二是节点的通信带宽使用情况
  vector<HandwareResource> cluster_handwareresource;
  vector<vector<int>> cluster_communresource;//矩阵存储节点间的通信带宽。//TODO 数据结构再考虑
  vector<int> clusternode_maxcommunresource;//节点的最大通信带宽，在初始化确定。
};
class PipeNode{//每一个pipenode的所需信息
/* 
node的具体位置;传入node的信息
部署时：
每个节点有对应的模型位置信息
推理：
传入节点数据
*/
};

class PipeLine{//pipline信息
  int piplineid_;//每个pipline有对应的id号
  TaskMeta taskmeta_;//任务信息；推理包含数据性信息
  vector<PipeNode> pipe_nodes_;
  vector<string>   model_name_;//每个pipenode的模型名，部署时
};

class TaskBasic{//基础类的任务（目前是资源查询）
  public:
    TaskBasic();
    TaskBasic(TaskMeta& task_meta,ResourceList &resourcelist):task_meta_(task_meta),resourcelist_(resourcelist){}
    ResourceList &GetClusterResourceInfo(){/*do nothing now 获取底层信息返回给resourcelist_，是否需要pipline信息*/ return resourcelist_;}//获取资源信息
    virtual PipeLine& start(){};//任务开始
  private:
    TaskMeta& task_meta_;
    ResourceList& resourcelist_;
};
class TaskInference:public TaskBasic{//推理任务
  public:
  TaskInference();
  TaskInference(TaskMeta& task_meta,ResourceList &resourcelist,PipeLine& pipline):task_meta_(task_meta),resourcelist_(resourcelist),pipline_(pipline){}
  PipeLine& start(){
    /*
    do nothing now;
    可能需要做推理原模型到切分的多模型的映射
    执行算法调度模块（推理调度）
    这里执行返回pipline
    */
   return pipline_;
  }
  private:
    TaskMeta& task_meta_;
    ResourceList& resourcelist_;
    PipeLine& pipline_;
};

class TaskDeploy:public TaskBasic{//部署任务
  public:
    TaskDeploy();
    TaskDeploy(TaskMeta& task_meta,ResourceList &resourcelist,PipeLine& pipline):task_meta_(task_meta),resourcelist_(resourcelist),pipline_(pipline){}
    PipeLine& start(){
    /*
    do nothing now;
    切分模块（考虑手动切分还是非手动切分）
    保存单模型到多模型的切分
    执行算法调度模块（部署调度）
    这里执行返回pipline
    */
   return pipline_;
  }
  private:
    TaskMeta& task_meta_;
    ResourceList& resourcelist_;
    PipeLine& pipline_;
};
class TaskRecycle:public TaskBasic{//资源回收任务，可以和算法调度动态调整走同一个通道
  public:
    TaskRecycle();
    TaskRecycle(TaskMeta& task_meta,ResourceList &resourcelist,PipeLine& pipline):task_meta_(task_meta),resourcelist_(resourcelist),pipline_(pipline){}
    PipeLine& start(){
    /*
    do nothing now;
    根据resourcelist信息，以及pipline信息?新增pipline的信息存储
    来动态确定启停的pipline。
    这里执行返回pipline
    */
   return pipline_;
  }
  private:
    TaskMeta& task_meta_;
    ResourceList& resourcelist_;
    PipeLine& pipline_; 
};

class Schedule{
    public:
      Schedule(TaskMeta &taskmeta,ResourceList &resourcelist,PipeLine& pipline);//初始化
      void init();//外部调用开始进行任务创建
      TaskMeta execute();//任务开始,执行完成/异常返回taskmeta信息
    public:
      TaskMeta &taskmeta_;//任务管理表提供，一行数据信息
      ResourceList& resourcelist_;//资源列表，一是任务调度使用，二是上传至上面
    private:
      TaskBasic* task_basic_;//任务类指针
      PipeLine&  pipeline_;//这里是一个pipline的结果数据，在各种部署任务和推理任务以及回收任务执行完成后返回一个Pipline信息。这个信息需要传递到下一层的pipline 
};

