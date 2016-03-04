#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include <string>
#include <vector>
#include "Mutex.h"

#include "NetManagerBase.h"
#include "TFunctionArg.h"
#include "CircleList.h"

const int MAX_SOCKET_LIST = 128;

class Thread;
class NetReactor;

class NetManager : public NetManagerBase
{
public:
	NetManager(int32 numThread,int32 skRcv,int32 stRcv,int32 skSnd, int32 stSnd,
		CFunctionArg1Base<int32>* AptFun,CFunctionArg1Base<int32>* conFun,
		CFunctionArg1Base<int32>* DisFun);
	~NetManager();

	virtual void SetPacketProcess(PacketProcessEx* pktpoc) { m_pktProc = pktpoc; }
	virtual PacketProcessEx* GetPacketProcess() { return m_pktProc; }
	virtual bool ProcessPacket(PacketBase* pkt);

	virtual void StartWorkThread();
	virtual void StopWorkThread();

	virtual bool Accept(const char* ip, int32 port);
	virtual SocketBase* Connect(const char* ip,int32 port);
	virtual SocketBase* GetSocket(int32 socketid);
	virtual bool Disconnect(int32 scoketid);

	virtual bool OnAccept(SocketBase* pSocket);
	virtual bool OnConnect(SocketBase* pSocket);

private:
	void ThreadRun();
	void RunAccept();

protected:
	std::string m_ip;
	int32 m_port;

	int32 m_numThread;
	int32 m_socketRcvSize;
	int32 m_socketSndSize;
	int32 m_streamRcvSize;
	int32 m_streamSndSize;
	CFunctionArg1Base<int32>* m_ConFun;
	CFunctionArg1Base<int32>* m_AptFun;
	CFunctionArg1Base<int32>* m_DisFun;

	PacketProcessEx* m_pktProc;

	bool m_isThreadExit;
	std::vector<Thread*> m_Threads;

	NetReactor* m_pReactor;

	SocketBase* m_pListenSocket;

	Thread* m_aptThread;

	Mutex m_Mutex;
	//Mutex m_DelMutex;

	CircleList<SocketBase*, MAX_SOCKET_LIST> m_SocketList;
};

#endif