#ifndef GETREQUEST_H
#define GETREQUEST_H

#include "Request.h"
#include <string>
//处理GET方法请求，解析出请求的URI，并将其传给Response类的对象进一步处理
class GetRequest:public Request
{
    public:
        GetRequest();
        virtual ~GetRequest();


    protected:

    private:
        virtual void doExecute();
        bool parseUri(std::string& filename,std::string& cgiargs);
        bool parseStaticContentUri(std::string& filename);
        //处理动态请求的uri和参数的
        bool parseDynamicContentUri(std::string& filename,std::string& cgiargs);
        void assignCigArgs(std::string& cgiargs);
        void doAssignCigArgs(std::string::size_type pos,std::string& cgiargs);
};

#endif // GETREQUEST_H
