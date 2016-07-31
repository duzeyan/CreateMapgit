#include "stdafx.h"
#include "CreateMapCommunication.h"
#include "ToolsUtil.h"
#include <math.h>

CreateMapCommunication::CreateMapCommunication(void)
{
}


CreateMapCommunication::~CreateMapCommunication(void)
{
}




//int CreateMapCommunication::NJUST_MC_Decode_IP_Data( const void* pIPData, const int nBytes,
//                             NJUST_MC_STATE_INFO  **pState, //当不是状态数据时,值为NULL
//							 NJUST_MC_NAV_INFO  **pNav, //当不是导航信息时,值为NULL
//							 NJUST_MC_DRIVE_INFO  **pDrive  //当不是执行数据时,值为NULL
//						   )
//{
//	int errCode;
//
//	// step.1----------初始化--------------------------------//
//	*pState=NULL;
//	*pNav=NULL;
//	*pDrive=NULL;
//	errCode=1;   //无有效数据 
//	//step.2----------数据解析-------------------------------//
//	if(((char *)pIPData)[0]==0x24&&((char *)pIPData)[1]==0x09){//状态
//		errCode=NJUST_MC_Decode_State(pIPData,nBytes);
//	    if (!errCode)
//		{
//			*pState=&gMCState;
//		}
//	}else if(((char *)pIPData)[0]==0x24&&((char *)pIPData)[1]==0x08){//导航
//		errCode=NJUST_MC_Decode_NAV(pIPData,nBytes);
//	    if (!errCode)
//		{
//			*pNav=&gMCNav;
//		}
//	
//	}else if(((char *)pIPData)[0]==0x24&&((char *)pIPData)[1]==0x07){//驱动
//		errCode=NJUST_MC_Decode_Drive(pIPData,nBytes);
//	    if (!errCode)
//		{
//			*pDrive=&gMCDrive;
//		}
//	}
//
//	//step.3----------返回-----------------------------------//
//	return errCode;
//}
//
//int CreateMapCommunication::NJUST_MC_Decode_State(const void* pIPData, const int nBytes)
//{   //解析导航信息:将数据解析到gMCState
//
//	int errCode=0; //无错误
//	char *pBuf;
//	signed char checksum;
//	int i;
//	//step.1----------初始化---------------------------------//
//	memset(&gMCState,0,sizeof(NJUST_MC_STATE_INFO));
//
//
//	//step.2----------将数据解析到gMCState-------------------//
//	pBuf = (char *)pIPData;
//	for(i=0,checksum=0;i<nBytes-1;i++)
//	{	
//		checksum +=pBuf[i];    
//	}
//	i=sizeof(NJUST_MC_STATE_INFO);
//	if((signed char)(pBuf[nBytes-1])!=checksum||(nBytes)!=sizeof(NJUST_MC_STATE_INFO)){//||(nBytes)!=sizeof(NJUST_MC_STATE_INFO)
//		errCode=1;//两个校验位或checksum或大小不对，有错误
//	}else{
//		memcpy(&gMCState,pBuf,sizeof(NJUST_MC_STATE_INFO));
//		if(gMCState.nSize!=sizeof(NJUST_MC_STATE_INFO)){//结构体大小对不上
//			memset(&gMCState,0,sizeof(NJUST_MC_STATE_INFO));
//			errCode=1;
//		}
//	}
//
//	//step.3----------返回错误码-----------------------------//
//	return errCode;
//}
//
//int CreateMapCommunication::NJUST_MC_Decode_NAV(const void* pIPData, const int nBytes)
//{   //解析状态数据,将数据解析到gMCNav
//	int errCode=0; //无错误
//	char *pBuf;
//	signed char checksum;
//	int i;
//	//step.1----------初始化---------------------------------//
//	memset(&gMCNav,0,sizeof(NJUST_MC_NAV_INFO));
//	//step.2----------将数据解析到gMCNav---------------------//
//	pBuf = (char *)pIPData;
//	for(i=0,checksum=0;i<nBytes-1;i++)
//	{	
//		checksum +=pBuf[i];    
//	}
//	i=sizeof(NJUST_MC_NAV_INFO);
//
//	//pBuf[nBytes-1]!=0x0a||pBuf[nBytes-2]!=0x0d
//	if((signed char)(pBuf[nBytes-1])!=checksum||(nBytes)!=sizeof(NJUST_MC_NAV_INFO)){
//		errCode=1;//两个校验位或checksum或大小不对，有错误
//	}else{
//		memcpy(&gMCNav,pBuf,sizeof(NJUST_MC_NAV_INFO));
//		if(gMCNav.nSize!=sizeof(NJUST_MC_NAV_INFO)){//结构体大小对不上
//			memset(&gMCNav,0,sizeof(NJUST_MC_NAV_INFO));
//			errCode=1;
//		}
//	}
//
//
//
//	//step.3----------返回错误码-----------------------------//
//	return errCode;
//
//	return errCode;
//}
//
//int CreateMapCommunication::NJUST_MC_Decode_Drive(const void* pIPData, const int nBytes)
//{   //解析执行数据,将数据解析到gMCDrive
//	int errCode=0; //无错误
//	char *pBuf;
//	signed char checksum;
//	int i;
//	//step.1----------初始化---------------------------------//
//	memset(&gMCDrive,0,sizeof(NJUST_MC_DRIVE_INFO));
//	//step.2----------将数据解析到gMCDrive-------------------//
//	pBuf = (char *)pIPData;
//	for(i=0,checksum=0;i<nBytes-3;i++)
//	{	
//		checksum +=pBuf[i];    
//	}
//
//	if(pBuf[nBytes-1]!=0x0a||pBuf[nBytes-2]!=0x0d||(signed char)(pBuf[nBytes-3])!=checksum||(nBytes-2)!=sizeof(NJUST_MC_DRIVE_INFO)){
//		errCode=1;//两个校验位或checksum或大小不对，有错误
//	}else{
//		memcpy(&gMCDrive,pBuf,sizeof(NJUST_MC_DRIVE_INFO));
//		if(gMCDrive.nSize!=sizeof(NJUST_MC_DRIVE_INFO)){//结构体大小对不上
//			memset(&gMCDrive,0,sizeof(NJUST_MC_DRIVE_INFO));
//			errCode=1;
//		}
//	}
//
//	//step.3----------返回错误码-----------------------------//
//	return errCode;
//}

