#ifndef EPOLL_H_INCLUDED
#define EPOLL_H_INCLUDED

#define MAX_FD 1000
#define MAXSIZE 1024

#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>

class Epoll{
public:
    Epoll();
    ~Epoll(){
        close(epfd);
    }
    Epoll(int listenfd);

    int getBuffer(char* temp){
    	strcpy(temp,buf);
    	return 0;
    }

    int setBuffer(char* temp){
//    	if(strlen(temp)>buf){
//    		LOG(INFO)<<"Length is too big";
//    		return -1;
//    	}
    	strcpy(buf,temp);
    	return 1;
    }

    int setListenfd(int listenfd){
        this->listenfd=listenfd;
        EpollAdd(listenfd,EPOLLIN);
    }
    //创建epoll
    int EpollCreate(int size);
    //设置epoll事件
    int EpollCtl(int op,int fd,struct epoll_event * event);
    //等待epoll事件
    int EpollWait(struct epoll_event* events,int maxevents,int timeout);
    //添加epoll事件
    void EpollAdd(int fd,int state);
    //删除epoll事件
    void EpollDelete(int fd,int state);
    //修改epoll事件
    void EpollModify(int fd,int state);
    //执行epoll操作
    void EpollDo();
    //事件处理函数
    void EpollHandleEvents(struct epoll_event* events,int num);
    //处理接收到的连接
    void EpollHandleAccept();
    //处理读事件
    static void* EpollRead(void* fd);
    //处理写事件
    void EpollWrite(int fd);
private:
// 	typedef union epoll_data {
//     void    *ptr;
//     int      fd;
//     uint32_t u32;
//     uint64_t u64;
// } epoll_data_t;

// struct epoll_event {
//     uint32_t     events;    /* Epoll events */
//     epoll_data_t data;      /* User data variable */
// };
//
	int epfd;
	struct epoll_event ev;				//用于增删改时临时使用
	struct epoll_event events[MAX_FD];	//用于存放活跃的事件
	char buf[MAXSIZE];
	int listenfd;
};


#endif // EPOLL_H_INCLUDED
