#include "RequestManager.h"
#include "Request.h"
#include "RequestCreater.h"
#include "IoReader.h"
//#include <glog/logging.h>
#include <iostream>

namespace {
//解析客户端的请求数据
class Parser {
public:
    Parser(int connfd) {
//        LOG(INFO)<<"tid "<<pthread_self()<<" enter Parser"<<std::endl;
        parseRequestHeaders(connfd);
    }

    const std::string getMethodName() {
        return method;
    }

    const std::string getUri() {
        return uri;
    }
private:
    //解析请求的数据，用到了IOReader类
    void parseRequestHeaders(int fd) {
//        LOG(INFO)<<"tid "<<pthread_self()<<" enter parseRequestHandle"<<std::endl;
        IoReader reader(fd);
        std::vector<std::string> header;
        reader.getLineSplitedByBlank(header);
//        LOG(INFO)<<"tid "<<pthread_self()<<" parse finish"<<std::endl;
        //置为空
        method=header[0];
        uri=header[1];
        version=header[2];
//        LOG(INFO)<<"tid="<<pthread_self()<<" message:method="<<method<<" uri="<<uri<<" version="<<version<<std::endl;
    }

    std::string method;
    std::string uri;
    std::string version;    //协议版本
};
}


RequestManager::RequestManager() {
    //ctor
}

RequestManager::RequestManager(int connfd):fileDescriptor(connfd),request(0){

}

RequestManager::~RequestManager() {
    //dtor
}

void RequestManager::run(){
    if(getRequestHandle())
        request->execute();
}

Request* RequestManager::getRequestHandle(){
    Parser parser(fileDescriptor);
//    std::cout<<parser.getMethodName()<<std::endl;
//    std::cout<<parser.getUri()<<std::endl;
    //使用工厂类（RequestCreater），创造不同的方法实例
    return request=RequestCreater::getRequestHandler(fileDescriptor,parser.getMethodName(),parser.getUri());
}
