#include "Task.h"
#include "RequestManager.h"
#include <unistd.h>
#include <mutex>

Task::Task(int n){
    no=n;
}
Task::~Task(){
}

void Task::run(){
    _locker.lock();
    RequestManager(no).run();
    close(no);
    _locker.unlock();
}
