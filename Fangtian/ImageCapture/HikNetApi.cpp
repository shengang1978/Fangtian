
#include "HikHCNetVs.h"
#include "HikNetApi.h"

HikHCNetVs *g_pHikHCNetVs = NULL;
bool g_bInitSuccess = false;

__declspec(dllexport) int __stdcall HikNetApi_Init()
{
	int iRet = 0;

	g_pHikHCNetVs = new HikHCNetVs();

	iRet = g_pHikHCNetVs->Init();

	if(iRet == 0)
	{
		g_bInitSuccess=true;
	}

	return iRet;
}
__declspec(dllexport) const IplImage* __stdcall HikNetApi_GetImage()
{
	if(g_bInitSuccess)
	{
		return g_pHikHCNetVs->GetImage();
	}
	return NULL;
}