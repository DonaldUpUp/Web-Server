#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

#include <iostream>
#include <mutex>

class Task
{
private:
    int no;
public:
    Task(int n);
    std::mutex _locker;
    virtual void run();
    virtual ~Task();
};


#endif // TASK_H_INCLUDED
