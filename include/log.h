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

#include "singleton.h"
#include "lock.h"

#include <stdint.h> // for uint32_t type define
#include <string>
#include <fstream>
#include <sstream>
#include <time.h> // for time() localtime_r()
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> // for stat
#include <string.h> // for strnlen
#include <stdarg.h> // for va_list,va_start,va_end

namespace ted {
	
class CLog;

/// define for use
typedef ted::CSingleton<ted::CLog, ted::CMutex> ted::TED_CLOG_SINGLETION;
#define TED_LOG ted::TED_CLOG_SINGLETION::Instance()

#define TED_LOG_DEBUG(fmt, args...) do {\
  TED_LOG->Write(__FILE__, __func__, __LINE__, ted::LOG_LEVEL_DEBUG, fmt, args); \
  TED_LOG->FlushByLevel(ted::LOG_LEVEL_DEBUG); \
}while(0)

#define TED_LOG_INFO(fmt, args...) do {\
  TED_LOG->Write(__FILE__, __func__, __LINE__, ted::LOG_LEVEL_INFO, fmt, args); \
  TED_LOG->FlushByLevel(ted::LOG_LEVEL_INFO); \
}while(0)

#define TED_LOG_RUN(fmt, args...) do {\
  TED_LOG->Write(__FILE__, __func__, __LINE__, ted::LOG_LEVEL_RUN, fmt, args); \
  TED_LOG->FlushByLevel(ted::LOG_LEVEL_RUN); \
}while(0)

#define TED_LOG_WARN(fmt, args...) do {\
  TED_LOG->Write(__FILE__, __func__, __LINE__, ted::LOG_LEVEL_WARN, fmt, args); \
  TED_LOG->FlushByLevel(ted::LOG_LEVEL_WARN); \
}while(0)

#define TED_LOG_ERROR(fmt, args...) do {\
  TED_LOG->Write(__FILE__, __func__, __LINE__, ted::LOG_LEVEL_ERROR, fmt, args); \
  TED_LOG->FlushByLevel(ted::LOG_LEVEL_ERROR); \
}while(0)

#define TED_LOG_EMERG(fmt, args...) do {\
  TED_LOG->Write(__FILE__, __func__, __LINE__, ted::LOG_LEVEL_EMERG, fmt, args); \
  TED_LOG->FlushByLevel(ted::LOG_LEVEL_EMERG); \
}while(0)

#define TED_LOG_FATAL(fmt, args...) do {\
  TED_LOG->Write(__FILE__, __func__, __LINE__, ted::LOG_LEVEL_FATAL, fmt, args); \
  TED_LOG->FlushByLevel(ted::LOG_LEVEL_FATAL); \
}while(0)

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

#define CHECK_RET( ret ) do {\
  if( !(ret) ) \
  {\
    return (ret);\
  }\
}while(0)

const uint32_t MAX_ERR_MSG_SIZE = 1024;
const uint32_t KB = 1024;
const uint32_t MB = 1024 * KB;
const uint32_t GB = 1024 * MB;
const uint64_t DEFAULT_LOG_SIZE = 10 * MB;
const uint32_t DEFAULT_LOG_FILE_NUM = 3;
const uint32_t MAX_BUFF_WRITE_SIZE = 10 * KB;

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
 CLog() { }
 CLog(const std::string& strLogPath,
      const std::string& strModuleName,
      const LOG_LEVEL dwLogLevel = LOG_LEVEL_ERROR,
      const uint64_t ddwMaxLogSize = DEFAULT_LOG_SIZE,
      const uint32_t dwMaxLogNum = DEFAULT_LOG_FILE_NUM);

 ~CLog();

public:
 inline bool Init(const std::string& strLogPath,
            const std::string& strModuleName,
            const LOG_LEVEL dwLogLevel = LOG_LEVEL_ERROR,
            const uint64_t ddwMaxLogSize = DEFAULT_LOG_SIZE,
            const uint32_t dwMaxLogNum = DEFAULT_LOG_FILE_NUM);

 inline const char * GetLastErrMsg() const   { return m_szErrMsg; }
 
 inline const LOG_LEVEL GetLogLevel() const  { return m_dwLogLevel; }
 
 inline void SetLogLevel(const LOG_LEVEL dwLogLevel) { this->m_dwLogLevel = dwLogLevel; }

 bool Write(const std::string& strFile,
            const std::string& strFunction,
            const uint32_t  dwLine,
            const LOG_LEVEL dwLogLevel,
            const char* strFormat,
            ...);
            
 void FlushByLevel(const LOG_LEVEL dwLogLevel);
 
 void FlushAll();

