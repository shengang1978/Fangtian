#pragma once
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#ifndef _WIN32_WINNT            // 指定要求的最低平台是 Windows Vista。
#define _WIN32_WINNT 0x0600     // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif
// Windows 头文件:
#include <windows.h>

#include "HCNetSDK.h"
#include "plaympeg4.h"
//#include "AnalyzeDataNewInterface.h"

#include <cv.h>

class HikHCNetVs
{
public:
	HikHCNetVs(void);
	~HikHCNetVs(void);

	int Init();

	const IplImage *GetImage();

	static void __stdcall DVR_RealDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,void *pUser);
	static void __stdcall DVR_DecCallBack(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nUser,long nReserved2);
	static void __stdcall DVR_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
public:
	int m_nImageNum;
	int m_nGetImageCount;
private:
	long m_lUserID;	// 登陆ID
	long m_lRealPlayHandle;

	HANDLE m_hMutx_Image;
	HANDLE m_hEvent_HasImage;

	int m_iImageWidth;
	int m_iImageHeigth;

	IplImage *m_pIplImage; 

	unsigned char g_szYUVImageBuffer[4096*4096*3];
	unsigned char g_szRGBImageBuffer[4096*4096*3];
};
