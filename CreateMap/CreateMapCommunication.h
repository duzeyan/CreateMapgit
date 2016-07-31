#pragma once

#include "UDP.h"
#include <windows.h>




class CreateMapCommunication
{

private:
	CGzUdp   m_Udp;
	//CreateMapDlg *m_pWnd;
public:
	CreateMapCommunication(void);
	~CreateMapCommunication(void);

	 void        GPSdataprocess(char * buff, int len);
     //int         NJUST_MC_Decode_IP_Data( const void* pIPData, const int nBytes,
     //                        NJUST_MC_STATE_INFO  **pState, //当不是状态数据时,值为NULL
					//		 NJUST_MC_NAV_INFO  **pNav, //当不是导航信息时,值为NULL
					//		 NJUST_MC_DRIVE_INFO  **pDrive  //当不是执行数据时,值为NULL
					//	   );
     //int         NJUST_MC_Decode_Drive(const void* pIPData, const int nBytes);
     //int         NJUST_MC_Decode_NAV(const void* pIPData, const int nBytes);
     //int         NJUST_MC_Decode_State(const void* pIPData, const int nBytes);
	 void        blh2xy(double x,double y,int &earthx,int &earthy); //纬度，经度，单位是度,单位是厘米

	// void        StartUdpCommunication(CCreateMapDlg *dlg);
	void		StartUdpCommunication(void *dlg);

	 void        ShutDownCommunication();

	 void  getGPSAndPostion(char * buff, int len,double longlat[2],int &frameID,int &state);
};

