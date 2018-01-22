/*
 * StaticResponseState.cpp
 *
 *  Created on: 2015年1月16日
 *      Author: augustus
 */

#include "StaticResponseState.h"
#include "IoWriter.h"
//#include <glog/logging.h>

StaticResponseState::StaticResponseState(int fd, std::string fName) : ResponseState(fd, fName)
{

}

const std::string StaticResponseState::buildForbiddenMsg()
{
	return "Tiny couldn't read the file";
}

void StaticResponseState::doRespond()
{
	IoWriter writer(getFileDescriptor());
	writer.writeString(buildRespondHeaders());
	writer.writeFile(getFileName(), getStat().st_size);
//	LOG(INFO)<<"tid="<<pthread_self()<<" send "<<getFileName()<<" successed!"<<std::endl;
}

const std::string StaticResponseState::buildRespondHeaders()
{
	return ResponseState::buildRespondHeaders()
				+ std::string("Content-length: ")
		        + sizeTypeToStr(getStat().st_size)
				+ "\r\n"
				+ std::string("Content-type: ")
		        + getFiletype()
				+ "\r\n\r\n";
}

const std::string StaticResponseState::getFiletype()
{
	std::string fileName = getFileName();
	if (fileName.find(".html")!=std::string::npos)
		return std::string("text/html");
	else if (fileName.find(".gif")!=std::string::npos)
		return std::string("image/gif");
	else if (fileName.find(".jpg")!=std::string::npos)
		return std::string("image/jpeg");
    else if(fileName.find(".css")!=std::string::npos)
        return std::string("text/css");
	else
		return std::string("text/plain");
}


