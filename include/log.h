/*
 * =====================================================================================
 *
 *       Filename:  log.h
 *
 *    Description:  封装一个log库
 *
 *        Version:  1.0
 *        Created:  2014年03月22日 22时09分59秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  ollyblue (ollybluew@gmail.com)
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _TED_LOG_H
#define _TED_LOG_H

#include "lock.h"

#include <stdint.h>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>

namespace ted {

#define FILE_SAFE_CLOSE( ofs ) do {\
  if( (ofs).is_open() )\
  {\
    (ofs).close();\
  }\
}while(0)

#define FILE_SAFE_FLUSH( ofs ) do {\
  if( (ofs).is_open() )\
  {\
    (ofs).flush();\
  }\
}while(0)


const uint32_t MAX_ERR_MSG_SIZE = 1024;
const uint32_t KB = 1024;
const uint32_t MB = 1024 * KB;
const uint32_t GB = 1024 * MB;
const uint64_t DEFAULT_LOG_SIZE = 10 * KB;
const uint32_t DEFAULT_LOG_FILE_NUM = 3;

enum LOG_LEVEL
{
  LOG_LEVEL_ALL   = 9,
  LOG_LEVEL_DEBUG = 8,
  LOG_LEVEL_INFO  = 7,
  LOG_LEVEL_RUN   = 6,
  LOG_LEVEL_WARN  = 5,
  LOG_LEVEL_ERROR = 4,
  LOG_LEVEL_EMERG = 2,
  LOG_LEVEL_FATAL = 1,
  LOG_LEVEL_OFF   = 0,
};

class CLog
{
public:
 CLog(const std::string& strLogPath,
      const LOG_LEVEL dwLogLevel = LOG_LEVEL_ERROR,
      const uint64_t ddwMaxLogSize = DEFAULT_LOG_SIZE,
      const uint32_t dwMaxLogNum = DEFAULT_LOG_FILE_NUM);

 ~CLog();

public:
 inline bool Init(const std::string& strLogPath,
            const LOG_LEVEL dwLogLevel = LOG_LEVEL_ERROR,
            const uint64_t ddwMaxLogSize = DEFAULT_LOG_SIZE,
            const uint32_t dwMaxLogNum = DEFAULT_LOG_FILE_NUM);

 inline const char * GetLastErrMsg() { return m_szErrMsg; }

 bool Write(const std::string& strFile,
            const std::string& strFunction,
            const uint32_t  dwLine,
            const char* strFormat,
            ...);

private:
 bool ShiftAllFiles();
 bool ShiftOneTypeFile(const std::string& strLogPath);
 std::string GetDate();
 bool InitLogPath();

  
private:
 char    m_szErrMsg[MAX_ERR_MSG_SIZE];
 std::ofstream  m_ofsOutDebugFile;
 std::ofstream  m_ofsOutInfoFile;
 std::ofstream  m_ofsOutRunFile;
 std::ofstream  m_ofsOutWarnFile;
 std::ofstream  m_ofsOutErrorFile;
 std::ofstream  m_ofsOutEmergFile;
 std::ofstream  m_ofsOutFatalFile;

 std::string    m_strLogPath;
 std::string    m_strDebugLogPath;
 std::string    m_strInfoLogPath;
 std::string    m_strRunLogPath;
 std::string    m_strWarnLogPath;
 std::string    m_strErrorLogPath;
 std::string    m_strEmergLogPath;
 std::string    m_strFatalLogPath;

 LOG_LEVEL      m_dwLogLevel;
 uint64_t       m_ddwMaxLogSize;
 uint32_t       m_dwMaxLogFileNum;
 ted::CMutex    m_mutex;
};

// implementation
CLog::CLog(const std::string& strLogPath,
           const LOG_LEVEL dwLogLevel,
           const uint64_t ddwMaxLogSize,
           const uint32_T dwMaxLogNum):
          m_strLogPath(strLogPath),
          m_dwLogLevel(dwLogLevel),
          m_ddwMaxLogSize(ddwMaxLogSize),
          m_dwMaxLogFileNum(dwMaxLogNum)
{
  InitLogPath();
}

~CLog::CLog()
{
}

bool CLog::Init(const std::string& strLogPath,
                const LOG_LEVEL dwLogLevel,
                const uint64_t ddwMaxLogSize,
                const uint32_t dwMaxLogNum)
{
  m_strLogPath = strLogPath;
  m_dwLogLevel = dwLogLevel;
  m_ddwMaxLogSize = ddwMaxLogSize;
  m_dwMaxLogFileNum = dwMaxLogNum;
  InitLogPath();
  return true;
}

bool CLog::Write(const std::string& strFile,
            const std::string& strFunction,
            const uint32_t  dwLine,
            const char* strFormat,
            ...)
{
  return true;
}

bool CLog::ShiftAllFiles()
{
  return true;
}

bool CLog::ShiftOneTypeFile(const std::string& strLogPath)
{
  return true;
}

std::string CLog::GetDate()
{
  return (std::string(""));
}

bool CLog::InitLogPath()
{
  m_strDebugLogPath += "_debug.log;
  m_strInfoLogPath += "_info.log";
  m_strRunLogPath += "_run.log";
  m_strWarnLogPath += "_warn.log";
  m_strErrorLogPath += "_error.log";
  m_strEmergLogPath += "_emerg.log";
  m_strFatalLogPath += "_fatal.log;
  return true;
}

} // !namesapce ted
#endif // !_TED_LOG_H

