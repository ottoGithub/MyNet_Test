#ifndef _PACKET_FUNCTION_H_
#define _PACKET_FUNCTION_H_

#include "TFunctionArg.h"

template<class ClassObj,typename SubPacket,typename BasePacket>
class CFunctionPacket : public CFunctionArg1Base<BasePacket>
{
	typedef void( ClassObj::*Function)(SubPacket pkt);
public:
	CFunctionPacket(ClassObj* p, Function f)
	{
		pObj = p;
		func = f;
	}
	virtual ~CFunctionPacket(void){}
	virtual void operator()(BasePacket basePkt)
	{
		if (pObj && func)
		{
			SubPacket pSubPkt = (SubPacket)basePkt;
			(pObj->*func)(pSubPkt);
		}			
	}
private:
	ClassObj* pObj;
	Function func;
};



#endif