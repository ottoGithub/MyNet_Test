#ifndef _MY_SOCKET_H_
#define _MY_SOCKET_H_

#include "Types.h"
#include "SocketBase.h"
#include "NetReactor.h"
#include "CompleteOperation.h"

#ifdef _WIN32
#include <ws2def.h>
#endif

const int SEND_BUFFER = 4096;
const int RECV_BUFFER = 4096;

const int PACKET_LEN = 2;
const int PACKET_HEAD_ID = 4;
const int PACKET_SESSION_ID = 4;
const int PACKET_TYPE_ID = 1;

const int PACKET_HEAD = PACKET_LEN + PACKET_HEAD_ID + PACKET_SESSION_ID + PACKET_TYPE_ID;
const int MAX_PACKET_BUFFER = 512;

class MySocket : public SocketBase, public NetEventHandler
{
public:
	MySocket(NetManagerBase* mgr, uint32 sndBufSize, uint32 rvcBufSize, int32 socketRcvBuff = RECV_BUFFER, int32 socketSndBuff = SEND_BUFFER);
	~MySocket();

#ifdef _WIN32
public:
	WSABUF* GetRecvWSABUF();
	WSABUF* GetSendWSABUF();

	CompleteOperator* GetRecvCompleteOperation();
	CompleteOperator* GetSendCompleteOperation();

	bool PostRecvOperation();
	bool PostSendOperation();

private:
	WSABUF m_RecvWSABUF;
	WSABUF m_SendWSABUF;
	CompleteOperator m_RecvCompleteOperation;
	CompleteOperator m_SendCompleteOperation;
#elif defined(__GNUC__)
public:
	AsyncCompleteOperation* GetAsyncCompleteOperation();

private:
	SessionCompleteOperation m_SessionCompleteOperation;
#endif
public:
	virtual void SetSocket(SOCKET s){m_Sock = s;}
	virtual SOCKET GetSocket(){return m_Sock;}

	virtual SocketBase* Accept(uint32 sndBuffSize, uint32 recvBuffSize, int32 socketRcvBuff, int32 socketSndBuff );
	virtual bool Listen(const std::string& ip, int32 port);
	virtual bool Connect(const std::string& ip, int32 port);
	virtual bool Recv();
	virtual void Disconnect();

	virtual bool CreateSocket();
	virtual bool SendPacket(PacketBase* pkt);
	virtual PacketBase* RecvPacket();

public:
	virtual SocketBase* GetHandleSocket();
	virtual void HandleInput(void* pParam);
	virtual void HandleOutput(void* pParam);
	virtual void HandleException(void* pParam);
	virtual void HandleTick(const uint64& tNow);
	virtual void HandleClose(void* pParam);
public:
	bool InitSocketBuffer();
protected:
	SOCKET m_Sock;

	CircularBuffer m_SendBuffer;
	CircularBuffer m_RecvBuffer;

	int32 m_SocketRcvBuff;
	int32 m_SocketSndBuff;
	NetManagerBase* m_NetManger;
};

#endif