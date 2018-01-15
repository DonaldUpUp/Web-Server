#include "NetConnection.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "errno.h"
#include <unistd.h>
#include <iostream>
using namespace std;

namespace
{
const int LISTENQ=1024;
void unix_error(char* msg)
{
    fprintf(stderr,"%s:%s\n",msg,strerror(errno));
    exit(0);
}
//使得套接字处于监听状态，等待连接
int open_listenfd(int port)
{
    int listenfd,optval=1;
    struct sockaddr_in serveraddr;
    //创建一个socket描述符
    if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0)
        return -1;
    //设置SO_REUSEADDR，使得端口可以重用
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval,sizeof(int)) < 0)
		return -1;

    bzero((char *)&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
    serveraddr.sin_port=htons((unsigned short)port);

    if(bind(listenfd,(sockaddr *)&serveraddr,sizeof(serveraddr))<0)
        return -1;
    //使listenfd准备去接受一个连接请求
    if(listen(listenfd,LISTENQ)<0)
        return -1;
    cout<<"Listenning..."<<endl;
    return listenfd;
}

int Open_listenfd(int port)
{
    int rc;
    if((rc=open_listenfd(port))<0)
        unix_error("Open_listened error");
    return rc;
}



int Accept(int s,struct sockaddr *addr,socklen_t *addrlen)
{
    int rc;
    struct sockaddr_in guest;
    char guest_ip[20];
    if((rc=accept(s,addr,addrlen))<0)
        unix_error("Accept error");
    socklen_t guest_len=sizeof(guest);
    getpeername(rc,(struct sockaddr *)&guest,&guest_len);
    inet_ntop(AF_INET,&guest.sin_addr,guest_ip,sizeof(guest_ip));
    cout<<"Accept:new client "<<guest_ip<<":"<<ntohs(guest.sin_port)<<" connect success!\n"<<endl;
    return rc;
}

void Close(int fd)
{
    int rc;
    if((rc=close(fd))<0)
        unix_error("Close error");
    }
}
NetConnection::NetConnection():lisenfd(-1),connfd(-1)
{
    //ctor
}

NetConnection::~NetConnection()
{
    //dtor
}

void NetConnection::lisen(int port){
    lisenfd=Open_listenfd(port);
}

int NetConnection::accept(){
    int clientlen;
    struct sockaddr_in clientaddr;
    clientlen=sizeof(clientaddr);

    connfd=Accept(lisenfd,(sockaddr *)&clientaddr,reinterpret_cast<socklen_t*>(&clientlen));
    return connfd;
}

void NetConnection::close(){
    Close(connfd);
    cout<<"Connection Closed!"<<endl;
}

