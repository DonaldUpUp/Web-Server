#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include "Epoll.h"
#include "glog/logging.h"
#include "RequestManager.h"
using namespace std;

Epoll::Epoll(){
}
//构造函数把监听描述符加入事件
Epoll::Epoll(int listenfd){
	memset(buf,0,MAXSIZE);
	this->listenfd=listenfd;
 	//EpollAdd(listenfd,EPOLLIN);
}



int Epoll::EpollCreate(int size){
	epfd=epoll_create(MAX_FD);
	return epfd;
}

int Epoll::EpollCtl(int op,int fd,struct epoll_event *event){
	int ret=epoll_ctl(epfd,op,fd,event);
	if(ret==-1){
		LOG(ERROR)<<"Epoll Set Failed!";
		if(errno==EBADF)
			LOG(ERROR)<<"epfd or fd is an invalid file descriptor.";
		else if(errno==EEXIST)
			LOG(ERROR)<<"op is EPOLL_CTL_ADD,meanwhile fd has been registered in epoll.";
        else if(errno==EINVAL)
            LOG(ERROR)<<"epfd is not a epoll decriptor";
        else if(errno==ENOENT)
            LOG(ERROR)<<"fd is not be rigister in epoll";
        else if(errno==ENOMEM )
            LOG(ERROR)<<"memory is little";
		else{
			LOG(ERROR)<<"epoll error.";
		}
		return -1;
	}
	return ret;
}

int Epoll::EpollWait(struct epoll_event *events,int maxevents,int timeout){
	int ret=epoll_wait(epfd,events,maxevents,timeout);
	if(ret==-1){
		LOG(ERROR)<<"Epoll Wait Failed!";
		return -1;
	}
	return ret;
}

void Epoll::EpollAdd(int fd,int state){
	ev.events=state;
	ev.data.fd=fd;
	EpollCtl(EPOLL_CTL_ADD,fd,&ev);
}

void Epoll::EpollDelete(int fd,int state){
	ev.events=state;
	ev.data.fd=fd;
	EpollCtl(EPOLL_CTL_DEL,fd,&ev);
}

void Epoll::EpollModify(int fd,int state){
	ev.events=state;
	ev.data.fd=fd;
	EpollCtl(EPOLL_CTL_MOD,fd,&ev);
}

void Epoll::EpollDo(){
    int ret;
    while(1){
        ret=EpollWait(events,MAX_FD,-1);
        EpollHandleEvents(events,ret);
    }
}

void Epoll::EpollHandleEvents(struct epoll_event* events,int num){
	int fd;
	vector<pthread_t> tids;
	pthread_t t;
	int ret;
	for(int i=0;i<num;i++){
        fd=events[i].data.fd;
		if((fd==listenfd)&&events[i].events&EPOLLIN)
			EpollHandleAccept();
		else if(events[i].events&EPOLLIN){
            ret=pthread_create(&t,NULL,EpollRead,(void*)&fd);
            tids.push_back(t);
            if(ret!=0){
                LOG(WARNING)<<"Can't create thread:"<<ret;
            }
//            thread t(EpollRead,this,fd);
//            tids.push_back(t);
		}
	}
	for(int i=0;i<tids.size();i++){
        pthread_join(tids[i],NULL);
	}
	tids.clear();
}


void Epoll::EpollHandleAccept(){
    //struct sockaddr_in guest;
    //socklen_t guest_len;
    //int clifd=accept(listenfd,(struct sockaddr*)&guest,&guest_len);
    int clifd;
    int clientlen;
    struct sockaddr_in guest;
    clientlen=sizeof(guest);

    clifd=accept(listenfd,(sockaddr *)&guest,reinterpret_cast<socklen_t*>(&clientlen));

    if(clifd==-1){
        LOG(ERROR)<<"accept error"<<errno;

    }else{
        char guest_ip[20];
        socklen_t guest_len=sizeof(guest);
        getpeername(clifd,(struct sockaddr *)&guest,&guest_len);
        inet_ntop(AF_INET,&guest.sin_addr,guest_ip,sizeof(guest_ip));
        cout<<"Accept:new client "<<guest_ip<<":"<<ntohs(guest.sin_port)<<" connect success!\n"<<endl;
        EpollAdd(clifd,EPOLLIN);
    }
}

void* Epoll::EpollRead(void* fd){
//	memset(buf,0,MAXSIZE);
//	int nread=read(fd,buf,MAXSIZE);
//	if(nread==-1){
//		LOG(INFO)<<"read error";
//		close(fd);
//		EpollDelete(fd,EPOLLIN);
//	}else{
//		EpollModify(fd,EPOLLOUT);
//	}
    RequestManager(*(int *)fd).run();

}

//void Epoll::EpollWrite(int fd){
//	int nwrite=write(fd,buf,strlen(buf));
//	if(nwrite==-1){
//		LOG(INFO)<<"write error";
//		close(fd);
//		EpollDelete(fd,EPOLLOUT);
//	}else
//		EpollModify(fd,EPOLLIN);
//	memset(buf,0,MAXSIZE);
//}
