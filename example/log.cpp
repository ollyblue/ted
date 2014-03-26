/*
 * =====================================================================================
 *
 *       Filename:  log.cpp
 *
 *    Description:  for test log.h
 *
 *        Version:  1.0
 *        Created:  2014年03月23日 21时27分02秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  ollyblue (ollybluew@gmail.com)
 *        Company:  
 *
 * =====================================================================================
 */

#include "include/log.h"

int main(int argc, const char *argv[])
{
  //ted::CLog log;

  // log.Init("/tmp/log/", __FILE__,  ted::LOG_LEVEL_ALL, 100ull, 5 );
  TED_LOG->Init("/tmp/log/", __FILE__,  ted::LOG_LEVEL_ALL, 100ull, 5 );
#if 0
  int i = 100000;
  while(i--)
  {
    std::string strMsg("test log");
    if( !log.Write(__FILE__,
                   __func__,
                   __LINE__,
                   ted::LOG_LEVEL_ALL,
                   "%s",
                   strMsg.c_str()) )
    {
      printf("err:%s\n", log.GetLastErrMsg());
    }
  }
#endif

  int a = 10;
  TED_LOG_DEBUG("a:%d", a);
  return 0;
}
