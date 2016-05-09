// UDP.cpp: implementation of the CUDP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UDP.h"
#include "CreateMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char buff[1024];

/////////////////////////////////////////////////////////////////////////////
CGzUdp::CGzUdp()
{
	//初始化
	m_sockReceive = INVALID_SOCKET;
	m_sockSend   = INVALID_SOCKET;
//	m_ipTarget    = TARGET_UDP_IP; //双字形式
	m_wTargetPort  = TARGET_UDP_PORT;
	m_wLocalPort   =LOCAL_UDP_PORT ;

	m_bIsReceiving = FALSE;
	m_hRcvThread   = NULL;
}

CGzUdp::~CGzUdp()
{
	DeleteSender();
	DeleteReceiver();
	StopReceiving();
}

BOOL CGzUdp::Init(CCreateMapDlg *dlg)
{

	WSADATA wsaData;
	WORD version = MAKEWORD(1, 1);
	int ret = WSAStartup(version, &wsaData);
	if(ret != 0)
	{
		TRACE(_T("Initilize Error!\n"));  //初始化失败
		return FALSE;
	}
	m_dlg=dlg;
	return TRUE;
}

void CGzUdp::SetTargetIP(DWORD inIP)
{
	m_ipTarget = inIP;
}

DWORD CGzUdp::GetTargetIP(void)
{
	return m_ipTarget;
}

void CGzUdp::SetTargetIP(const char * inIP)
{
	m_ipTarget = ntohl(inet_addr(inIP));
}

void CGzUdp::GetTargetIP(char * outIP)
{
	if (outIP)
	{
		struct in_addr   in;
		in.S_un.S_addr = htonl(m_ipTarget);
		char * pStr = inet_ntoa(in);
		strcpy(outIP, pStr);
	}
}

void CGzUdp::SetTargetPort(WORD inPort)
{
	m_wTargetPort = inPort;
}

WORD CGzUdp::GetTargetPort(void)
{
	return m_wTargetPort;
}

void CGzUdp::SetLocalPort(WORD inPort)
{
	m_wLocalPort = inPort;
}

WORD CGzUdp::GetLocalPort(void)
{
	return m_wLocalPort;
}

//网络发送初始化
BOOL CGzUdp::CreateSender(void)
{
	DeleteSender();

	m_sockSend = socket(AF_INET, SOCK_DGRAM, 0);//UDP协议
	if (m_sockSend != INVALID_SOCKET)
	{
		return TRUE;
	}

	int nZero=0;
	setsockopt(m_sockSend,SOL_SOCKET,SO_SNDBUF,(char *)&nZero,sizeof(nZero));

	//发送时限
	int nNetTimeout=100;//100毫秒
	setsockopt(m_sockSend,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));

	return FALSE;
}

//关闭网络
void CGzUdp::DeleteSender(void)
{
	if (m_sockSend != INVALID_SOCKET)
	{
		closesocket(m_sockSend);
		m_sockSend = INVALID_SOCKET;
	}
}

//创建接收网络
BOOL CGzUdp::CreateReceiver(void)
{
	DeleteReceiver();

	m_sockReceive = socket(AF_INET, SOCK_DGRAM, 0);//UDP协议
	if (m_sockReceive != INVALID_SOCKET)
	{
		BOOL flag = TRUE;
		int ret = setsockopt(m_sockReceive, SOL_SOCKET, SO_REUSEADDR, 
			(char *) &flag, sizeof(flag));
		if (ret == SOCKET_ERROR) 
		{
			DeleteReceiver();
			return FALSE;
		}

		SOCKADDR_IN  addr;
		addr.sin_family      = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port        = htons(m_wLocalPort);
		ret = bind(m_sockReceive, (struct sockaddr*) &addr, sizeof(addr));
		if (ret == SOCKET_ERROR) 
		{
			DeleteReceiver();
			return FALSE;
		}

		int nZero=0;
		setsockopt(m_sockReceive,SOL_SOCKET,SO_RCVBUF,(char *)&nZero,sizeof(int));

		//接收超时时限
		int nNetTimeout=80;//80毫秒
		setsockopt(m_sockReceive,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));

		return TRUE;
	}

	return FALSE;
}

//关闭网络接收
void CGzUdp::DeleteReceiver(void)
{
	if (m_sockReceive != INVALID_SOCKET)
	{
		closesocket(m_sockReceive);
		m_sockReceive = INVALID_SOCKET;
	}
}

//数据发送
int CGzUdp::Send(const char * inBuffer, long inLength)
{
	SOCKADDR_IN  addr;
	memset((char *) &addr, 0, sizeof(addr));  
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = htonl(m_ipTarget);
	addr.sin_port        = htons(m_wTargetPort);

	int val = sendto(m_sockSend, inBuffer, inLength, 0, 
		(sockaddr *) &addr, sizeof(addr));

	return val;
}


BOOL CGzUdp::SendToPC(const char * inBuffer,int len,char * ip,WORD port)
{
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock != INVALID_SOCKET)
	{
		SOCKADDR_IN  addr;
		memset((char *) &addr, 0, sizeof(addr));  
		addr.sin_family      = AF_INET;
		addr.sin_port        = htons(port);
		addr.sin_addr.s_addr=inet_addr(ip);
		int val = sendto(sock, inBuffer, len, 0, 
		(sockaddr *) &addr, sizeof(addr));
		closesocket(sock);
		return (val != SOCKET_ERROR);
	}
	return FALSE;

}

//开启接收线程
BOOL CGzUdp::StartReceiving(void)
{
	// Create socket if necessary
	if (m_sockReceive == INVALID_SOCKET)
	{
		CreateReceiver();
	}

	if (m_sockReceive != INVALID_SOCKET)
	{
		if (m_bIsReceiving)
		{
			return TRUE;
		}
		
		DWORD threadID = 0;
		m_hRcvThread = CreateThread(NULL, 0, ReceivingThrd, 
			this, 0, &threadID);
		return (m_hRcvThread != NULL);
	}
	return FALSE;
}

void CGzUdp::StopReceiving(void)
{
	if (m_bIsReceiving)
	{
		DeleteReceiver();
		// Make sure the receiving thread has been terminated 
		if (m_hRcvThread != NULL) 
		{
			if(WaitForSingleObject(m_hRcvThread, 100) != WAIT_OBJECT_0)
			{
				::TerminateThread(m_hRcvThread,1);
			}

			m_hRcvThread = NULL;
		}
	}
}

DWORD WINAPI CGzUdp::ReceivingThrd(void * pParam)
{
	CGzUdp * pController = (CGzUdp*) pParam;
	pController->ReceivingLoop();
	return 0;
}

//接收循环
void CGzUdp::ReceivingLoop()
{
	struct sockaddr_in  addr_cli;
	int  addr_cli_len = sizeof(addr_cli);
	long bytes = 0;
	m_bIsReceiving = TRUE;

	while (m_bIsReceiving)
	{			
		memset(buff,0,1024);
		bytes = recvfrom(m_sockReceive, buff, 1024, 0,(LPSOCKADDR) &addr_cli, (int *) &addr_cli_len);	
		
		m_dlg->showNowGPS(buff,bytes);
	}
}
