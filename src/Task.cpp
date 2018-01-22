#include "Task.h"
#include "RequestManager.h"
#include <unistd.h>

Task::Task(int n){
    no=n;
}
Task::~Task(){
}

void Task::run(){
    RequestManager(no).run();
    close(no);
}
