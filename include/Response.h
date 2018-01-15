/*
 * Response.h
 *
 *  Created on: 2015Äê1ÔÂ11ÈÕ
 *      Author: augustus
 */

#ifndef RESPONSE_H_
#define RESPONSE_H_

#include <string>

class ResponseState;

class Response
{
public:
	Response(int fd, std::string name, std::string cgiargs, bool isStc);
	~Response();
	void respond();

private:
	void preRespond();
	void doRespond();
	Response(const Response&);
	Response& operator= (const Response&);
private:
	ResponseState* state;
};

#endif /* RESPONSE_H_ */
