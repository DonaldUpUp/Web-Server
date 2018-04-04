#include "IoWriter.h"
#include "glog/logging.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <cerrno>
#include <iostream>

namespace{
void unix_error(char *msg){
    fprintf(stderr,"%s: %s\n",msg,strerror(errno));
    exit(2);
}

ssize_t rio_writen(int fd,void *usrbuf,size_t n){
    size_t nleft=n;
    size_t nwritten;
    char *bufp=reinterpret_cast<char*>(usrbuf);
    while(nleft>0){
        if((nwritten=write(fd,bufp,nleft))<=0){
            if(nwritten==-1){
                ep.EpollDelete(fd,EPOLLIN);
                LOG(INFO)<<fd;
            }
            if(errno==EINTR)
                nwritten=0;
            else
                return -1;
        }
        nleft-=nwritten;
        bufp+=nwritten;
    }
    return n;
}

void Rio_writen(int fd,void *usrbuf,size_t n){
    if(rio_writen(fd,usrbuf,n)!=n)
        unix_error("Rio_writen error");

}

int Open(const char *pathname,int flags,mode_t mode){
    int rc;
    if((rc=open(pathname,flags,mode))<0)
        unix_error("Open error");
    return rc;
}

void Close(int fd){
    int rc;
    if((rc=close(fd))<0)
        unix_error("Close error");
}

//将资源文件映射到进程的地址空间，返回被映射区的指针
void* Mmap(void *addr,size_t len,int prot,int flags,int fd,off_t offset){
    void *ptr;
    if((ptr=mmap(addr,len,prot,flags,fd,offset))==((void *)-1))
        unix_error("mmap error");

    return (ptr);
}

//取消参数start 所指的映射内存起始地址
void Munmap(void *start,size_t length){
    if(munmap(start,length)<0)
        unix_error("munmap error");
}

}

IoWriter::IoWriter()
{
    //ctor
}

IoWriter::IoWriter(int fd):fileDescriptor(fd){
}

void IoWriter::writeString(const std::string& str){
    Rio_writen(fileDescriptor,const_cast<char*>(str.c_str()),str.length());
}

IoWriter::~IoWriter()
{
    //dtor
}

void IoWriter::writeFile(const std::string& fileName,int filesSize){
    int srcfd;
    char *srcp;
    srcfd=Open(const_cast<char*>(fileName.c_str()),O_RDONLY,0);
    srcp=reinterpret_cast<char*>(Mmap(0,filesSize,PROT_READ,MAP_PRIVATE,srcfd,0));
    Close(srcfd);
    Rio_writen(fileDescriptor,srcp,filesSize);
    Munmap(srcp,filesSize);
//    Rio_writen(fileDescriptor,(void *)srcfd,filesSize);
    Close(fileDescriptor);

    std::cout<<fileName<<" send success!"<<std::endl;
    //pthread_join(pthread_self(),NULL);
}

