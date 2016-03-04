#include "PreComp.h"
#include "NodeServer.h"

int main(int argc, char* argv[])
{
	NodeServer::Instance().Init();
	NodeServer::Instance().Start();
	return 0;
}