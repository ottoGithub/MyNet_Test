#ifndef _IOCP_REACTOR_H_
#define _IOCP_REACTOR_H_

#include "NetReactor.h"


class IOCPReactor : public NetReactor
{
public:
	IOCPReactor(int32 tNumThread);
	virtual ~IOCPReactor();
public:
	virtual bool Open(int32 maxNumHandlers, uint32 tickInter);
	virtual void Close();
	virtual bool RegisterHandler(NetEventHandler* pHander, uint32 tEvent = NetEvent_All);
	virtual bool RemoveHandler(NetEventHandler* pHander, uint32 tEvent = NetEvent_All);
	virtual bool HandleEvent(const int64& tNow);
	virtual int32 GetHandlerNum()const;
protected:
	int32 numOfThread;
	HANDLE completion_port;
	bool m_isOpen;
};

#endif