//纬度，经度，单位是度,单位是厘米
void CreateMapCommunication::blh2xy(double x,double y,int &earthx,int &earthy)
{
	int n,L0;
	double X,N54,W54,t,m,a54,e54,e_54;
	double iptr;
	double t_2=0,t_4=0,yita_2=0,yita_4=0;
	double lp=0,lp_2=0;
	double SinL,CosL,CosL_2,SinL_2;
	double SinG,CosG;
	double daa,df,db2p,dl2p,dahm;
	double deltabo,deltalo;
	double w84,n84,m84,a84,e842,f84,f54,dx,dy,dz;
	double lati,logi,hegt;
	double pi=3.1415926535;

	lati = x;
	logi = y;

	lati = lati*pi/180;
	logi = logi*pi/180;
	SinL = sin(lati);
	CosL = cos(lati);
	SinG = sin(logi);
	CosG = cos(logi);
	CosL_2 = CosL * CosL;
	SinL_2 = SinL * SinL;

	a84=6378137.0;
	e842=0.00669437999014132;
	f84=1.0/298.257223563;

	a54=6378245.0;
	f54=1.0/298.3;

	dx=-16.0;
	dy=147.0;
	dz=77.0;

	w84=sqrt(1-e842*SinL_2);
	n84=a84/w84;
	m84=a84*(1-e842)/(w84*w84*w84);

	daa=a54-a84;
	df=f54-f84;

	db2p=(-dx*SinL*CosG-dy*SinL*SinG+dz*CosL+(a84*df+f84*daa)*sin(2*lati))/(m84*sin(1/3600.0*pi/180));
	dl2p=(-dx*SinG+dy*CosG)/(n84*CosL*sin(1/3600.0*pi/180));
	dahm=dx*CosL*CosG+dy*CosL*SinG+dz*SinL+(a84*df+f84*daa)*SinL_2-daa;

	deltabo=(db2p/3600.0)*pi/180.0;
	deltalo=(dl2p/3600.0)*pi/180.0;

	logi = logi+deltalo;
	lati = lati+deltabo;
//	hegt = hegt+dahm;

	SinL = sin(lati);
	CosL = cos(lati);
	CosL_2 = CosL * CosL;
	SinL_2 = SinL * SinL;

	a54 = 6378245.0;
	e54 = 0.0066934274898192;

	W54=sqrt(1.0-e54*SinL_2);
	N54=a54/W54;
	e_54=0.0067385254147;

	logi=logi*180/pi;
	modf(logi/6.0,&iptr);
	n=(int)iptr+1;
	L0=n*6-3;

	lp = (logi-L0)*pi/180;
	lp_2 = lp*lp;

	m=CosL_2*lp_2;
	yita_2=e_54*CosL_2;
	yita_4 = yita_2 * yita_2;

	t=tan(lati);
	t_2 = t*t;
	t_4 = t_2*t_2;

	double gps_xx,gps_yy;
	X = 111134.8611*lati*180/pi
		-SinL*CosL*( 32005.7799 + 133.9238*SinL_2 + 0.6973*SinL_2*SinL_2+0.0039*SinL_2*SinL_2*SinL_2);

	gps_yy = X + N54*t*m*( 0.5 + 1.0/24.0*( 5.0 - t_2 + 9.0*yita_2 + 4.0*yita_4)*m
		+ 1.0/720.0*( 61.0 - 58.0*t_2 + t_4)*m*m);

	gps_xx = N54*CosL*lp*( 1.0 + 1.0/6.0*( 1-t_2 + yita_2)*m
		+ 1.0/120.0*( 5.0 - 18.0*t_2 + t_4 + 14.0*yita_2-58.0*yita_2*t_2)*m*m);
	gps_xx = gps_xx + 1000000*n+500000;
	gps_xx*=100;
	gps_yy *=100;

	earthx = (int)gps_xx;
	earthy = (int)gps_yy;
	return;
}


