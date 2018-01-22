#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED
#include <thread>
#include <mutex>
#include "Task.h"

class Thread
{
private:
    std::thread _thread;
    bool _isfree;
    Task *_task;
    std::mutex _locker;
public:
    Thread();
    bool isfree();
    void add_task(Task* task);
    void run();
};


#endif // THREAD_H_INCLUDED
