#include "HikHCNetVs.h"
#include "HHVLog.h"
#include "Functions.h"

//unsigned char g_szYUVImageBuffer[4096*4096*3] = {0x00};
//unsigned char g_szRGBImageBuffer[4096*4096*3] = {0x00};

HikHCNetVs::HikHCNetVs(void)
{
	m_hEvent_HasImage = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hMutx_Image = CreateMutex( NULL, FALSE, NULL );
	m_pIplImage = NULL;
	m_nImageNum = 0;
	m_nGetImageCount = 0;

	memset(g_szYUVImageBuffer, 0, sizeof(g_szYUVImageBuffer));
	memset(g_szRGBImageBuffer, 0, sizeof(g_szRGBImageBuffer));
}

HikHCNetVs::~HikHCNetVs(void)
{
}
int HikHCNetVs::Init()
{
	int iRet = 0;

	// 初始化
	if(FALSE == NET_DVR_Init())
	{
		iRet = NET_DVR_GetLastError();
		TRACE_ERROR("DVR初始化出错, %d\r\n", iRet);
		return iRet;
	}

	//设置异常消息回调函数
	if(FALSE == NET_DVR_SetExceptionCallBack_V30(0, NULL,DVR_ExceptionCallBack, NULL))
	{
		iRet = NET_DVR_GetLastError();
		TRACE_ERROR("设置异常消息回调函数出错, %d\r\n", iRet);
		NET_DVR_Cleanup();
		return iRet;
	}

	// 连接摄像头
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	m_lUserID = NET_DVR_Login_V30("192.168.0.222", 8000, "admin", "12345", &struDeviceInfo);
	if (m_lUserID < 0)
	{
		iRet = NET_DVR_GetLastError();
		TRACE_ERROR("连接摄像头出错, %d\r\n", iRet);
		NET_DVR_Cleanup();
		return iRet;
	}

	// 启动预览并设置回调数据流
	NET_DVR_CLIENTINFO ClientInfo = {0};
	ClientInfo.hPlayWnd = NULL;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	ClientInfo.lChannel     = 1;
	ClientInfo.lLinkMode    = 0;       //最高位(31)为0表示主码流，为1表示子码流0～30位表示连接方式：0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式;
	ClientInfo.sMultiCastIP = NULL;   //多播地址，需要多播预览时配置

	BOOL bPreviewBlock = 0;       //请求码流过程是否阻塞，0：否，1：是
	m_lRealPlayHandle = NET_DVR_RealPlay_V30(m_lUserID, &ClientInfo, DVR_RealDataCallBack, this, bPreviewBlock);
	if (m_lRealPlayHandle < 0)
	{
		iRet = NET_DVR_GetLastError();
		TRACE_ERROR("启动预览并设置回调数据流出错,%d\r\n", iRet);
		NET_DVR_Logout(m_lUserID);
		NET_DVR_Cleanup();
		return iRet;
	}

	return iRet;
}