void CreateMapCommunication::StartUdpCommunication(void *dlg)
{
	m_Udp.Init(dlg);  
	m_Udp.SetLocalPort(LOCAL_UDP_PORT);
	m_Udp.CreateReceiver();
	m_Udp.StartReceiving();
}

void CreateMapCommunication::ShutDownCommunication()
{
	m_Udp.DeleteReceiver();
	m_Udp.StopReceiving();
}



//回调实现功能
void  CreateMapCommunication::getGPSAndPostion(char * buff, int len,double longlat[2],int &frameID,int &state)
{
    
	double   m_longtitude=0;
	double   m_latitude=0;
	//int      m_earthx;
	//int      m_earthy;
	longlat[0]=longlat[1]=0.0;

	NJUST_MC_STATE_INFO  *pState; //当不是状态数据时,值为NULL
    NJUST_MC_NAV_INFO    *pNav; //当不是导航信息时,值为NULL
    NJUST_MC_DRIVE_INFO  *pDrive ; //当不是执行数据时,值为NUL
	ToolsUtil::NJUST_MC_Decode_IP_Data( buff,  //IP数据,目标模块从网络收到的数据
							 len,     //pIPData数据的字节个数
                             &pState, //当不是状态数据时,值为NULL
				             &pNav, //当不是导航信息时,值为NULL
							 &pDrive  //当不是执行数据时,值为NULL
						   );
        if(pNav)//收到导航信息
        {
            m_latitude=pNav->Latitude_degree*60;
	        m_longtitude=pNav->Longitude_degree*60;
			state=pNav->GPS_Status;
			frameID=pNav->FrameID;
			longlat[0]=m_longtitude;
			longlat[1]=m_latitude;
		}
}

