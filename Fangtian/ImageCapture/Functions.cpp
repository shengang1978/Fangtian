//******************************************************************************
//*
//* FileName		: Functions.cpp
//* Description		: 通用函数类
//* Author			: 2011.09.09 James.Sun
//*
//******************************************************************************
//---------
// 修改历史
//---------
// v001 2011.09.09 James.Sun 
//------------------------------------------------------------------------------
#include <io.h>
#include <iostream>
#include <direct.h>
#include "Functions.h"

//******************************************************************************
//*                                                                            *
//* FunctionName    : IsExists                                                 *
//* Description     : 判断目录或文件是否存在                                   *
//* Parameters      : *pFilePath 目录或文件路径                                *
//* Return          : true 存在  flase 不存在                                  *
//* Data & Author   : 2011.09.09 James.Sun                                     *
//*                                                                            *
//******************************************************************************
bool Functions::IsExists(const char *pFilePath)
{
    if((_access(pFilePath, 0)) == -1)
	{
		return false;
	}
	return true;
}

//******************************************************************************
//*                                                                            *
//* FunctionName    : MakeDir                                                  *
//* Description     : 创建目录                                                 *
//* Parameters      : *pPath 目录路径                                          *
//* Return          : true 成功  flase 不成功                                  *
//* Data & Author   : 2011.09.09 James.Sun                                     *
//*                                                                            *
//******************************************************************************
bool Functions::MakeDir(const char *pPath)
{
	std::string str = pPath; 
	int index = 0; 
	while(1) 
	{ 
		std::string::size_type pos = str.find( "/",index);
		if( pos == -1 )
		{
			pos = str.find( "\\",index);
		}
		
		if( pos != -1 ) 
		{ 
			std::string str1; 
			str1 = str.substr(0, pos); 
			if(IsExists(str1.c_str()) == false)
			{
				_mkdir(str1.c_str()); 
			}
		} 
		else
		{ 
			break; 
		} 
		index = pos + 1; 
	} 
	return true;
}

//******************************************************************************
//*                                                                            *
//* FunctionName    : StringToDateTime                                         *
//* Description     : 时间字符串转换为时间                                     *
//* Parameters      : *pTimeString 时间字符串                                  *
//*                 : *pSysTime 输出时间                                       *
//* Return          : true 成功  flase 不成功                                  *
//* Data & Author   : 2011.09.09 James.Sun                                     *
//*                                                                            *
//******************************************************************************
//bool Functions::StringToDateTime(const char *pTimeString, SYSTEMTIME *pSysTime)
//{    
//	if (!pTimeString)    
//	{        
//		return FALSE;    
//	}
//
//	int nYear   = 0;    
//	int nMonth  = 0;    
//	int nDay    = 0;    
//	int nHour   = 0;    
//	int nSecond = 0;    
//	int nMinute = 0;    
//
//	// 转换
//	_stscanf(pTimeString, _T("%4d%2d%2d%2d%2d%2d"), &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);    
//  
//	// 赋值
//	pSysTime->wYear   = nYear;    
//	pSysTime->wMonth  = nMonth;    
//	pSysTime->wDay    = nDay;    
//	pSysTime->wHour   = nHour;    
//	pSysTime->wSecond = nSecond;    
//	pSysTime->wMinute = nMinute; 
//
//	return true;
//}

//******************************************************************************
//*                                                                            *
//* FunctionName    : GetExeFilePath                                           *
//* Description     : 获取当前程序路径                                         *
//* Parameters      : *pFilePath 输出当前程序路径                              *
//* Return          : true 成功  flase 不成功                                  *
//* Data & Author   : 2011.09.09 James.Sun                                     *
//*                                                                            *
//******************************************************************************
//bool Functions::GetExeFilePath(char *pFilePath)
//{
//	char szPath[_MAX_PATH] = {0X00};
//	::GetModuleFileNameA(NULL,(char*)szPath,sizeof(szPath));
//	for(int i=strlen(szPath)-1;i>0;i--)
//	{
//		if(szPath[i]=='\\')
//		{
//			strncpy( pFilePath, szPath, i+1 );
//			return true;
//		}
//	}
//	return false;
//}

