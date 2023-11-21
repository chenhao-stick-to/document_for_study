#include "schedule.h"
Schedule::Schedule(TaskMeta &taskmeta,ResourceList &resourcelist,PipeLine& pipline):taskmeta_(taskmeta),resourcelist_(resourcelist),
pipeline_(pipline){
}

void Schedule::init(){
switch(taskmeta_.tasktype){
case TaskType::INFERENCE:{
    task_basic_=new TaskInference(taskmeta_,resourcelist_,pipeline_);
    break;
}
case TaskType::DEPLOY:{
    task_basic_=new TaskDeploy(taskmeta_,resourcelist_,pipeline_);
    break;
}
case TaskType::RECYCLE:{
    task_basic_=new TaskRecycle(taskmeta_,resourcelist_,pipeline_);
    break;
}
 default:{
    task_basic_=new TaskBasic(taskmeta_,resourcelist_);
    break;
}
}
}  
TaskMeta Schedule::execute(){//任务开始,执行完成/异常返回taskmeta信息
resourcelist_=task_basic_->GetClusterResourceInfo();//获取资源信息
task_basic_->start();//任务开始执行
}
