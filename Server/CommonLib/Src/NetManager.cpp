#include "PreComp.h"
#include "NetManager.h"
#include "PacketProcess.h"
#include "Thread.h"
#include "MySocket.h"

#ifdef _WIN32
#include "IOCPReactor.h"
#endif

NetManager::NetManager(int32 numThread,int32 skRcv,int32 stRcv,int32 skSnd, int32 stSnd,
	CFunctionArg1Base<int32>* AptFun,CFunctionArg1Base<int32>* conFun,
	CFunctionArg1Base<int32>* DisFun) : m_numThread(numThread),m_socketRcvSize(skRcv),
	m_streamRcvSize(stRcv),m_socketSndSize(skSnd),
	m_streamSndSize(stSnd),m_AptFun(AptFun),m_ConFun(conFun),m_DisFun(DisFun)
{
	m_pktProc = NULL;
	m_pListenSocket = NULL;
	m_aptThread = NULL;
	m_isThreadExit = false;
#ifdef _WIN32
	//initialize Winsock
	WSADATA wsaData;
	if (WSAStartup( MAKEWORD(2,2), &wsaData ) != NO_ERROR)
	{
		return;
	}
	m_pReactor = new IOCPReactor(4);
#endif
	m_pReactor->Open(4,0);
}

NetManager::~NetManager()
{
	SAFE_DELETE(m_AptFun);
	SAFE_DELETE(m_ConFun);
	SAFE_DELETE(m_DisFun);
	m_isThreadExit = true;
	if(WSACleanup() != NO_ERROR)
	{

	}
}

void NetManager::StartWorkThread()
{
	for( int32 i = 0; i < m_numThread; ++i )
	{
		char tname[64] = {0};
		sprintf_s(tname,"Peer Net Thread:%d",i);
		m_Threads.push_back(new Thread(new CFunctionArg0<NetManager>(this,&NetManager::ThreadRun), tname) );
	}
	std::vector<Thread*>::iterator it = m_Threads.begin();
	std::vector<Thread*>::iterator end = m_Threads.end();
	for( ; end != it; ++it )
	{
		(*it)->start();
	}

}

void NetManager::StopWorkThread()
{

}

void NetManager::ThreadRun()
{
	while(!m_isThreadExit)
	{
		m_pReactor->HandleEvent(INFINITE);
	}
}

bool NetManager::Accept(const char* ip, int32 port)
{
	if(m_pListenSocket)
	{
		return false;
	}
	m_pListenSocket = new MySocket(this, 4096, 4096);
	if(!m_pListenSocket->Listen(ip,port))
	{
		SAFE_DELETE(m_pListenSocket);
		return false;
	}
	m_aptThread = new Thread(new CFunctionArg0<NetManager>(this,&NetManager::RunAccept), "accept thread");
	if(!m_aptThread)
	{
		return false;
	}
	m_aptThread->start();
	return true;
}

SocketBase* NetManager::Connect(const char* ip,int32 port)
{
	MySocket* pSocket = new MySocket(this, m_streamSndSize,m_streamRcvSize,m_socketRcvSize,m_socketSndSize);
	if (!pSocket)
	{
		return NULL;
	}
	if (!pSocket->Connect(ip,port))
	{
		return NULL;
	}
	//notify connect
	pSocket->SetReactor(m_pReactor);
	OnConnect(pSocket);
	return pSocket;
}

bool NetManager::OnConnect(SocketBase* pSocket)
{
	if (!pSocket)
	{
		return false;
	}
	MutexGuard tmpMutex(m_Mutex);
	MySocket* pMySocket = (MySocket*)pSocket;
	if (!pMySocket)
	{
		return false;
	}
	NetEventHandler* pHandler = (NetEventHandler*)pMySocket;
	if (!pHandler)
	{
		return false;
	}
	//add sock to epoll set
	if(!m_pReactor->RegisterHandler(pHandler,NetEvent_IO))
	{
		return false;
	}
	int32 sid = m_SocketList.Add(pSocket);
	pSocket->SetID(sid);
	if (m_ConFun)
	{
		(*m_ConFun)(pSocket->GetID());
	}
#ifdef _WIN32
	return	pMySocket->PostRecvOperation();
#endif
}

void NetManager::RunAccept()
{
	while(!m_isThreadExit)
	{
		MySocket* pSocket = (MySocket*) m_pListenSocket->Accept(m_streamSndSize, m_streamRcvSize,m_socketRcvSize,m_socketSndSize);
		if (pSocket)
		{
			OnAccept(pSocket);
		}
	}
}

bool NetManager::OnAccept(SocketBase* pSocket)
{
	if (!pSocket)
	{
		return false;
	}
	MutexGuard tmpMutex(m_Mutex);
	MySocket* pMySocket = (MySocket*)pSocket;
	if(!pMySocket)
	{
		return false;
	}
	NetEventHandler* pHandler = (NetEventHandler*)pMySocket;
	if (!pHandler)
	{
		return false;
	}
	if(!m_pReactor->RegisterHandler(pHandler,NetEvent_IO))
	{
		return false;
	}
	int32 sid = m_SocketList.Add(pSocket);
	pSocket->SetID(sid);
	if (m_AptFun)
	{
		(*m_AptFun)(pSocket->GetID());
	}
#ifdef _WIN32
	return pMySocket->PostRecvOperation();	
#endif
}

bool NetManager::Disconnect(int32 socketid)
{
	MutexGuard tmpMutex(m_Mutex);
	SocketBase* pSocket = GetSocket(socketid);
	if (!pSocket)
	{
		return false;
	}
	//OnDisconnect(pSocket);
	if (m_DisFun)
	{
		(*m_DisFun)(pSocket->GetID());
	}
	m_SocketList.Remove(pSocket->GetID());
	//MutexGuard tMutex(m_DelMutex);
	//m_DelList.Add(pSocket);
	return true;
}

SocketBase* NetManager::GetSocket(int32 socketid)
{
	return m_SocketList.Get(socketid);
}

bool NetManager::ProcessPacket(PacketBase* pkt)
{
	return m_pktProc->ProcessPacket(pkt);
}