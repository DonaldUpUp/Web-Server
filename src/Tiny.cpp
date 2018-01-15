#include "NetConnection.h"
#include "RequestManager.h"
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
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
}

int main(int argc, char **argv)
{
//        getchar();
    NetConnection connection;

    connection.lisen(getStartPort(argc, argv));
    while (1)
    {
        int connfd = connection.accept();
        RequestManager(connfd).run();
        connection.close();
    }


//    getchar();

//    cin.get();
//    system("pause");
}
