#include "PreComp.h"
#include "PeerBase.h"
#include "PacketProcess.h"
#include "TFunctionArg.h"
#include "NetManager.h"
#include "PacketDefine.h"
#include "PacketBase.h"
#include "SocketBase.h"
#include "PacketFactory.h"
#include "GTimer.h"
#include "CommonLog.h"

PeerBase::PeerBase()
{		
	m_PeerPocPacket = NULL;
	m_NetManger = NULL;
	m_PeerIP = "127.0.0.1";
	m_PeerPort = 8889;
	m_LastFrameTime = 0;
}

PeerBase::~PeerBase()
{
	SAFE_DELETE(m_PeerPocPacket);
	SAFE_DELETE(m_NetManger);
}

bool PeerBase::Init(const char* szIp, int port)
{
	m_PeerIP = szIp;
	m_PeerPort = port;
	m_PeerPocPacket = new PacketProc();
	if (!m_PeerPocPacket)
	{
		return false;
	}
	CFunctionArg1Base<int32>* AFun = new CFunctionArg1<PeerBase,int32>(this, &PeerBase::OnPeerAccept);
	CFunctionArg1Base<int32>* CFun = new CFunctionArg1<PeerBase,int32>(this, &PeerBase::OnPeerConnect);
	CFunctionArg1Base<int32>* DFun = new CFunctionArg1<PeerBase,int32>(this, &PeerBase::OnPeerDisconnect);
	
	m_NetManger = new NetManager(2,4096,4096,4096,4096,AFun,CFun,DFun);
	if (!m_NetManger)
	{
		return false;
	}
	m_NetManger->SetPacketProcess(m_PeerPocPacket);
	m_NetManger->StartWorkThread();
	RegPktHandler();
	return true;
}

bool PeerBase::PeerAccept()
{
	if(!m_NetManger)
	{
		return false;
	}
	return m_NetManger->Accept(m_PeerIP.c_str(), m_PeerPort);
}

int PeerBase::PeerConnect()
{
	if(!m_NetManger)
	{
		return false;
	}
	SocketBase* pSocket = m_NetManger->Connect(m_PeerIP.c_str(), m_PeerPort);
	if (!pSocket)
	{
		return -1;
	}
	return pSocket->GetID();
}

void PeerBase::OnPeerConnect(int32 socketid)
{
	PeerConnect(socketid);
}
void PeerBase::OnPeerAccept(int32 socketid)
{
	PeerAccept(socketid);
}
void PeerBase::OnPeerDisconnect(int32 socketid)
{
	PeerDisconnect(socketid);
}

void PeerBase::RegPktHandler()
{
	if(!m_PeerPocPacket)
	{
		return;
	}
	REG_PACKET_HANDLER(m_PeerPocPacket,PacketText,PeerBase,PktTestHandler);
	//REG_DEFAULT_PACKET_HANDLER(m_PeerPocPacket, PacketBase, PeerBase, DefaultPeerPktHandler);
}

void PeerBase::PktTestHandler(PacketText* pkt)
{
	if (!pkt)
	{
		return;
	}
	LOG_INFO("packet flag = %d;",pkt->flag);
}

void PeerBase::PeerRun()
{
	//TODO:
	DoPeerRun();
	int32 tTime = GTimer::GetTimeMS();
	int32 tPeerNetThreadFrameTime = tTime - m_LastFrameTime;
	m_LastFrameTime = tTime;
	PeerProcPacket();
	if (m_PktSendList.GetPacketCount() <= 0)
	{
		//ÔÝÍ£Ò»»áÅ¶¡£¡£¡£
	}
	int32 tSleeptime = 100 - tPeerNetThreadFrameTime;
	Sleep(100);
}

void PeerBase::DoPeerRun()
{
	if(m_NetManger)
	{
		DoPeerSend();
	}
}

bool PeerBase::PeerProcPacket()
{
	if (!m_PeerPocPacket)
	{
		return false;
	}
	PacketProc* pProc = (PacketProc*)m_PeerPocPacket;
	return pProc->DoProc();
}

void PeerBase::DoPeerSend()
{
	int32 realSndCount = 0;
	int32 curSndCount = 0;
	int32 dropCount = 0;
	PacketBase* pkt = m_PktSendList.Pop_Head();
	while (pkt)
	{
		SocketBase* s = m_NetManger->GetSocket(pkt->GetSocketID());
		if (s)
		{
			++curSndCount;
			if (!s->SendPacket(pkt))
			{
				if (!pkt->m_isDrop)
				{
					m_PktFailedList.Push_Back(pkt);
					pkt = m_PktSendList.Pop_Head();
					continue;
				}
				++dropCount;
			}
			++realSndCount;
			
			s_PacketFactory.DestroyPacket(pkt);
		}
		if (curSndCount >= 50)
		{
			break;
		}
		pkt = m_PktSendList.Pop_Head();
	}
	{
		PacketBase* failpkt = m_PktFailedList.Pop_Back();
		while (failpkt)
		{
			m_PktSendList.Push_Back(failpkt);
			failpkt = m_PktFailedList.Pop_Back();
		}
	}
}


bool PeerBase::PeerSend(int32 socketid, PacketBase* pkt)
{
	if (socketid < 0 || !pkt)
	{
		return false;
	}
	PacketBase* tmppkt = (PacketBase*)s_PacketFactory.CreatePacket(pkt->GetPacketID());
	if (!tmppkt)
	{
		return false;
	}
	char buffer[MAX_PACKET_SIZE];
	pkt->WritePacket(buffer);
	tmppkt->ReadPacket(buffer);
	tmppkt->SetSocketID(socketid);
	tmppkt->m_isDrop = tmppkt->m_isDrop;
	return m_PktSendList.Push_Back(tmppkt);
}