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
  ted::CLog log;

  log.Init("/tmp/log/", __FILE__,  ted::LOG_LEVEL_ALL, 100ull, 5 );
  int i = 1000;
  while(i--)
  {
    std::string strMsg("test log");
    strMsg =  strMsg + log.GetDate() + "\n";
    if( !log.WriteBase(strMsg, ted::LOG_LEVEL_ALL) )
    {
      printf("err:%s\n", log.GetLastErrMsg());
    }
  }
  return 0;
}
