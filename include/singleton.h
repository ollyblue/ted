/*
 * =====================================================================================
 *
 *       Filename:  singleton.h
 *
 *    Description:  单例模式封装，考虑了多线程的情况，用了double check 加锁，但是在c++语言
 *                  中无法保证new的原子性所以还是有可能发生异常情况，这是无法保证的
 *
 *        Version:  1.0
 *        Created:  2014年03月22日 21时44分24秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  ollyblue (ollybluew@gmail.com)
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _TED_SINGLETON_H
#define _TED_SINGLETON_H

#include "lock.h"

namespace ted {

template <typename T, typename Lock = class CDummyLock >
class CSingleton
{
public:
  static T* Instance()
  {
    if( 0 == m_pInstance )
    {
      // try lock
      Lock l;
      l.Lock();
      if( 0 == m_pInstance )
      {
        m_pInstance = new T();
      }
      l.UnLock();
    }
    return m_pInstance;
  }
private:
  static T* m_pInstance;
};

template<typename T, typename Lock>
T* CSingleton<T, Lock>::m_pInstance = 0;

}; // !namespace ted

#endif // !_TED_SINGLETON_H
