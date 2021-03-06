#ifndef _POOL_ALLOCATOR_H_
#define _POOL_ALLOCATOR_H_

#include "Types.h"
#include "Allocator.h"
#include "Mutex.h"


struct sMemoryCell 
{
	sMemoryCell* m_pnext;
	MemoryHead m_info;
	sMemoryCell()
	{
		m_pnext = NULL;
	}
};

template<typename T>
class PoolAllocator: public Allocator
{
public:
	PoolAllocator();
	PoolAllocator(int32 size);
	~PoolAllocator();
	virtual void* DLMalloc(int32 size);
	virtual void DLFree(void* ptr);
	virtual void SetMemoryInfo(const MemoryHead& info);
	virtual bool Init(int32 size);
	virtual void CollectMemory();
	virtual bool IsValidMemory(void* ptr);
	virtual int32 GetAllocInfo(void* ptr);
	virtual int64 GetMemroyUsage();
	virtual int32 GetType(){return MEMORY_POOL;}
	virtual MemoryHead GetMemoryHead()
	{
		return m_info;
	}
protected:
	void* PopMemory();
	void PushMemory(void* ptr);
	uint32 m_freecount;
	uint32 m_count;
	uint64 m_memusage;
	int32 m_objsize;
	int32 m_cellsize;
	MemoryHead m_info;
	sMemoryCell* m_phead; //�����ڴ�ͷָ��
	T m_mutex;
private:
};

#include "PoolAllocator.inl"

#endif