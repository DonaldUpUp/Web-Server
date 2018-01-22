#ifndef IOREADER_H
#define IOREADER_H

#include <string>
#include <vector>
//封装底层IO
class IoReader
{
    public:
        IoReader();
        virtual ~IoReader();
        IoReader(int fd);
        //读取http请求的报头，读取一行，并使用“ ”分隔成多个字符串后返回
        void getLineSplitedByBlank(std::vector<std::string>& buf);
    protected:

    private:
};

#endif // IOREADER_H
