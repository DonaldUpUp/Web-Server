#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

#include <iostream>

class Task
{
private:
    int no;
public:
    Task(int n);
    virtual void run();
    virtual ~Task();
};


#endif // TASK_H_INCLUDED