private:
 bool ShiftOneTypeFile(std::ofstream& ofsOutFile,
                       const std::string& strLogPath);
 bool InitLogPath();
 
 size_t GetFileSize(const std::string& strFilePath);
 
 void GetLogFilePathByIndex(const std::string& strLogPath,
                            uint32_t dwIndex,
                            std::string& strNewPath);
                            
 template <typename T>
 std::string ToString(const T& v);

 bool ProcessWrite(std::ofstream& ofsOutFile,
                  const std::string& strLogPath,
                  const std::string& strMsg);
                  
 bool WriteBase(const std::string& strMsg, 
            const LOG_LEVEL dwLogLevel);
            
 std::string GetDate();

 const char* GetErrStrByLevel(const LOG_LEVEL dwLogLevel) const;
  
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
 std::string    m_strModuleName;
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
           const std::string& strModuleName,
           const LOG_LEVEL dwLogLevel,
           const uint64_t ddwMaxLogSize,
           const uint32_t dwMaxLogNum):
          m_strLogPath(strLogPath),
          m_strModuleName(strModuleName),
          m_dwLogLevel(dwLogLevel),
          m_ddwMaxLogSize(ddwMaxLogSize),
          m_dwMaxLogFileNum(dwMaxLogNum)
{
  InitLogPath();
}

CLog::~CLog()
{
}

bool CLog::Init(const std::string& strLogPath,
                const std::string& strModuleName,
                const LOG_LEVEL dwLogLevel,
                const uint64_t ddwMaxLogSize,
                const uint32_t dwMaxLogNum)
{
  m_strLogPath = strLogPath;
  m_strModuleName = strModuleName;
  m_dwLogLevel = dwLogLevel;
  m_ddwMaxLogSize = ddwMaxLogSize;
  m_dwMaxLogFileNum = dwMaxLogNum;
  InitLogPath();
  return true;
}

bool CLog::Write(const std::string& strFile,
            const std::string& strFunction,
            const uint32_t  dwLine,
            const LOG_LEVEL dwLogLevel,
            const char* strFormat,
            ...)
{
  char szBuff[MAX_BUFF_WRITE_SIZE] = { '\0' };
  size_t dwBuffLen = 0;

  // write date
  std::string strDate = this->GetDate();
  snprintf(szBuff, sizeof(szBuff),"[%s] ", strDate.c_str());
  const char* pLogErrMsg = GetErrStrByLevel(dwLogLevel);

  dwBuffLen = strnlen(szBuff, sizeof(szBuff));
  snprintf(szBuff + dwBuffLen, sizeof(szBuff) - dwBuffLen, "[%s] [%s:%u] [%s] ",
           pLogErrMsg, strFile.c_str(), dwLine, strFunction.c_str());

  dwBuffLen = strnlen(szBuff, sizeof(szBuff));

  va_list ap;
  va_start(ap, strFormat);
  int nLen = vsnprintf(szBuff + dwBuffLen, sizeof(szBuff) - dwBuffLen, strFormat, ap);
  va_end(ap);
  if( nLen > 0 && nLen < ( sizeof(szBuffLen) - dwBuffLen ))
  {
  	szBuff[nLen] = '\n';
  }

  return WriteBase(szBuff, dwLogLevel);
}

