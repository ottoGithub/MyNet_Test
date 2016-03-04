#ifndef _PACKET_LIST_H_
#define _PACKET_LIST_H_

#include "Types.h"
#include "Mutex.h"


class PacketBase;

class PacketList
{
public:
	PacketList();
	virtual ~PacketList();
	virtual bool Push_Head(PacketBase* pkt);
	virtual PacketBase* Pop_Head();
	virtual bool Push_Back(PacketBase* pkt);
	virtual PacketBase* Pop_Back();
	virtual PacketBase* GetNextPacket(PacketBase* pkt);
	int32 GetPacketCount()const{return m_count;}
protected:
private:
	PacketBase* m_head;
	PacketBase* m_back;
	int32 m_count;
};

class PacketListEx: public PacketList
{
public:
	PacketListEx(){}
	virtual ~PacketListEx(){}
	virtual bool Push_Head(PacketBase* pkt);
	virtual PacketBase* Pop_Head();
	virtual bool Push_Back(PacketBase* pkt);
	virtual PacketBase* Pop_Back();
	virtual PacketBase* GetNextPacket(PacketBase* pkt);
private:
	Mutex m_Mutex;
};

#endif