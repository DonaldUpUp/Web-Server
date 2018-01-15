#include "RequestManager.h"
#include "Request.h"
#include "RequestCreater.h"
#include "IoReader.h"

namespace {
//解析客户端的请求数据
class Parser {
public:
    Parser(int connfd) {
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
        IoReader reader(fd);
        std::vector<std::string> header;
        reader.getLineSplitedByBlank(header);
        //置为空
        method=header[0];
        uri=header[1];
        version=header[2];
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
    //使用工厂类（RequestCreater），创造不同的方法实例
    return request=RequestCreater::getRequestHandler(fileDescriptor,parser.getMethodName(),parser.getUri());
}
