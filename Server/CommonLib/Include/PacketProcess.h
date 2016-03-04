#ifndef _PACKET_PROCESS_H_
#define _PACKET_PROCESS_H_

#include <map>


#include "Singleton.h"
#include "TFunctionArg.h"
#include "ProcessBase.h"
#include "PacketFunction.h"
#include "Mutex.h"
#include "PacketList.h"

class PacketBase;


#define REG_PACKET_HANDLER( processor, packet, classObj, function ) \
	processor->RegPacketHandler(#packet, UtilID::StringToUtilID(#packet,(uint32)strlen(#packet)), new CFunctionPacket<classObj,packet*,PacketBase*>(this,&classObj::function) ); packet tmppkt##packet;

#define REG_DEFAULT_PACKET_HANDLER(processor,basePacket,classObj,function) \
	processor->RegDefaultHandler( new CFunctionPacket<classObj,basePacket*,basePacket*>(this,&classObj::function));

#define REMOVE_PACKET_HANDLER(processor,funcName,packet) \
	processor->RemovePacketHandler(  UtilID::StringToUtilID(#packet,(uint32)strlen(#packet)), #funcName );

class PacketProcess
{
	typedef std::map<uint32, ProcessBase<CFunctionArg1Base<PacketBase*>, PacketBase* >* > FunMap;
public:
	PacketProcess();
	~PacketProcess();
	virtual bool ProcessPacket(PacketBase* pkt);
	virtual void RegPacketHandler(const char* name, uint32 packetid, CFunctionArg1Base<PacketBase*>* Fun);
	virtual void RemovePacketHandler(uint32 packetid, CFunctionArg1Base<PacketBase*>* Fun);
	virtual void RegDefaultHandler(CFunctionArg1Base<PacketBase*>* DefFun);

private:
	FunMap m_Funmap;
	ProcessBase<CFunctionArg1Base<PacketBase*>, PacketBase* >* m_DefFun;
};

class PacketProcessEx
{
	typedef std::map<uint32, CFunctionArg1Base<PacketBase*>* > FunMap;
public:
	PacketProcessEx();
	~PacketProcessEx();
	virtual bool ProcessPacket(PacketBase* pkt);
	virtual void RegPacketHandler(const char* name, uint32 packetid, CFunctionArg1Base<PacketBase*>* Fun);
	virtual void RemovePacketHandler(uint32 packetid, CFunctionArg1Base<PacketBase*>* Fun);
	virtual void RegDefaultHandler(CFunctionArg1Base<PacketBase*>* DefFun);
private:
	FunMap m_Funmap;
	CFunctionArg1Base<PacketBase*>* m_DefFun;
};

class PacketProc : public PacketProcessEx
{
public:
	PacketProc();
	~PacketProc();

	bool DoProc();
private:
	Mutex	m_Mutex;
	PacketList m_PktList;
};

#endif