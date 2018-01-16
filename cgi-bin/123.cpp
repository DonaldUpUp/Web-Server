#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;
 
int main (int argc,char *argv[])
{
   //string arg=getenv("QUERY_STRING");
   cout << "Content-type:text/html\r\n\r\n";
   cout << "<html>\n";
   cout << "<head>\n";
   cout << "<title>Hello World - 第一个 CGI 程序</title>\n";
   cout << "</head>\n";
   cout << "<body>\n";
   cout << "<h2>Hello World! 这是我的第一个 CGI 程序</h2>\n";
   cout << "<tr><td>";
   if(argc<3){
	cout<<"Parameter little!";
	cout<<"</td></tr>\n";
   	cout << "</body>\n";
   	cout << "</html>\n";
	return -1;
   }
   cout<<"First parm="<<argv[1]<<"\n";
   cout<<"Second parm="<<argv[2]<<"\n";
   cout<<"</td></tr>\n";
   cout << "</body>\n";
   cout << "</html>\n";
   return 0;
}