//******************************************************************************
//*                                                                            *
//* FunctionName    : LongToTime                                               *
//* Description     : 整形值转时间                                             *
//* Parameters      : dat 输入整形值                                           *
//*                 : *pSystemTime 输出时间                                    *
//* Return          : true 成功  flase 不成功                                  *
//* Data & Author   : 2011.09.09 James.Sun                                     *
//*                                                                            *
//******************************************************************************
bool Functions::LongToTime( unsigned long dat , SYSTEMTIME *pSystemTime )
{
	int  days[13] = { 0, 31 , 29 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 } ;
    int  i ;
	
    pSystemTime->wSecond	= WORD(dat % 60) ;
    dat					= dat / 60 ;        //dat is minute
    
    pSystemTime->wMinute = WORD(dat % 60) ;
    dat          = dat / 60 ;        //dat is hour  
    
    pSystemTime->wHour   = WORD(dat % 24) ;
    dat          = dat / 24 ;        //dat is day   
	
    i            = 0    ;	
	
	bool bLeapYear = true; //从2000.1.1日开始
	while( dat >= (DWORD)( bLeapYear ? 366 :365 ))// 存在闰年问题
    {
        if( bLeapYear )
            dat -= 366 ;
        else
            dat -= 365 ;
        i++ ;
		bLeapYear = (2000+i)%400==0 || ( (2000+i)%4==0 && (2000+i)%100!=0 ) ;
    }
    
    pSystemTime->wYear  = 2000 + i ;
    if( !bLeapYear ) 
        days[2] = 28 ;
    
    for( i = 1 ; i < 13 ; i++ )
    {
        if( dat < (DWORD)days[i] )	
            break;
        dat -= days[i] ;    
    }
    pSystemTime->wMonth  =  i  ;
    pSystemTime->wDay    = (WORD)(dat + 1) ;
	return true;
}

//******************************************************************************
//*                                                                            *
//* FunctionName    : TimeToLong                                               *
//* Description     : 时间转整形值                                             *
//* Parameters      : *pSystemTime 输入时间                                    *
//* Return          : <=0 转换失败  >0 时间整形值                              *
//* Data & Author   : 2011.09.09 James.Sun                                     *
//*                                                                            *
//******************************************************************************
int Functions::TimeToLong( SYSTEMTIME *pSystemTime )
{
	if( pSystemTime->wYear > 2100 )
	{
		return 0;
	}
	int  days[13] = { 0, 31 , 29 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 } ;
    int  day  , i ;
    unsigned long  seconds ;
    
    seconds =    0 ;
    day     =    0 ;
    try
	{
		bool bLeapYear = pSystemTime->wYear%400 == 0 || ( pSystemTime->wYear%4 == 0 && pSystemTime->wYear%100 != 0 ) ;
		if( !bLeapYear )
			days[2] = 28 ;
	    
		for( i = 0 ; i < ( pSystemTime->wYear - 2000 ) ; i++ )
		{
			bLeapYear = (2000+i)%400 == 0 || ( (2000+i)%4 == 0 && (2000+i)%100 != 0 ) ;
			if( bLeapYear )
				day += 366 ;
			else
				day += 365 ;
		}    
		for( i = 1 ; i < pSystemTime->wMonth ; i++ )  
		{
			day += days[i] ;
		}       
		day += ( pSystemTime->wDay - 1 );     
	    
		seconds = ( day * 24 + pSystemTime->wHour )*3600 + pSystemTime->wMinute * 60 + pSystemTime->wSecond ;
	}
	catch(...)
	{
		return 0;
	}
    return seconds ;    
}

