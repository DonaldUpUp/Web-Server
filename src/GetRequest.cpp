#include "GetRequest.h"
#include "Response.h"
GetRequest::GetRequest()
{
    //ctor
}

GetRequest::~GetRequest()
{
    //dtor
}

void GetRequest::doExecute(){
    std::string filename,cgiargs;
    bool isStatic=parseUri(filename,cgiargs);
    Response(getFileDescriptor(),filename,cgiargs,isStatic).respond();
}

bool GetRequest::parseUri(std::string& filename,std::string& cgiargs){
    if(getUri().find("cgi-bin")==std::string::npos)
        return parseStaticContentUri(filename);
    else
        return parseDynamicContentUri(filename,cgiargs);
}

bool GetRequest::parseStaticContentUri(std::string& filename){
    std::string uri=getUri();
    filename="test-files"+uri;
    if(uri[uri.length()-1]=='/')
        filename+="index.html";
    return true;
}

bool GetRequest::parseDynamicContentUri(std::string& filename,std::string& cgiargs){
    assignCigArgs(cgiargs);
    filename="."+getUri();
    return false;
}

void GetRequest::assignCigArgs(std::string& cgiargs){
    std::string uri=getUri();
    std::string::size_type pos=uri.find_first_of("?");

    doAssignCigArgs(pos,cgiargs);
}

void GetRequest::doAssignCigArgs(std::string::size_type pos,std::string& cgiargs){
    if(pos!=std::string::npos)
        cgiargs=getUri().substr(pos,getUri().length()-1);
    else
        cgiargs.clear();
}

