/*
 * Response.cpp
 *
 *  Created on: 2015年1月11日
 *      Author: augustus
 */

#include "Response.h"
#include "TinyError.h"
#include "StaticResponseState.h"
#include "DynamicResponseState.h"
//#include <glog/logging.h>
#include <stdexcept>

Response::Response(int fd, std::string name, std::string cgiargs, bool isStc)
{
	if(isStc)
		state = new StaticResponseState(fd, name);
	else
		state = new DynamicResponseState(fd, name, cgiargs);
}

Response::~Response()
{
	if(state)
		delete state;
}

void Response::respond()
{
	try
	{
		preRespond();
		doRespond();
	}
	catch (TinyError& err)
	{
//        LOG(INFO)<<"tid "<<pthread_self()<<" send error "<<err.ErrNum<<std::endl;
		return state->respondError(err.ErrNum, err.ShortMsg, err.LongMsg);
	}
	catch (std::runtime_error& err)
	{
		return state->respondError("500", "Server internal error", err.what());
	}
}

void Response::preRespond()
{
	state->preRespond();
}

void Response::doRespond()
{
	state->respond();
}



