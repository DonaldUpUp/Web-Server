#ifndef REQUEST_H
#define REQUEST_H

#include <string>

//Request是一个抽象类，每一个子类都需要实现doExecute方法才能实例化
class Request
{
    public:
        Request();
        virtual ~Request();
        void init(int fd,std::string uri);
        void execute();
    protected:
        int getFileDescriptor() const;
        const std::string& getUri() const;
    private:
        virtual void doExecute()=0;     //虚函数

        int fileDescriptor;
        std::string uri;
};

#endif // REQUEST_H
