#include "HHVLog.h"
#include "tchar.h"

bool HHVLog::m_bInit = false;
bool HHVLog::m_bPrint = false;
int HHVLog::m_iLogLevel = LOG_LEVEL_INFO;
FILE* HHVLog::m_pFile  = NULL;
HANDLE HHVLog::m_hMutx_File = CreateMutex( NULL, FALSE, NULL );;

HHVLog::HHVLog(void)
{
}

HHVLog::~HHVLog(void)
{
	UnInit();
	CloseHandle( m_hMutx_File );
}
void HHVLog::SetLogLevel(int iLogLevel, bool bPrint)
{
	m_iLogLevel = iLogLevel;
	m_bPrint = bPrint;
}
void HHVLog::Init()
{
	CreateLogFile();
	m_bInit = true;
}
void HHVLog::UnInit()
{
	if( m_pFile != NULL )
		fclose(m_pFile ); 
	m_bInit = false;
}
void HHVLog::CreateLogFile()
{
	TCHAR szPath[256] = {0X00};
	TCHAR szFilePath[256] = {0X00};
	::GetModuleFileName(NULL,(TCHAR *)szPath,sizeof(szPath));
	for(int i=_tcslen(szPath)-1;i>0;i--)
	{
		if(szPath[i]=='\\')
		{
			_tcsncpy( szFilePath, szPath, i+1 );
		}
	}

	_tcscat( szFilePath, _T("LOG_HHV\\"));
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = 1;	
	::CreateDirectory( szFilePath, &sa );

	SYSTEMTIME	systime;
	GetLocalTime(&systime);

	TCHAR buf[256] = {0x00};
	_stprintf( buf, _T("%04d%02d%02d%02d%02d%02d%03d.log"), systime.wYear, systime.wMonth, systime.wDay, 
													systime.wHour, systime.wMinute, systime.wSecond,
													systime.wMilliseconds);
	
	_tcscat(szFilePath, buf);

	m_pFile = _tfopen( szFilePath, _T("w+b"));
}
void HHVLog::WriteLogFile(const char *pMessage)
{
	fputs( pMessage, m_pFile );
	fflush( m_pFile );	
}
void HHVLog::Log(const char *pMessage)
{
	if(pMessage == NULL) return;
	if ( WAIT_OBJECT_0 == WaitForSingleObject( m_hMutx_File, 1000 ) )
	{
		try
		{
			if( !m_bInit )
			{
				Init();
				m_bInit = true;
			}
			WriteLogFile( pMessage );
			if( ftell(m_pFile) >= MAX_LOG_FILE_LENGTH )
				UnInit();
		}
		catch(...)
		{ 

		}
		ReleaseMutex( m_hMutx_File );
	}
}
void HHVLog::Debug(const char *pMessage, ...)
{
	if(m_iLogLevel <= LOG_LEVEL_DEBUG)
	{
		char msg[1024] = {0x00};
		char szMessage[1024] = {0x00};

		va_list argPtr;	
		va_start(argPtr, pMessage);   
		vsprintf(msg, pMessage, argPtr);  
		va_end(argPtr);

		SYSTEMTIME systime;
		GetLocalTime(&systime);
		sprintf( szMessage, "%04d-%02d-%02d %02d:%02d:%02d.%03d Debug %s",systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds, msg);

		Log(szMessage);
	}
}
void HHVLog::Info(const char *pMessage, ...)
{
	if(m_iLogLevel <= LOG_LEVEL_INFO)
	{
		char msg[1024] = {0x00};
		char szMessage[1024] = {0x00};

		va_list argPtr;	
		va_start(argPtr, pMessage);   
		vsprintf(msg, pMessage, argPtr);  
		va_end(argPtr);

		SYSTEMTIME systime;
		GetLocalTime(&systime);
		sprintf( szMessage, "%04d-%02d-%02d %02d:%02d:%02d.%03d Info  %s",systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds, msg);
		
		if(m_bPrint)
		{
			printf(szMessage);
		}
		Log(szMessage);
	}
}
void HHVLog::Warn(const char *pMessage, ...)
{
	if(m_iLogLevel <= LOG_LEVEL_WARN)
	{
		char msg[1024] = {0x00};
		char szMessage[1024] = {0x00};

		va_list argPtr;	
		va_start(argPtr, pMessage);   
		vsprintf(msg, pMessage, argPtr);  
		va_end(argPtr);

		SYSTEMTIME systime;
		GetLocalTime(&systime);
		sprintf( szMessage, "%04d-%02d-%02d %02d:%02d:%02d.%03d Warn  %s",systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds, msg);

		if(m_bPrint)
		{
			printf(szMessage);
		}
		Log(szMessage);
	}
}
void HHVLog::Error(const char *pMessage, ...)
{
	if(m_iLogLevel <= LOG_LEVEL_ERROR)
	{
		char msg[1024] = {0x00};
		char szMessage[1024] = {0x00};

		va_list argPtr;	
		va_start(argPtr, pMessage);   
		vsprintf(msg, pMessage, argPtr);  
		va_end(argPtr);

		SYSTEMTIME systime;
		GetLocalTime(&systime);
		sprintf( szMessage, "%04d-%02d-%02d %02d:%02d:%02d.%03d Error %s",systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds, msg);

		if(m_bPrint)
		{
			printf(szMessage);
		}
		Log(szMessage);
	}
}
void HHVLog::Test(const char *pMessage, ...)
{
	char msg[1024] = {0x00};
	char szMessage[1024] = {0x00};

	va_list argPtr;	
	va_start(argPtr, pMessage);   
	vsprintf(msg, pMessage, argPtr);  
	va_end(argPtr);

	SYSTEMTIME systime;
	GetLocalTime(&systime);
	sprintf( szMessage, "%04d-%02d-%02d %02d:%02d:%02d.%03d Test  %s",systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds, msg);

	if(m_bPrint)
	{
		printf(szMessage);
	}
	Log(szMessage);
}