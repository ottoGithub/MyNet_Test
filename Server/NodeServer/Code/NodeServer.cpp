#include "PreComp.h"
#include "NodeServer.h"
#include "PeerBase.h"

void NodeServer::Init()
{
	PeerBase::Init("127.0.0.1",10010);
	PeerBase::PeerAccept();
}

void NodeServer::Start()
{
	Run();
}

void NodeServer::Run()
{
	while(true)
	{
		PeerBase::PeerRun();
	}
}