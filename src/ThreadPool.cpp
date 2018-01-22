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
    _locker.lock();
    task_queue.push(task);
    _locker.unlock();
}

void ThreadPool::run(){
    while(true){
        _locker.lock();
        if(task_queue.empty()){
            _locker.unlock();
            continue;
        }

        for(int i=0;i<_pool.size();i++){
            if(_pool[i]->isfree()){
                _pool[i]->add_task(task_queue.front());
                task_queue.pop();
                break;
            }
        }
        _locker.unlock();
    }
}
