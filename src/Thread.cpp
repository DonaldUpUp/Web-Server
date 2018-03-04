#include "Thread.h"

Thread::Thread():_isfree(true),_task(nullptr){
    _thread=std::thread(&Thread::run,this);
    _thread.detach();
}

bool Thread::isfree(){
    return _isfree;
}

void Thread::add_task(Task* task){
    if(_isfree){
        _task=task;
        _isfree=false;
    }
}

void Thread::run(){
    while(true){
        if(_task){
            _isfree=false;
            _task->run();
            _isfree=true;
            _task=nullptr;
        }
    }
}
