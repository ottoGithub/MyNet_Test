#include <iostream>
#include <string>
#include "PeerBase.h"
#include "PacketDefine.h"

int main(int argc , char *argv[])
{
	std::string ip = "127.0.0.1";
	int port = 10010;
	int isListen = 0;
	for (int i = 0; i < argc; ++i)
	{
		if (strcmp(argv[i], "-ip") == 0)
		{
			ip = argv[i+1];
		}
		else if (strcmp(argv[i], "-port") == 0)
		{
			port = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-type") == 0)
		{
			isListen = atoi(argv[i+1]);
		}
	}
	PeerBase* pPeer = new PeerBase();
	if (!pPeer)
	{
		return 0;
	}
	pPeer->Init(ip.c_str(),port);
	if(isListen)
	{
		std::cout<<"start listen...."<<std::endl;
		pPeer->PeerAccept();
	}
	else
	{
		std::cout<<"start connecting...."<<std::endl;
		pPeer->PeerConnect();
		PacketText pkt;
		pkt.flag = 1;
		pPeer->PeerSend(0,&pkt);
	}
	while (true)
	{
		pPeer->PeerRun();
	}
	std::cout<<"test ok!"<<std::endl;
}