bool CLog::ShiftOneTypeFile(std::ofstream& ofsOutFile,
                            const std::string& strLogPath)
{
  if( GetFileSize(strLogPath) >= m_ddwMaxLogSize )
  {
    if( ofsOutFile.is_open() )
    {
      ofsOutFile.close();
    }

    // remove last log file
    uint32_t dwLastLogFileIndex = m_dwMaxLogFileNum - 1;
    std::string strLastLogFilePath;
    GetLogFilePathByIndex(strLogPath, dwLastLogFileIndex, strLastLogFilePath);
    if( access(strLastLogFilePath.c_str(), F_OK ) == 0 )
    {
      if( remove(strLastLogFilePath.c_str()) < 0 )
      {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "remove File:%s failed",
                 strLastLogFilePath.c_str());
        return false;
      }
    }

    for(int i = m_dwMaxLogFileNum - 2; i >= 0; --i)
    {
      std::string strOldLogFilePath;
      GetLogFilePathByIndex(strLogPath, static_cast<uint32_t>(i), strOldLogFilePath);
      if( access(strOldLogFilePath.c_str(), F_OK)  == 0 )
      {
        std::string strNewLogFilePath;
        GetLogFilePathByIndex(strLogPath, static_cast<uint32_t>(i) + 1, strNewLogFilePath);
        if( rename(strOldLogFilePath.c_str(), strNewLogFilePath.c_str() ) < 0 )
        {
          snprintf(m_szErrMsg, sizeof(m_szErrMsg), "rename old name:%s to new name:%s failed",
                   strOldLogFilePath.c_str(), strNewLogFilePath.c_str());
          return false;
        }
      }
    }
  }
  return true;
}

bool CLog::ProcessWrite(std::ofstream& ofsOutFile,
                        const std::string& strLogPath,
                        const std::string& strMsg)
{
  if( !ofsOutFile.is_open() )
  {
    ofsOutFile.open(strLogPath.c_str(), std::ios::out | std::ios::app);
  }
  if( ofsOutFile.fail() )
  {
    snprintf(m_szErrMsg, sizeof(m_szErrMsg), "Fail open %s", strLogPath.c_str());
    return false;
  }
  if( !ShiftOneTypeFile(ofsOutFile,strLogPath) )
  {
    return false;
  }
  if( !ofsOutFile.is_open() )
  {
    ofsOutFile.open(strLogPath.c_str(), std::ios::out | std::ios::app);
  }
  if( ofsOutFile.fail() )
  {
    snprintf(m_szErrMsg, sizeof(m_szErrMsg), "Fail open %s", strLogPath.c_str());
    return false;
  }

  ted::CAutoLock lock(m_mutex);
  ofsOutFile << strMsg;

  return true;
}

bool CLog::WriteBase(const std::string& strMsg,
                 const LOG_LEVEL dwLogLevel)
{
  bool bSuc = false;
  if( dwLogLevel > LOG_LEVEL_OFF )
  {
    if( dwLogLevel >= LOG_LEVEL_FATAL )
    {
      bSuc = ProcessWrite(m_ofsOutFatalFile, m_strFatalLogPath, strMsg);
      CHECK_RET(bSuc);
    }
    if( dwLogLevel >= LOG_LEVEL_EMERG )
    {
      bSuc = ProcessWrite(m_ofsOutEmergFile, m_strEmergLogPath, strMsg);
      CHECK_RET(bSuc);
    }
    if( dwLogLevel >= LOG_LEVEL_ERROR )
    {
      bSuc = ProcessWrite(m_ofsOutErrorFile, m_strErrorLogPath, strMsg);
      CHECK_RET(bSuc);
    }
    if( dwLogLevel >= LOG_LEVEL_WARN )
    {
      bSuc = ProcessWrite(m_ofsOutWarnFile, m_strWarnLogPath, strMsg);
      CHECK_RET(bSuc);
    }
    if( dwLogLevel >= LOG_LEVEL_RUN )
    {
      bSuc = ProcessWrite(m_ofsOutRunFile, m_strRunLogPath, strMsg);
      CHECK_RET(bSuc);
    }
    if( dwLogLevel >= LOG_LEVEL_INFO )
    {
      bSuc = ProcessWrite(m_ofsOutInfoFile, m_strInfoLogPath, strMsg);
      CHECK_RET(bSuc);
    }
    if( dwLogLevel >= LOG_LEVEL_DEBUG )
    {
      bSuc = ProcessWrite(m_ofsOutDebugFile, m_strDebugLogPath, strMsg);
      CHECK_RET(bSuc);
    }
  }

  return true;
}

