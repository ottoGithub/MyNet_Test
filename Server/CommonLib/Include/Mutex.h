#ifndef _MUTEX_H_
#define _MUTEX_H_
#include "NoCopyable.h"
#if defined(_WIN32)
#include <Windows.h>
#else
#include <pthread.h>
#endif


class Mutex : private NoCopyable
{
public:
	Mutex()
	{
#ifdef _WIN32
		// use critical sections in windows; much faster
		InitializeCriticalSection( &mutex_ );
#else
		pthread_mutex_init(&mutex_, NULL);
#endif
	}

	~Mutex()
	{
#ifdef _WIN32
		DeleteCriticalSection( &mutex_ );
#else
		pthread_mutex_destroy(&mutex_);
#endif
	}

	void Lock()
	{
#ifdef _WIN32
		EnterCriticalSection( &mutex_ );
#else
		pthread_mutex_lock(&mutex_);
#endif
	}

	void UnLock()
	{
#ifdef _WIN32
		LeaveCriticalSection( &mutex_ );
#else
		pthread_mutex_unlock(&mutex_);
#endif
	}

#ifdef _WIN32
	CRITICAL_SECTION*
#else
	pthread_mutex_t* 
#endif
		getPthreadMutex() /* non-const */
	{
		return &mutex_;
	}

private:
#ifdef _WIN32
	CRITICAL_SECTION mutex_;
#else
	pthread_mutex_t mutex_;
#endif
};

class MutexGuard : private NoCopyable
{
public:
	explicit MutexGuard(Mutex& pMutex );
	~MutexGuard();

public:
	void Lock();
	void UnLock();

private:
	Mutex& m_pMutex;
	bool m_Owner;
};

inline
	MutexGuard::MutexGuard( Mutex& pMutex )
	: m_pMutex( pMutex ), m_Owner(false)
{
	Lock();
}

inline MutexGuard::~MutexGuard()
{
	UnLock();
}

inline void MutexGuard::Lock()
{
	m_pMutex.Lock();
	m_Owner = true;
}

inline void MutexGuard::UnLock()
{
	if( m_Owner )
	{
		m_Owner = false;
		m_pMutex.UnLock();
	}
}


#endif