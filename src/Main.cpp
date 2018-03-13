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
#include <memory>
#include <mutex>
#include <condition_variable>
using namespace std;

//#define _THREAD_POOL

static const int kItemRepositorySize=10;


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
    return 0;
}
}

////单生产者单消费者
//struct ItemRepository {
//    int item_buffer[kItemRepositorySize]; // 产品缓冲区, 配合 read_position 和 write_position 模型环形队列.
//    size_t read_position; // 消费者读取产品位置.
//    size_t write_position; // 生产者写入产品位置.
//    std::mutex mtx; // 互斥量,保护产品缓冲区
//    std::condition_variable repo_not_full; // 条件变量, 指示产品缓冲区不为满.
//    std::condition_variable repo_not_empty; // 条件变量, 指示产品缓冲区不为空.
//} gItemRepository; // 产品库全局变量, 生产者和消费者操作该变量.
//
//typedef struct ItemRepository ItemRepository;
//
//
//void ProduceItem(ItemRepository *ir, int item)
//{
//    std::unique_lock<std::mutex> lock(ir->mtx);
//    while(((ir->write_position + 1) % kItemRepositorySize)
//        == ir->read_position) { // item buffer is full, just wait here.
//        std::cout << "Producer is waiting for an empty slot...\n";
//        (ir->repo_not_full).wait(lock); // 生产者等待"产品库缓冲区不为满"这一条件发生.
//    }
//
//    (ir->item_buffer)[ir->write_position] = item; // 写入产品.
//    (ir->write_position)++; // 写入位置后移.
//
//    if (ir->write_position == kItemRepositorySize) // 写入位置若是在队列最后则重新设置为初始位置.
//        ir->write_position = 0;
//
//    (ir->repo_not_empty).notify_all(); // 通知消费者产品库不为空.
//    lock.unlock(); // 解锁.
//}
//
//int ConsumeItem(ItemRepository *ir)
//{
//    int data;
//    std::unique_lock<std::mutex> lock(ir->mtx);
//    // item buffer is empty, just wait here.
//    while(ir->write_position == ir->read_position) {
//        std::cout << "Consumer is waiting for items...\n";
//        (ir->repo_not_empty).wait(lock); // 消费者等待"产品库缓冲区不为空"这一条件发生.
//    }
//
//    data = (ir->item_buffer)[ir->read_position]; // 读取某一产品
//    (ir->read_position)++; // 读取位置后移
//
//    if (ir->read_position >= kItemRepositorySize) // 读取位置若移到最后，则重新置位.
//        ir->read_position = 0;
//
//    (ir->repo_not_full).notify_all(); // 通知消费者产品库不为满.
//    lock.unlock(); // 解锁.
//
//    return data; // 返回产品.
//}
//
//
//void ProducerTask() // 生产者任务
//{
//    NetConnection connection;
//    connection.lisen(8080);
//    int connfd;
//    while(1){
//        connfd=connection.accept();
//        ProduceItem(&gItemRepository,connfd);
//    }
//}
//
//void ConsumerTask() // 消费者任务
//{
//    while(1){
//        int item=ConsumeItem(&gItemRepository);
//        RequestManager(item).run();
//        close(item);
//    }
//}
//
//void InitItemRepository(ItemRepository *ir)
//{
//    ir->write_position = 0; // 初始化产品写入位置.
//    ir->read_position = 0; // 初始化产品读取位置.
//}

//单生产者多消费者
struct ItemRepository {
    int item_buffer[kItemRepositorySize];
    size_t read_position;
    size_t write_position;
    size_t item_counter;
    std::mutex mtx;
    std::mutex item_counter_mtx;
    std::condition_variable repo_not_full;
    std::condition_variable repo_not_empty;
} gItemRepository;

typedef struct ItemRepository ItemRepository;


void ProduceItem(ItemRepository *ir, int item)
{
    std::unique_lock<std::mutex> lock(ir->mtx);
    while(((ir->write_position + 1) % kItemRepositorySize)
        == ir->read_position) { // item buffer is full, just wait here.
        std::cout << "Producer is waiting for an empty slot...\n";
        (ir->repo_not_full).wait(lock);
    }

    (ir->item_buffer)[ir->write_position] = item;
    (ir->write_position)++;

    if (ir->write_position == kItemRepositorySize)
        ir->write_position = 0;

    (ir->repo_not_empty).notify_all();
    lock.unlock();
}

int ConsumeItem(ItemRepository *ir)
{
    int data;
    std::unique_lock<std::mutex> lock(ir->mtx);
    // item buffer is empty, just wait here.
    while(ir->write_position == ir->read_position) {
        std::cout << "Consumer is waiting for items...\n";
        (ir->repo_not_empty).wait(lock);
    }

    data = (ir->item_buffer)[ir->read_position];
    (ir->read_position)++;

    if (ir->read_position >= kItemRepositorySize)
        ir->read_position = 0;

    (ir->repo_not_full).notify_all();
    lock.unlock();

    return data;
}


void ProducerTask()
{
    NetConnection connection;
    connection.lisen(8080);
    int connfd;
    while(1){
        connfd=connection.accept();
        ProduceItem(&gItemRepository,connfd);
    }
}

void ConsumerTask()
{
    while(1) {
        //sleep(1);
        std::unique_lock<std::mutex> lock(gItemRepository.item_counter_mtx);
        int item = ConsumeItem(&gItemRepository);
        RequestManager(item).run();
        lock.unlock();
        close(item);
    }
}

void InitItemRepository(ItemRepository *ir)
{
    ir->write_position = 0;
    ir->read_position = 0;
    ir->item_counter = 0;
}





int main(int argc, char **argv)
{
//    NetConnection connection;
//    //10个线程的线程池
//    CThreadPool threadPool(10);
//
//    connection.lisen(getStartPort(argc, argv));

//    google::InitGoogleLogging(argv[0]);
//    FLAGS_log_dir="./myLog";
//    FLAGS_logtostderr = false;


    //单线程
//    #ifndef _THREAD_POOL
//    while (1)
//    {
//        int connfd = connection.accept();
//        RequestManager(connfd).run();
//        connection.close();
//    }
//
//
//    #else
//
//
//    //线程池
////    ThreadPool tp(10);
////    while(1)
////    {
////        CMyTask taskObj;
////        int connfd=connection.accept();
////        LOG(INFO)<<"Connect "<<connfd<<endl;
////        taskObj.SetData(&connfd);
////        threadPool.AddTask(&taskObj);
////    }
//
////    while(1)
////    {
////        cout<<"there are still "<<threadPool.getTaskSize()<<" tasks need to handle"<<endl;
////        if(threadPool.getTaskSize()==0)
////        {
////            if(threadPool.StopAll()==-1)
////            {
////                cout<<"Now I will exit from main"<<endl;
////                exit(4);
////            }
////        }
////        sleep(2);
////    }
//    //线程池２
//    std::mutex _locker;
//    while(1){
//        int connfd=connection.accept();
//        Task t1(connfd);
//        tp.add_task(&t1);
//    }
//
//    #endif // _THREAD_POOL

//    //生产者消费者　
//    InitItemRepository(&gItemRepository);
//    thread producer(ProducerTask);
//    thread consumer(ConsumerTask);
//    producer.join();
//    consumer.join();

    //单生产者多消费者
    InitItemRepository(&gItemRepository);
    std::thread producer(ProducerTask);
    std::thread consumer1(ConsumerTask);
    std::thread consumer2(ConsumerTask);
    std::thread consumer3(ConsumerTask);
    std::thread consumer4(ConsumerTask);

    producer.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();
    consumer4.join();

    return 1;
}
