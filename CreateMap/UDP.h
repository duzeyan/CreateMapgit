
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

//��ǰ����
class CCreateMapDlg;

class CGzUdp
{
public:
	CGzUdp();
	~CGzUdp();
protected:
	SOCKET		m_sockReceive;//�������
	SOCKET		m_sockSend;  //���緢��
	DWORD		m_ipTarget;   //Ŀ��IP 
	WORD		m_wTargetPort; //Ŀ������˿�
	WORD		m_wLocalPort;  //��������˿�

	HWND m_hParent;
	
	BOOL		m_bIsReceiving;//�Ƿ����ڽ���
	HANDLE		m_hRcvThread;  //�����߳�
	
	CCreateMapDlg *m_dlg;

	
protected:
	virtual void  ReceivingLoop();
	static DWORD WINAPI ReceivingThrd(void * pParam);
	
public:
	//�ӿں���
	
	BOOL Init(CCreateMapDlg *dlg);
	
	void SetTargetIP(DWORD inIP);  //����Ŀ��IP
	DWORD GetTargetIP(void);       //���Ŀ��IP
	void SetTargetIP(const char * inIP);//����Ŀ��IP
	void GetTargetIP(char * outIP);//���Ŀ��IP
	
	void SetTargetPort(WORD inPort); //����Ŀ��˿�
	WORD GetTargetPort(void);        //���Ŀ��˿�
	
	void SetLocalPort(WORD inPort);  //���ñ��ض˿�
	WORD GetLocalPort(void);         //��ñ��ض˿�
	
	BOOL CreateSender(void);    //���ʹ���
	void DeleteSender(void);    //����ɾ��
	BOOL CreateReceiver(void);  //���մ���
	void DeleteReceiver(void);  //����ɾ��
	
	
	//����
	int Send(const char * inBuffer, long inLength);
	BOOL SendToPC(const char * inBuffer,int len,char * ip,WORD port);
	//����
	BOOL StartReceiving(void);
	void StopReceiving(void);
};

#endif // __H_CUDPManager__