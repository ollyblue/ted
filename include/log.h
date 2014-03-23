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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

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

 inline const char * GetLastErrMsg() { return m_szErrMsg; }
 inline const LOG_LEVEL GetLogLevel() { return m_dwLogLevel; }
 inline void SetLogLevel(const LOG_LEVEL dwLogLevel) { this->m_dwLogLevel = dwLogLevel; }

 bool Write(const std::string& strFile,
            const std::string& strFunction,
            const uint32_t  dwLine,
            const LOG_LEVEL dwLogLevel,
            const char* strFormat,
            ...);

private:
 bool ShiftAllFiles();
 bool ShiftOneTypeFile(std::ofstream& ofsOutFile,
                       const std::string& strLogPath);
 bool InitLogPath();
 size_t GetFileSize(const std::string& strFilePath);
 void GetLogFilePathByIndex(const std::string& strLogPath,
                            uint32_t dwIndex,
                            std::string& strNewPath);
 template <typename T>
 std::string ToString(const T& v);
public:
 bool ProcessWrite(std::ofstream& ofsOutFile,
                  const std::string& strLogPath,
                  const std::string& strMsg);
 bool WriteBase(const std::string& strMsg, 
            const LOG_LEVEL dwLogLevel);
 std::string GetDate();

  
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
  return true;
}

bool CLog::ShiftAllFiles()
{
  return true;
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

template <typename T>
std::string CLog::ToString(const T& v)
{
  std::stringstream oss;
  oss << v;
  return oss.str();
}

} // !namesapce ted
#endif // !_TED_LOG_H

