#include "ThreadPool.h"

ThreadPool::ThreadPool(int n){
    while(n--){
        Thread *t = new Thread();
        _pool.push_back(t);
    }
    //单独开一个线程来给线程分配任务
    std::thread main_thread(&ThreadPool::run,this);
    //将此线程转入后台
    main_thread.detach();
}

ThreadPool::~ThreadPool(){
    for(int i=0;i<_pool.size();i++){
        delete _pool[i];
    }
}

void ThreadPool::add_task(Task *task){
    task_queue.push(task);
}

//void ThreadPool::add_task(std::shared_ptr<Task> task){
//    task_queue.push(&(*task));
//}

void ThreadPool::run(){
    while(true){
        if(task_queue.empty()){
            continue;
        }
        for(int i=0;i<_pool.size();i++){
            if(_pool[i]->isfree()){
                _pool[i]->add_task(task_queue.front());
                task_queue.pop();
                break;
            }
        }
    }
}
