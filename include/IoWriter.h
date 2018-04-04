#ifndef IOWRITER_H
#define IOWRITER_H

#include <string>
#include "Epoll.h"
//这个类与IoReader对应，是封装底层IO写操作的，实际上就是向客户端发送数据。当解析出客户端想要访问的uri后，这里就会将相应的文件发送回去，这后浏览器解析这个文件，我们就能看到网页了
class IoWriter
{
    public:
        IoWriter();
        virtual ~IoWriter();
        IoWriter(int fd);
        void writeString(const std::string& str);   //主要是用于发送响应报头
        void writeFile(const std::string& filename,int filesSize);  //把客户端想要的文件返回
    protected:

    private:
        int fileDescriptor;

};
extern Epoll ep;
#endif // IOWRITER_H
