#ifndef _NODE_SERVER_H_
#define _NODE_SERVER_H_
#include "Singleton.h"
#include "PeerBase.h"
#include "Mutex.h"

class NodeServer : public Singleton<NodeServer, Mutex> ,public PeerBase
{
public:
	void Init();
	void Run();
	void Start();
};

#endif