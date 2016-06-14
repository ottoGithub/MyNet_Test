#include <iostream>
#include "EventLoop.h"
#include "TcpServer.h"

using namespace xtProximaCentauri;

int main()
{
	EventLoop el;
	InetAddress ia("127.0.0.1", 9527);
	TcpServer ts(&el, ia, "TcpServerTest", ServerOption_t::eReusePort);
	ts.Start();
	el.Loop();
	return 0;
}