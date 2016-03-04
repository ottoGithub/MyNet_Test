#ifndef _PACKET_H_
#define _PACKET_H_

#include "Types.h"
#include "BaseClass.h"
#include "PacketBase.h"
#include "PacketFactory.h"
#include "PoolAllocator.h"

#define PACKET(packet, basepacket)\
class packet: public basepacket{\
	DECLARE_FACTORY_ARG0(packet, UtilID::StringToUtilID(#packet,(uint32)strlen(#packet)), new PoolAllocator<Mutex>(sizeof(packet)), s_PacketFactory)\
public:\
	packet(){\
	m_packetid=GetClassStatic()->GetClassID();}\
	~packet(){}\
	char* ReadPacket(char* buffer){\
	char* tmp=basepacket::ReadPacket(buffer);\
	return Read(tmp,Type_Ser);}\
	char* WritePacket(char* buffer){\
	char* tmp=basepacket::WritePacket(buffer);\
	return Write(tmp,Type_Ser);}\
	int32 GetPakcetSize(){\
	return basepacket::GetPakcetSize()+GetClassStatic()->GetClassSerSize();}\
	int32 PacketSize(){return sizeof(packet);}

#define START_ADD_PACKET_MEMBER(packet)\
	Reg_Base_Class_Begin(packet)

#define ADD_PACKET_MEMBER(packet,member,type,des)\
	Add_Class_Member(packet,member,type,Type_Ser,des)

#define ADD_PACKET_MEMBER_STR(packet,member,size,des)\
	Add_Class_Member_Char(packet,member,size,Type_Ser,des)

#define PACKET_END(packet)\
	Reg_Base_Class_End(packet)\
};

#define PACKET_IMPL(packet)\
	Reg_Class_Initial(packet)\
	FINISH_FACTORY_ARG0(packet)

#endif