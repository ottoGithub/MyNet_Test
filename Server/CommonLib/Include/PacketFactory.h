#ifndef _PACKET_FACTORY_H_
#define _PACKET_FACTORY_H_

#include "Types.h"
#include "Factory.h"
#include "Singleton.h"
#include "Mutex.h"
#include "TManager.h"


class PacketBase;

class PacketFactory: public TManager<CFactoryArg0_Base>, public Singleton<PacketFactory, Mutex>
{
public:
	PacketFactory(){}
	~PacketFactory(){}
	void* CreatePacket(const char* name);
	void* CreatePacket(uint32 packetid);
	bool DestroyPacket(PacketBase* pkt);
protected:
private:

	PacketFactory(const PacketFactory&);
	PacketFactory& operator=(const PacketFactory&);
};

#define s_PacketFactory PacketFactory::Instance()

#endif