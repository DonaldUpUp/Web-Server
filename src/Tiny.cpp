#include "NetConnection.h"
#include "RequestManager.h"
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;

namespace
{
int getPortFromCommandLine(char **argv)
{
    return atoi(argv[1]);
}

int getDefalutPort()
{
    return 8080;
}

int getStartPort(int argc, char **argv)
{
    if (argc == 2)
        return getPortFromCommandLine(argv);
    else
        cout<<"Listen 8080 port."<<endl;
        return getDefalutPort();
}

void* threadRun(void *fd){
    int connfd=*((int*)fd);
    pthread_t tid=pthread_self();
    RequestManager(connfd).run();
    pthread_exit(NULL);
    close(connfd);
    return 0;
}
}

int main(int argc, char **argv)
{
//        getchar();
    NetConnection connection;

    connection.lisen(getStartPort(argc, argv));
//    while (1)
//    {
//        int connfd = connection.accept();
//        RequestManager(connfd).run();
//        connection.close();
//    }
    while (1)
    {
        pthread_t tid;
        int connfd = connection.accept();
        int ret=pthread_create(&tid,NULL,threadRun,(void *)&(connfd));
        if(ret!=0){
cout<<"pthread_create error:error_code="<<ret<<endl;
        }
    }
    //pthread_exit(NULL);


//    getchar();

//    cin.get();
//    system("pause");
}
