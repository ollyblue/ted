/*
 * =====================================================================================
 *
 *       Filename:  lock.h
 *
 *    Description: 针对lock相关的简单封装 
 *
 *        Version:  1.0
 *        Created:  2014年03月22日 21时37分38秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  ollyblue (ollybluew@gmail.com)
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _TED_LOCK_H_
#define _TED_LOCK_H_

#include <pthread.h>

namespace ted {

class CDummyLock 
{ 
public:
	bool Lock()
	{ 
		return true;
	}

	bool UnLock()
	{ 
		return true;
	}
};

class CLockBase
{
public:
	CLockBase() 
	{ }

	virtual ~CLockBase()
	{ }

	virtual bool Lock() = 0;
	virtual bool UnLock() = 0;
};

class CMutex: public CLockBase
{
public:
	CMutex()
	{
		::pthread_mutex_init(&m_lock, NULL);
	}

	~CMutex()
	{
		::pthread_mutex_destroy(&m_lock);
	}

public:
	inline virtual bool Lock()
	{
		return ::pthread_mutex_lock(&m_lock);
	}

	inline virtual bool UnLock()
	{
		return ::pthread_mutex_unlock(&m_lock);
	}

private:
	CMutex(const CMutex&);
	CMutex& operator = (const CMutex&);
	
private:
	pthread_mutex_t	m_lock;
};
	
class CAutoLock
{
public:
	CAutoLock(CLockBase &lock): m_lock(lock)
	{
		m_lock.Lock();
	}

	virtual ~CAutoLock()
	{
		m_lock.UnLock();
	}

private:
	CLockBase&	m_lock;
};

} // !namespace ted

#endif // !_TED_LOCK_H_
