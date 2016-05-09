#pragma once

#include "NJUST_MAP_data.h"
#include "UDP.h"
#include <windows.h>


static NJUST_MC_STATE_INFO  gMCState;
static NJUST_MC_NAV_INFO    gMCNav;
static NJUST_MC_DRIVE_INFO  gMCDrive;

class CreateMapCommunication
{

private:
	CGzUdp   m_Udp;
	//CreateMapDlg *m_pWnd;
public:
	CreateMapCommunication(void);
	~CreateMapCommunication(void);

	 void        GPSdataprocess(char * buff, int len);
     int         NJUST_MC_Decode_IP_Data( const void* pIPData, const int nBytes,
                             NJUST_MC_STATE_INFO  **pState, //当不是状态数据时,值为NULL
							 NJUST_MC_NAV_INFO  **pNav, //当不是导航信息时,值为NULL
							 NJUST_MC_DRIVE_INFO  **pDrive  //当不是执行数据时,值为NULL
						   );
     int         NJUST_MC_Decode_Drive(const void* pIPData, const int nBytes);
     int         NJUST_MC_Decode_NAV(const void* pIPData, const int nBytes);
     int         NJUST_MC_Decode_State(const void* pIPData, const int nBytes);
	 void        blh2xy(double x,double y,int &earthx,int &earthy);
	 void        Initial();
	 void        StartUdpCommunication(CCreateMapDlg *dlg);
	 void  getGPSAndPostion(char * buff, int len,CString []);
};

