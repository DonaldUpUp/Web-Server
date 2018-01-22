#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include <vector>
#include <string>
#include <pthread.h>

#include "Task.h"
using std::vector;

class CTask;

//线程池类
class CThreadPool
{
private:
    static vector<CTask*> m_TaskList;       //任务列表
    static bool shutdown;                   //线程退出标志
    int m_iThreadNum;                       //线程池中启动的线程数
    pthread_t *pthread_id;                  //存放线程id

    static pthread_mutex_t m_pthreadMutex;  //线程同步锁
    static pthread_cond_t m_pthreadCond;    //线程同步的条件变量

protected:
    static void* ThreadFunc(void * threadData); //新线程的回调函数
    static int MoveToIdle(pthread_t tid) {}   //线程执行结束，放入到空闲线程中
    static int MoveToBusy(pthread_t tid) {} //把线程放入忙碌队列中去
    int Create();                          //创建线程池中的线程

public:
    CThreadPool(int threadNum=10);          //构造函数
    int AddTask(CTask *task);               //把任务添加到任务队列中
    int StopAll();                          //使线程池中的线程全部退出
    int getTaskSize();                      //获取当前队列中的任务数
};



#endif // THREAD_H_INCLUDED
