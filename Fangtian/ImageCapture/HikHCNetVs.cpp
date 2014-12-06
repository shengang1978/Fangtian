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

	// ��ʼ��
	if(FALSE == NET_DVR_Init())
	{
		iRet = NET_DVR_GetLastError();
		TRACE_ERROR("DVR��ʼ������, %d\r\n", iRet);
		return iRet;
	}

	//�����쳣��Ϣ�ص�����
	if(FALSE == NET_DVR_SetExceptionCallBack_V30(0, NULL,DVR_ExceptionCallBack, NULL))
	{
		iRet = NET_DVR_GetLastError();
		TRACE_ERROR("�����쳣��Ϣ�ص���������, %d\r\n", iRet);
		NET_DVR_Cleanup();
		return iRet;
	}

	// ��������ͷ
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	m_lUserID = NET_DVR_Login_V30("192.168.0.222", 8000, "admin", "12345", &struDeviceInfo);
	if (m_lUserID < 0)
	{
		iRet = NET_DVR_GetLastError();
		TRACE_ERROR("��������ͷ����, %d\r\n", iRet);
		NET_DVR_Cleanup();
		return iRet;
	}

	// ����Ԥ�������ûص�������
	NET_DVR_CLIENTINFO ClientInfo = {0};
	ClientInfo.hPlayWnd = NULL;         //��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
	ClientInfo.lChannel     = 1;
	ClientInfo.lLinkMode    = 0;       //���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ������0��30λ��ʾ���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ��2���ಥ��ʽ��3��RTP��ʽ;
	ClientInfo.sMultiCastIP = NULL;   //�ಥ��ַ����Ҫ�ಥԤ��ʱ����

	BOOL bPreviewBlock = 0;       //�������������Ƿ�������0����1����
	m_lRealPlayHandle = NET_DVR_RealPlay_V30(m_lUserID, &ClientInfo, DVR_RealDataCallBack, this, bPreviewBlock);
	if (m_lRealPlayHandle < 0)
	{
		iRet = NET_DVR_GetLastError();
		TRACE_ERROR("����Ԥ�������ûص�����������,%d\r\n", iRet);
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
			// YUVת��ΪRGB24
			INT width = m_iImageWidth;
			INT height = m_iImageHeigth;
			BYTE* y = g_szYUVImageBuffer ;
			BYTE* v = y + width * height ;
			BYTE* u = v + ( width >> 1 ) * ( height >> 1 ) ;		
			Functions::YUVToRGB24(y,width,u,v,width>>1,g_szRGBImageBuffer,width,(int)height);

			// ����ͼƬ
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
				TRACE_INFO("����ͼƬ����%d  ����ͼƬ����%d\r\n",m_nImageNum,m_nGetImageCount);
			}
			return m_pIplImage;
		}
		else
		{
			ReleaseMutex( m_hMutx_Image );
			TRACE_ERROR("��ȡͼƬ��ʱ��ϵͳ��æ��\r\n");
			NULL;
		}
	}
	else
	{
		TRACE_ERROR("��ȡͼƬ��ʱ����ȷ���Ƿ����������������\r\n");
		return NULL;
	}
}
void __stdcall HikHCNetVs::DVR_DecCallBack(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nUser,long nReserved2)
{
	HikHCNetVs *pThis = (HikHCNetVs *)nUser;
	if( pThis == NULL )
		return;
	if( pFrameInfo->nType != T_YV12 )//������Ƶ֡
		return;
 
	// ����ͼƬ
	if ( WAIT_OBJECT_0 == WaitForSingleObject( pThis->m_hMutx_Image, 5000 ) )
	{
		//TRACE_INFO("����ͼƬ��%d,%d,%d��\r\n", pFrameInfo->nWidth, pFrameInfo->nHeight, nSize);
		pThis->m_iImageWidth = pFrameInfo->nWidth;
		//pThis->m_iImageHeigth = pFrameInfo->nHeight;//ֱ�Ӹ�ֵ��1080p�ֱ���������
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
		case NET_DVR_SYSHEAD: //ϵͳͷ
			TRACE_INFO("����ʵʱ������ģʽ\r\n");
			if (!PlayM4_SetStreamOpenMode(0, STREAME_REALTIME))  //����ʵʱ������ģʽ
			{
				TRACE_WARN("����ʵʱ������ģʽ����  \r\n");
				break;
			} 
			TRACE_INFO("�����ӿ�\r\n");
			if (!PlayM4_OpenStream(0, pBuffer, dwBufSize, 1024*1024*4)) //�����ӿ�
			{
				TRACE_WARN("�����ӿڳ���  \r\n");
				break;
			}
			TRACE_INFO("���ý���ص�����\r\n");
			if( !PlayM4_SetDecCallBackMend( 0, DVR_DecCallBack, (long)pUser) )
			{
				TRACE_WARN("���ý���ص�����ʧ��:%d \r\n", PlayM4_GetLastError(0));
				break;
			}
			TRACE_INFO("���ſ�ʼ\r\n");
			//if (!PlayM4_Play(0, GetConsoleWindow())) //���ſ�ʼ
			if (!PlayM4_Play(0, NULL)) //���ſ�ʼ
			{
				TRACE_WARN("���뿪ʼʧ�� \r\n");
				break;
			}
			break;
		case NET_DVR_STREAMDATA:   //��������
			try
			{
				if (!PlayM4_InputData(0, pBuffer, dwBufSize))
				{
					break;
				}
			}
			catch(...)
			{
				TRACE_ERROR("�쳣������\r\n");
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
		case EXCEPTION_EXCHANGE:    //�û�����ʱ�쳣
			TRACE_ERROR("!!! [HikHCNetVs] �û�����ʱ�쳣 \r\n");
			break;
		case EXCEPTION_AUDIOEXCHANGE:    //�����Խ��쳣
			TRACE_ERROR("!!! [HikHCNetVs] �����Խ��쳣 \r\n");
			break;
		case EXCEPTION_ALARM:    //�����쳣
			TRACE_ERROR("!!! [HikHCNetVs] �����쳣 \r\n");
			break;
		case EXCEPTION_PREVIEW:    //����Ԥ���쳣
			TRACE_ERROR("!!! [HikHCNetVs] ����Ԥ���쳣 \r\n");
			break;
		case EXCEPTION_SERIAL:    //͸��ͨ���쳣
			TRACE_ERROR("!!! [HikHCNetVs] ͸��ͨ���쳣 \r\n");
			break;
		case EXCEPTION_RECONNECT:    //Ԥ��ʱ����
			TRACE_ERROR("!!! [HikHCNetVs] Ԥ��ʱ���� \r\n");
			break;
		case EXCEPTION_ALARMRECONNECT:    //����ʱ����
			TRACE_ERROR("!!! [HikHCNetVs] ����ʱ���� \r\n");
			break;
		case EXCEPTION_SERIALRECONNECT:    //͸��ͨ������
			TRACE_ERROR("!!! [HikHCNetVs] ͸��ͨ������ \r\n");
			break;
		case SERIAL_RECONNECTSUCCESS:    //͸��ͨ�������ɹ�
			TRACE_ERROR("!!! [HikHCNetVs] ͸��ͨ�������ɹ� \r\n");
			break;
		case EXCEPTION_PLAYBACK:    //�ط��쳣
			TRACE_ERROR("!!! [HikHCNetVs] �ط��쳣 \r\n");
			break;
		case EXCEPTION_DISKFMT:    //Ӳ�̸�ʽ��
			TRACE_ERROR("!!! [HikHCNetVs] Ӳ�̸�ʽ�� \r\n");
			break;
		case EXCEPTION_PASSIVEDECODE:    //���������쳣
			TRACE_ERROR("!!! [HikHCNetVs] ���������쳣 \r\n");
			break;
		case EXCEPTION_EMAILTEST:    //�ʼ������쳣
			TRACE_ERROR("!!! [HikHCNetVs] �ʼ������쳣 \r\n");
			break;
		case EXCEPTION_BACKUP:    //�����쳣
			TRACE_ERROR("!!! [HikHCNetVs] �����쳣 \r\n");
			break;
		case PREVIEW_RECONNECTSUCCESS:    //Ԥ��ʱ�����ɹ�
			TRACE_ERROR("!!! [HikHCNetVs] Ԥ��ʱ�����ɹ� \r\n");
			break;
		case ALARM_RECONNECTSUCCESS:    //����ʱ�����ɹ�
			TRACE_ERROR("!!! [HikHCNetVs] ����ʱ�����ɹ� \r\n");
			break;
		default:
			TRACE_ERROR("!!! [HikHCNetVs] δ֪�쳣 \r\n");
			break;
    }
}