//******************************************************************************
//*                                                                            *
//* FunctionName    : SystemTimeToInt64                                        *
//* Description     : 时间转整形毫秒值                                         *
//* Parameters      : *pSystemTime 输入时间                                    *
//* Return          : <=0 转换失败  >0 时间整形毫秒值                          *
//* Data & Author   : 2011.09.09 James.Sun                                     *
//*                                                                            *
//******************************************************************************
__int64 Functions::SystemTimeToInt64( SYSTEMTIME *pSystemTime )
{
	if( pSystemTime->wYear > 2100 || pSystemTime->wYear < 2000 || 
		pSystemTime->wMonth > 12 || pSystemTime->wMonth < 1 || 
		pSystemTime->wDay > 31 || pSystemTime->wDay < 1 || 
		pSystemTime->wHour > 23 || pSystemTime->wMinute > 59 || 
		pSystemTime->wSecond > 59 || pSystemTime->wMilliseconds > 999)
	{
		return -1;
	}

	__int64 iSystemTime = 252427881600000;//2000.1.1 0:0:0 的毫秒数
	int days[13] = { 0, 31 , 28 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 } ;

	if(pSystemTime->wYear%400 == 0 || ( pSystemTime->wYear%4 == 0 && pSystemTime->wYear%100 != 0 ))
	{
		days[2] = 28;
	}

	for(int i=2000;i<pSystemTime->wYear;i++)
	{
		if(i%400 == 0 || ( i%4 == 0 && i%100 != 0 ))
		{
			iSystemTime += (__int64)366 * 86400000; //24*60*60*1000=86400000
		}
		else
		{
			iSystemTime += (__int64)365 * 86400000;
		}
	}

	for(int i=1;i<pSystemTime->wMonth;i++)
	{
		iSystemTime += days[i] * (__int64)86400000;
	}

	for(int i=1; i<pSystemTime->wDay; i++)
	{
		iSystemTime += (__int64)86400000;
	}
	
	iSystemTime += pSystemTime->wHour * (__int64)3600000;
	iSystemTime += pSystemTime->wMinute * (__int64)60000;
	iSystemTime += pSystemTime->wSecond * (__int64)1000 + pSystemTime->wMilliseconds;

	return iSystemTime;
}