std::string CLog::GetDate()
{
  time_t dwTime = time(NULL);
  struct tm stTm;
  localtime_r(&dwTime, &stTm);
  char szBuff[64] = { 0 };
  snprintf(szBuff, sizeof(szBuff) - 1, "%04d-%02d-%02d %02d:%02d:%02d",
           stTm.tm_year + 1900, stTm.tm_mon + 1, stTm.tm_mday, stTm.tm_hour, stTm.tm_min, stTm.tm_sec);
  return std::string(szBuff);
}

bool CLog::InitLogPath()
{
  m_strDebugLogPath =  m_strLogPath + m_strModuleName +  "_debug.log";
  m_strInfoLogPath =  m_strLogPath + m_strModuleName +  "_info.log";
  m_strRunLogPath =  m_strLogPath + m_strModuleName +  "_run.log";
  m_strWarnLogPath =  m_strLogPath + m_strModuleName +  "_warn.log";
  m_strErrorLogPath =  m_strLogPath + m_strModuleName +  "_error.log";
  m_strEmergLogPath =  m_strLogPath + m_strModuleName +  "_emerg.log";
  m_strFatalLogPath =  m_strLogPath + m_strModuleName +  "_fatal.log";
  return true;
}

size_t CLog::GetFileSize(const std::string& strFilePath)
{
  struct stat stStat;
  if( stat(strFilePath.c_str(), &stStat) >= 0 )
  {
    return stStat.st_size;
  }
  return 0ul;
}

void CLog::GetLogFilePathByIndex(const std::string& strLogPath,
                          uint32_t dwIndex,
                          std::string& strNewPath)
{
  if( 0ul == dwIndex )
  {
    strNewPath = strLogPath;
  }
  else
  {
    size_t dwPos = strLogPath.find_last_of(".");
    if( dwPos != std::string::npos )
    {
      strNewPath = strLogPath.substr(0, dwPos ) + ToString(dwIndex) + ".log";
    }
  }
}

const char* CLog::GetErrStrByLevel(const LOG_LEVEL dwLogLevel)
{
  switch(dwLogLevel)
  {
    case LOG_LEVEL_FATAL:
      return "fatal";
    case LOG_LEVEL_EMERG:
      return "emerg";
    case LOG_LEVEL_ERROR:
      return "error";
    case LOG_LEVEL_WARN:
      return "warn";
    case LOG_LEVEL_RUN:
      return "run";
    case LOG_LEVEL_INFO:
      return "info";
    case LOG_LEVEL_DEBUG:
      return "debug";
    case LOG_LEVEL_ALL:
      return "all";
    default:
      return "unkown level";
  }
  return "unkown level";
}

template <typename T>
std::string CLog::ToString(const T& v)
{
  std::stringstream oss;
  oss << v;
  return oss.str();
}

void CLog::FlushByLevel(const LOG_LEVEL dwLogLevel) const
{
  switch(dwLogLevel)
  {
        case LOG_LEVEL_FATAL:
		FILE_SAFE_FLUSH(m_ofsOutFatalFile);
		break;
	case LOG_LEVEL_EMERG:
		FILE_SAFE_FLUSH(m_ofsOutEmergFile);
		break;
	case LOG_LEVEL_ERROR:
		FILE_SAFE_FLUSH(m_ofsOutErrorFile);
		break;
	case LOG_LEVEL_WARN:
		FILE_SAFE_FLUSH(m_ofsOutWarnFile);
		break;
	case LOG_LEVEL_RUN:
		FILE_SAFE_FLUSH(m_ofsOutRunFile);
		break;
	case LOG_LEVEL_INFO:
		FILE_SAFE_FLUSH(m_ofsOutInfoFile);
		break;
	case LOG_LEVEL_DEBUG:
		FILE_SAFE_FLUSH(m_ofsOutDebugFile);
		break;
	default:
		break;
  }
}

void CLog::FlushAll()
{
  FILE_SAFE_FLUSH(m_ofsOutFatalFile);
  FILE_SAFE_FLUSH(m_ofsOutEmergFile);
  FILE_SAFE_FLUSH(m_ofsOutErrorFile);
  FILE_SAFE_FLUSH(m_ofsOutWarnFile);
  FILE_SAFE_FLUSH(m_ofsOutInfoFile);
  FILE_SAFE_FLUSH(m_ofsOutRunFile);
  FILE_SAFE_FLUSH(m_ofsOutDebugFile);
}

} // !namesapce ted
#endif // !_TED_LOG_H

