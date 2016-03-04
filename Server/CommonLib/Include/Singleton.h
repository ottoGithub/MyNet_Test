#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <stdlib.h>
#include <new>
#include "NoCopyable.h"

class NoLock :private NoCopyable
{
public:
	NoLock(){}
	~NoLock(){}

	void Lock(){}
	void UnLock(){}
};

template<class T, typename Lock = NoLock>
class Singleton
{
public:
	Singleton(){}
	virtual ~Singleton(){}

	static inline T& Instance() { return *InstancePtr();}
	static inline T* InstancePtr()
	{
		if(!m_ptr)
		{
			m_Lock.Lock();
			m_ptr = (T*)::malloc(sizeof(T));
			if(m_ptr)
			{
				new (m_ptr)T();
			}
			m_Lock.UnLock();
		}
		return m_ptr;
	}
	static void SetInstance( T* pObj)
	{
		m_ptr = pObj;
	}
private:
	static T* m_ptr;
	static Lock m_Lock;
};

template<class T, typename Lock> T* Singleton<T,Lock>::m_ptr = NULL;
template<class T, typename Lock> Lock Singleton<T,Lock>::m_Lock;

#endif