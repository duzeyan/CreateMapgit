
#ifndef __H_CUDPManager__
#define __H_CUDPManager__

#include <winsock.h>
#include <afxsock.h>
#include <iostream>
//#include "CreateMapDlg.h"
//using namespace std;

#define TARGET_UDP_PORT  8000
//#define TARGET_UDP_PORT  6000
//#define  LOCAL_UDP_PORT  50000
#define  LOCAL_UDP_PORT  6000

//超前引用
class CCreateMapDlg;

class CGzUdp
{
public:
	CGzUdp();
	~CGzUdp();
protected:
	SOCKET		m_sockReceive;//网络接收
	SOCKET		m_sockSend;  //网络发送
	DWORD		m_ipTarget;   //目标IP 
	WORD		m_wTargetPort; //目标网络端口
	WORD		m_wLocalPort;  //本地网络端口

	HWND m_hParent;
	
	BOOL		m_bIsReceiving;//是否正在接收
	HANDLE		m_hRcvThread;  //接收线程
	
	CCreateMapDlg *m_dlg;

	
protected:
	virtual void  ReceivingLoop();
	static DWORD WINAPI ReceivingThrd(void * pParam);
	
public:
	//接口函数
	
	BOOL Init(CCreateMapDlg *dlg);
	
	void SetTargetIP(DWORD inIP);  //设置目标IP
	DWORD GetTargetIP(void);       //获得目标IP
	void SetTargetIP(const char * inIP);//设置目标IP
	void GetTargetIP(char * outIP);//获得目标IP
	
	void SetTargetPort(WORD inPort); //设置目标端口
	WORD GetTargetPort(void);        //获得目标端口
	
	void SetLocalPort(WORD inPort);  //设置本地端口
	WORD GetLocalPort(void);         //获得本地端口
	
	BOOL CreateSender(void);    //发送创建
	void DeleteSender(void);    //发送删除
	BOOL CreateReceiver(void);  //接收创建
	void DeleteReceiver(void);  //接收删除
	
	
	//发送
	int Send(const char * inBuffer, long inLength);
	BOOL SendToPC(const char * inBuffer,int len,char * ip,WORD port);
	//接收
	BOOL StartReceiving(void);
	void StopReceiving(void);
};

#endif // __H_CUDPManager__