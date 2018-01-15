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
        Request* getRequestHandle();
        int fileDescriptor;         //关联的文件描述符
        Request* request;
};

#endif // REQUESTMANAGER_H
