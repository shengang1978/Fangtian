//******************************************************************************
//*
//* FileName		: Functions.h
//* Description		: ͨ�ú�����
//* Author			: 2011.09.09 James.Sun
//*
//******************************************************************************
//---------
// �޸���ʷ
//---------
// v001 2011.09.09 James.Sun 
//------------------------------------------------------------------------------
#pragma once
#include <tchar.h>
#include <windows.h>

class Functions
{
public:
	// Ŀ¼�ຯ��
	static bool IsExists( const char *pFilePath );
	static bool MakeDir( const char *pPath );
	//static bool GetExeFilePath( char *pFilePath );

	// ʱ��ת������
	static bool LongToTime( unsigned long dat , SYSTEMTIME *pSystemTime ) ;
	static int TimeToLong(  SYSTEMTIME *pSystemTime  ) ; 
	//static bool StringToDateTime( const char *pTimeString, SYSTEMTIME *pSystemTime );
	static __int64 SystemTimeToInt64( SYSTEMTIME *pSystemTime );

	static void YUVToRGB24( unsigned char *puc_y, int stride_y, unsigned char *puc_u, unsigned char *puc_v, int stride_uv, 
							unsigned char *puc_out, int width_y, int height_y );
};