const IplImage* HikHCNetVs::GetImage()
{
	if(WAIT_OBJECT_0 == WaitForSingleObject(m_hEvent_HasImage, 10000))
	{
		if ( WAIT_OBJECT_0 == WaitForSingleObject( m_hMutx_Image, 5000 ) )
		{
			// YUV转化为RGB24
			INT width = m_iImageWidth;
			INT height = m_iImageHeigth;
			BYTE* y = g_szYUVImageBuffer ;
			BYTE* v = y + width * height ;
			BYTE* u = v + ( width >> 1 ) * ( height >> 1 ) ;		
			Functions::YUVToRGB24(y,width,u,v,width>>1,g_szRGBImageBuffer,width,(int)height);

			// 生成图片
			if(m_pIplImage == NULL)
			{
				m_pIplImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
				m_pIplImage->imageData = (char*)g_szRGBImageBuffer;
			}
			
			//ResetEvent( m_hEvent_HasImage );

			ReleaseMutex( m_hMutx_Image );

			m_nGetImageCount++;
			if(m_nGetImageCount % 200 == 0)
			{
				TRACE_INFO("解码图片数：%d  请求图片数：%d\r\n",m_nImageNum,m_nGetImageCount);
			}
			return m_pIplImage;
		}
		else
		{
			ReleaseMutex( m_hMutx_Image );
			TRACE_ERROR("获取图片超时，系统繁忙。\r\n");
			NULL;
		}
	}
	else
	{
		TRACE_ERROR("获取图片超时，请确认是否正常连接摄像机。\r\n");
		return NULL;
	}
}
void __stdcall HikHCNetVs::DVR_DecCallBack(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nUser,long nReserved2)
{
	HikHCNetVs *pThis = (HikHCNetVs *)nUser;
	if( pThis == NULL )
		return;
	if( pFrameInfo->nType != T_YV12 )//不是视频帧
		return;
 
	// 复制图片
	if ( WAIT_OBJECT_0 == WaitForSingleObject( pThis->m_hMutx_Image, 5000 ) )
	{
		//TRACE_INFO("复制图片【%d,%d,%d】\r\n", pFrameInfo->nWidth, pFrameInfo->nHeight, nSize);
		pThis->m_iImageWidth = pFrameInfo->nWidth;
		//pThis->m_iImageHeigth = pFrameInfo->nHeight;//直接赋值，1080p分辨率有问题
		pThis->m_iImageHeigth = nSize/pFrameInfo->nWidth * 2 / 3;
		memcpy(pThis->g_szYUVImageBuffer, pBuf, nSize);
		pThis->m_nImageNum++;
		SetEvent( pThis->m_hEvent_HasImage );
	}
	ReleaseMutex( pThis->m_hMutx_Image );
}
void __stdcall HikHCNetVs::DVR_RealDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,void *pUser)
{
	
	if(dwBufSize <= 0) return;

	HikHCNetVs *pThis = (HikHCNetVs *)pUser;
	switch (dwDataType)
	{
		case NET_DVR_SYSHEAD: //系统头
			TRACE_INFO("设置实时流播放模式\r\n");
			if (!PlayM4_SetStreamOpenMode(0, STREAME_REALTIME))  //设置实时流播放模式
			{
				TRACE_WARN("设置实时流播放模式出错  \r\n");
				break;
			} 
			TRACE_INFO("打开流接口\r\n");
			if (!PlayM4_OpenStream(0, pBuffer, dwBufSize, 1024*1024*4)) //打开流接口
			{
				TRACE_WARN("打开流接口出错  \r\n");
				break;
			}
			TRACE_INFO("设置解码回调函数\r\n");
			if( !PlayM4_SetDecCallBackMend( 0, DVR_DecCallBack, (long)pUser) )
			{
				TRACE_WARN("设置解码回调函数失败:%d \r\n", PlayM4_GetLastError(0));
				break;
			}
			TRACE_INFO("播放开始\r\n");
			//if (!PlayM4_Play(0, GetConsoleWindow())) //播放开始
			if (!PlayM4_Play(0, NULL)) //播放开始
			{
				TRACE_WARN("解码开始失败 \r\n");
				break;
			}
			break;
		case NET_DVR_STREAMDATA:   //码流数据
			try
			{
				if (!PlayM4_InputData(0, pBuffer, dwBufSize))
				{
					break;
				}
			}
			catch(...)
			{
				TRACE_ERROR("异常！！！\r\n");
			}

			break;
		default:
			break;
	}
	return;
}
void __stdcall HikHCNetVs::DVR_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    switch(dwType) 
    {
		case EXCEPTION_EXCHANGE:    //用户交互时异常
			TRACE_ERROR("!!! [HikHCNetVs] 用户交互时异常 \r\n");
			break;
		case EXCEPTION_AUDIOEXCHANGE:    //语音对讲异常
			TRACE_ERROR("!!! [HikHCNetVs] 语音对讲异常 \r\n");
			break;
		case EXCEPTION_ALARM:    //报警异常
			TRACE_ERROR("!!! [HikHCNetVs] 报警异常 \r\n");
			break;
		case EXCEPTION_PREVIEW:    //网络预览异常
			TRACE_ERROR("!!! [HikHCNetVs] 网络预览异常 \r\n");
			break;
		case EXCEPTION_SERIAL:    //透明通道异常
			TRACE_ERROR("!!! [HikHCNetVs] 透明通道异常 \r\n");
			break;
		case EXCEPTION_RECONNECT:    //预览时重连
			TRACE_ERROR("!!! [HikHCNetVs] 预览时重连 \r\n");
			break;
		case EXCEPTION_ALARMRECONNECT:    //报警时重连
			TRACE_ERROR("!!! [HikHCNetVs] 报警时重连 \r\n");
			break;
		case EXCEPTION_SERIALRECONNECT:    //透明通道重连
			TRACE_ERROR("!!! [HikHCNetVs] 透明通道重连 \r\n");
			break;
		case SERIAL_RECONNECTSUCCESS:    //透明通道重连成功
			TRACE_ERROR("!!! [HikHCNetVs] 透明通道重连成功 \r\n");
			break;
		case EXCEPTION_PLAYBACK:    //回放异常
			TRACE_ERROR("!!! [HikHCNetVs] 回放异常 \r\n");
			break;
		case EXCEPTION_DISKFMT:    //硬盘格式化
			TRACE_ERROR("!!! [HikHCNetVs] 硬盘格式化 \r\n");
			break;
		case EXCEPTION_PASSIVEDECODE:    //被动解码异常
			TRACE_ERROR("!!! [HikHCNetVs] 被动解码异常 \r\n");
			break;
		case EXCEPTION_EMAILTEST:    //邮件测试异常
			TRACE_ERROR("!!! [HikHCNetVs] 邮件测试异常 \r\n");
			break;
		case EXCEPTION_BACKUP:    //备份异常
			TRACE_ERROR("!!! [HikHCNetVs] 备份异常 \r\n");
			break;
		case PREVIEW_RECONNECTSUCCESS:    //预览时重连成功
			TRACE_ERROR("!!! [HikHCNetVs] 预览时重连成功 \r\n");
			break;
		case ALARM_RECONNECTSUCCESS:    //报警时重连成功
			TRACE_ERROR("!!! [HikHCNetVs] 报警时重连成功 \r\n");
			break;
		default:
			TRACE_ERROR("!!! [HikHCNetVs] 未知异常 \r\n");
			break;
    }
}
