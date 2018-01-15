#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <string>

class Request;
//等待HTTP请求，并解析内容
//其中Request 就是具体方法的基类，其子类可以是GET，POST等等。
class RequestManager
{
    public:
        RequestManager();
        RequestManager(int connfd);
        virtual ~RequestManager();
        void run();
    private:
        Request* getRequestHandle();    //根据不同的请求类型，获取不同的请求句柄
        int fileDescriptor;             //关联的文件描述符
        Request* request;               //获取的请求句柄
};

#endif // REQUESTMANAGER_H
