//******************************************************************************
//*
//* FileName		: Functions.h
//* Description		: 通用函数类
//* Author			: 2011.09.09 James.Sun
//*
//******************************************************************************
//---------
// 修改历史
//---------
// v001 2011.09.09 James.Sun 
//------------------------------------------------------------------------------
#pragma once
#include <tchar.h>
#include <windows.h>

class Functions
{
public:
	// 目录类函数
	static bool IsExists( const char *pFilePath );
	static bool MakeDir( const char *pPath );
	//static bool GetExeFilePath( char *pFilePath );

	// 时间转化函数
	static bool LongToTime( unsigned long dat , SYSTEMTIME *pSystemTime ) ;
	static int TimeToLong(  SYSTEMTIME *pSystemTime  ) ; 
	//static bool StringToDateTime( const char *pTimeString, SYSTEMTIME *pSystemTime );
	static __int64 SystemTimeToInt64( SYSTEMTIME *pSystemTime );

	static void YUVToRGB24( unsigned char *puc_y, int stride_y, unsigned char *puc_u, unsigned char *puc_v, int stride_uv, 
							unsigned char *puc_out, int width_y, int height_y );
};
