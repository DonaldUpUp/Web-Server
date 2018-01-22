#include "RequestCreater.h"
//#include <glog/logging.h>

GetRequest RequestCreater::getHandle = GetRequest();

Request* RequestCreater::getRequestHandler(int fd, const std::string name, const std::string uri)
{
	if(name == "GET")
	{
		getHandle.init(fd, uri);
//		LOG(INFO)<<"tid "<<pthread_self()<<" get handle!"<<std::endl;
		return &getHandle;
	}
	return 0;
}
