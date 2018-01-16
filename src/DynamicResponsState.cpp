/*
 * DynamicResponsState.cpp
 *
 *  Created on: 2015年1月16日
 *      Author: augustus
 */

#include "DynamicResponseState.h"
#include "IoWriter.h"
#include <stdexcept>
#include <string>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
using std::string;
using std::vector;

namespace
{
pid_t _fork(void)
{
	pid_t pid;

	if ((pid = fork()) < 0)
		throw std::runtime_error("Fork error");
	return pid;
}

//重定向描述符，用fd1替换fd2
int _dup2(int fd1, int fd2)
{
	int rc;

	if ((rc = dup2(fd1, fd2)) < 0)
		throw std::runtime_error("Dup2 error");
	return rc;
}

//执行相关程序
void _execve(const char *filename, char * const argv[], char * const envp[])
{
	if (execve(filename, argv, envp) < 0)
		throw std::runtime_error("Execve error");
    std::cout<<"argv"<<argv<<std::endl;
}

pid_t _wait(int *status)
{
	pid_t pid;

	if ((pid = wait(status)) < 0)
		throw std::runtime_error("Wait error");
	return pid;
}

//获取cgi程序的参数
void separationString(string str,vector<string> &list,string fileName){
    size_t last=0;
    size_t equalPos=str.find("=",last);
    int length=str.length();
    size_t filePos=fileName.rfind("/");
    string file=fileName.substr(filePos+1,fileName.length()-filePos-1);
    list.push_back(file);
    while(equalPos!=std::string::npos){
        size_t andPos=str.find("&",last);
        if(andPos==std::string::npos){
            //list[i]=const_cast<char *>(str.substr(equalPos+1,length-equalPos-1).c_str());
            list.push_back(str.substr(equalPos+1,length-equalPos-1));
            break;
        }
        list.push_back(str.substr(equalPos+1,andPos-equalPos-1));
        last=andPos+1;
        equalPos=str.find("=",last);
    }
}
}

DynamicResponseState::DynamicResponseState(int fd, std::string fName, std::string args)
: ResponseState(fd, fName), cgiArgs(args)
{

}

const std::string DynamicResponseState::buildForbiddenMsg()
{
	return "Tiny couldn't run the CGI program";
}

void DynamicResponseState::doRespond()
{
	IoWriter writer(getFileDescriptor());
	writer.writeString(buildRespondHeaders());

	execveCgiProgram();
}

void DynamicResponseState::execveCgiProgram()
{
	char *emptylist[] = { NULL };
    vector<string> parm;
    separationString(cgiArgs,parm,getFileName());
    int i;
    for(i=0;i<parm.size();i++){
        emptylist[i]=const_cast<char *>(parm[i].c_str());
    }
    emptylist[i]=(char*)0;
	if (_fork() == 0)
	{
		setenv("QUERY_STRING", cgiArgs.c_str(), 1);
		_dup2(getFileDescriptor(), STDOUT_FILENO);
		_execve(getFileName().c_str(), emptylist, environ);
	}

	_wait(NULL);
	std::cout<<"Run "<<getFileName()<<" successed!"<<std::endl;
}



