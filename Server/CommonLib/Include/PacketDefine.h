#ifndef _PACKET_DEFINE_H_
#define _PACKET_DEFINE_H_

#include "Types.h"
#include "Packet.h"

PACKET(PacketText, PacketBase)
	int32 flag;
START_ADD_PACKET_MEMBER(PacketText)
	ADD_PACKET_MEMBER(PacketText,flag,int32,test)
	PACKET_END(PacketText)

#endif