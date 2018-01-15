#ifndef REQUEST_CREATER_H
#define REQUEST_CREATER_H

#include "GetRequest.h"

class RequestCreater
{
public:
	static Request* getRequestHandler(int fd, const std::string name, const std::string uri);

private:
	static GetRequest getHandle;
};

#endif
