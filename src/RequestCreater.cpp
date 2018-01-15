#include "RequestCreater.h"

GetRequest RequestCreater::getHandle = GetRequest();

Request* RequestCreater::getRequestHandler(int fd, const std::string name, const std::string uri)
{
	if(name == "GET")
	{
		getHandle.init(fd, uri);
		return &getHandle;
	}

	return 0;
}