//******************************************************************************
//*                                                                            *
//* FunctionName    : YUVToRGB24                                               *
//* Description     : YUV转RGB24                                               *
//* Parameters      : (pY,width,pU,pV,width>>1,pRGBBuf,width,(int)0-height)    *
//* Return          : 无                                                       *
//* Data & Author   : 2011.09.09 James.Sun                                     *
//*                                                                            *
//******************************************************************************
#define MAXIMUM_Y_WIDTH 4096
static unsigned __int64 mmw_mult_Y    = 0x2568256825682568;
static unsigned __int64 mmw_mult_U_G  = 0xf36ef36ef36ef36e;
static unsigned __int64 mmw_mult_U_B  = 0x40cf40cf40cf40cf;
static unsigned __int64 mmw_mult_V_R  = 0x3343334333433343;
static unsigned __int64 mmw_mult_V_G  = 0xe5e2e5e2e5e2e5e2;
static unsigned __int64 mmb_0x10      = 0x1010101010101010;
static unsigned __int64 mmw_0x0080    = 0x0080008000800080;
static unsigned __int64 mmw_0x00ff    = 0x00ff00ff00ff00ff;
static unsigned __int64 mmw_cut_red   = 0x7c007c007c007c00;
static unsigned __int64 mmw_cut_green = 0x03e003e003e003e0;
static unsigned __int64 mmw_cut_blue  = 0x001f001f001f001f;
void Functions::YUVToRGB24(unsigned char *puc_y, int stride_y, unsigned char *puc_u, unsigned char *puc_v, int stride_uv, 
				unsigned char *puc_out, int width_y, int height_y) 
{
	int y, horiz_count;
	unsigned char *puc_out_remembered;
	int stride_out = width_y * 3;
    
	if (height_y < 0) {
		//we are flipping our output upside-down
		height_y  = -height_y;
		puc_y     += (height_y   - 1) * stride_y ;
		puc_u     += (height_y/2 - 1) * stride_uv;
		puc_v     += (height_y/2 - 1) * stride_uv;
		stride_y  = -stride_y;
		stride_uv = -stride_uv;
	}

	horiz_count = -(width_y >> 3);

	for (y=0; y<height_y; y++) {
		if (y == height_y-1) {
			//this is the last output line - we need to be careful not to overrun the end of this line
			unsigned char temp_buff[3*MAXIMUM_Y_WIDTH+1];
			puc_out_remembered = puc_out;
			puc_out = temp_buff; //write the RGB to a temporary store
		}
		_asm {
			push eax
			push ebx
			push ecx
			push edx
			push edi

			mov eax, puc_out       
			mov ebx, puc_y       
			mov ecx, puc_u       
			mov edx, puc_v
			mov edi, horiz_count

			horiz_loop:
			movd mm2, [ecx]
			pxor mm7, mm7
			movd mm3, [edx]
			punpcklbw mm2, mm7       
			movq mm0, [ebx]          
			punpcklbw mm3, mm7       
			movq mm1, mmw_0x00ff     
			psubusb mm0, mmb_0x10    
			psubw mm2, mmw_0x0080    
			pand mm1, mm0            
			psubw mm3, mmw_0x0080    
			psllw mm1, 3             
			psrlw mm0, 8             
			psllw mm2, 3             
			pmulhw mm1, mmw_mult_Y   
			psllw mm0, 3             
			psllw mm3, 3             
			movq mm5, mm3            
			pmulhw mm5, mmw_mult_V_R 
			movq mm4, mm2            
			pmulhw mm0, mmw_mult_Y   
			movq mm7, mm1            
			pmulhw mm2, mmw_mult_U_G 
			paddsw mm7, mm5
			pmulhw mm3, mmw_mult_V_G
			packuswb mm7, mm7
			pmulhw mm4, mmw_mult_U_B
			paddsw mm5, mm0      
			packuswb mm5, mm5
			paddsw mm2, mm3          
			movq mm3, mm1            
			movq mm6, mm1            
			paddsw mm3, mm4
			paddsw mm6, mm2
			punpcklbw mm7, mm5
			paddsw mm2, mm0
			packuswb mm6, mm6
			packuswb mm2, mm2
			packuswb mm3, mm3
			paddsw mm4, mm0
			packuswb mm4, mm4
			punpcklbw mm6, mm2
			punpcklbw mm3, mm4

			// 32-bit shuffle.
			pxor mm0, mm0
			movq mm1, mm6
			punpcklbw mm1, mm0
			movq mm0, mm3
			punpcklbw mm0, mm7
			movq mm2, mm0
			punpcklbw mm0, mm1
			punpckhbw mm2, mm1

			// 24-bit shuffle and sav
			movd   [eax], mm0
			psrlq mm0, 32
			movd  3[eax], mm0
			movd  6[eax], mm2
			psrlq mm2, 32            
			movd  9[eax], mm2        

			// 32-bit shuffle.
			pxor mm0, mm0            
			movq mm1, mm6            
			punpckhbw mm1, mm0       
			movq mm0, mm3            
			punpckhbw mm0, mm7       
			movq mm2, mm0
			punpcklbw mm0, mm1       
			punpckhbw mm2, mm1

			// 24-bit shuffle and sav
			movd 12[eax], mm0        
			psrlq mm0, 32            
			movd 15[eax], mm0        
			add ebx, 8               
			movd 18[eax], mm2        
			psrlq mm2, 32            
			add ecx, 4               
			add edx, 4               
			movd 21[eax], mm2        
			add eax, 24              
			inc edi
			jne horiz_loop

			pop edi
			pop edx
			pop ecx
			pop ebx
			pop eax

			emms
		}

		if (y == height_y-1) {
			//last line of output - we have used the temp_buff and need to copy
			int x = 3 * width_y;                  //interation counter
			unsigned char *ps = puc_out;                // source pointer (temporary line store)
			unsigned char *pd = puc_out_remembered;     // dest pointer
			while (x--) *(pd++) = *(ps++);          // copy the line
		}

		puc_y   += stride_y;
		if (y%2) {
			puc_u   += stride_uv;
			puc_v   += stride_uv;
		}
		puc_out += stride_out; 
	}
}