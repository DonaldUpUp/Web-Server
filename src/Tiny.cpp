#include "NetConnection.h"
#include "RequestManager.h"
#include "Thread.h"
#include "Task.h"
#include "ThreadPool.h"
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;

#define _THREAD_POOL

//pthread_mutex_t runMutex=PTHREAD_MUTEX_INITIALIZER;
//
//class CMyTask:public CTask
//{
//public:
//    CMyTask() {}
//    static pthread_mutex_t runMutex;
//    inline int Run()
//    {
//        //pthread_mutex_lock(&runMutex);
//        //LOG(INFO)<<"tid="<<pthread_self()<<" get runMutex"<<endl;
//        RequestManager(*(int*)m_ptrData).run();
//        close(*(int*)m_ptrData);
//
//        //LOG(INFO)<<"tid="<<pthread_self()<<" release runMutex"<<endl;
//        //pthread_mutex_unlock(&runMutex);
//
//    }
//};

//pthread_mutex_t CMyTask::runMutex=PTHREAD_MUTEX_INITIALIZER;

namespace
{
int getPortFromCommandLine(char **argv)
{
    return atoi(argv[1]);
}

int getDefalutPort()
{
    return 8080;
}

int getStartPort(int argc, char **argv)
{
    if (argc == 2)
        return getPortFromCommandLine(argv);
    else
        cout<<"Listen 8080 port."<<endl;
    return getDefalutPort();
}

void* threadRun(void *fd)
{
    int connfd=*((int*)fd);
    //pthread_t tid=pthread_self();
    RequestManager(connfd).run();
    close(connfd);
    //close(connfd);
    return 0;
}
}

int main(int argc, char **argv)
{
    NetConnection connection;
    //10个线程的线程池
    //CThreadPool threadPool(10);

    connection.lisen(getStartPort(argc, argv));

//    google::InitGoogleLogging(argv[0]);
//    FLAGS_log_dir="./myLog";
//    FLAGS_logtostderr = false;

    ThreadPool tp(10);
    //单线程
    #ifndef _THREAD_POOL
    while (1)
    {
        int connfd = connection.accept();
        RequestManager(connfd).run();
        connection.close();
    }
    #else
    //线程池
//    while(1)
//    {
//        CMyTask taskObj;
//        int connfd=connection.accept();
//        LOG(INFO)<<"Connect "<<connfd<<endl;
//        taskObj.SetData(&connfd);
//        threadPool.AddTask(&taskObj);
//    }

//    while(1)
//    {
//        cout<<"there are still "<<threadPool.getTaskSize()<<" tasks need to handle"<<endl;
//        if(threadPool.getTaskSize()==0)
//        {
//            if(threadPool.StopAll()==-1)
//            {
//                cout<<"Now I will exit from main"<<endl;
//                exit(4);
//            }
//        }
//        sleep(2);
//    }
    //线程池２
    while(1){
        int connfd=connection.accept();
        Task t1(connfd);
        tp.add_task(&t1);
    }

    #endif // _THREAD_POOL
    return 1;
}
