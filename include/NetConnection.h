#ifndef NETCONNECTION_H
#define NETCONNECTION_H


class NetConnection
{
    public:
        NetConnection();
        virtual ~NetConnection();
        void lisen(int port);
        int accept();
        void close();
    protected:

    private:
        int lisenfd;        //打开socket时系统函数返回的描述符
        int connfd;         //调用accept系统函数时返回的文件描述符，也就是实际上的数据通道
};

#endif // NETCONNECTION_H
