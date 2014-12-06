#pragma once
#include <stdio.h>
#include <windows.h>

#define TRACE_DEBUG		HHVLog::Debug
#define TRACE_INFO		HHVLog::Info
#define TRACE_WARN		HHVLog::Warn
#define TRACE_ERROR		HHVLog::Error
#define TRACE_TEST		HHVLog::Test

#define LOG_LEVEL_DEBUG		0
#define LOG_LEVEL_INFO		1
#define LOG_LEVEL_WARN		2
#define LOG_LEVEL_ERROR		3

#define MAX_LOG_FILE_LENGTH		(8*1024*1024)

class HHVLog
{
public:
	HHVLog(void);
	~HHVLog(void);
	static void SetLogLevel(int iLogLevel, bool bPrint = true);
	static void Debug(const char *pMessage, ...);
	static void Info(const char *pMessage, ...);
	static void Warn(const char *pMessage, ...);
	static void Error(const char *pMessage, ...);
	static void Test(const char *pMessage, ...);

private:
	static void Init();
	static void UnInit();
	static void CreateLogFile();
	static void WriteLogFile( const char *pMessage );
	static void Log(const char *pMessage);

	static bool m_bInit;
	static bool m_bPrint;
	static int m_iLogLevel;
	static FILE* m_pFile;	
	static HANDLE m_hMutx_File;
};
