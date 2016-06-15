#pragma once

#include "NJUST_MAP_data.h"
class ToolsUtil
{
public:
	ToolsUtil(void);
	~ToolsUtil(void);
public:
	//γ�ȣ����ȣ�  �������x,y ��λ�Ƕ�,��λ������
	 static void  GPS2Earthy(double lat, double lng, int &earthx, int &earthy);

	//�������Ծ�γ�Ȼ�ȡ֮���ֱ�߾���(m)
	static double GetDistanceByGPS(double lng1,double lat1,double lng2,double lat2);

	//���ַ�ת��ΪASSIC dstLen��λΪ�ֽ�
	static void WtoA(char* dst,unsigned int dstLen,const CString *src);


	//�������Ե�����Ϣ
	static int ToolsUtil::NJUST_MC_Decode_IP_Data( const void* pIPData, const int nBytes,
		NJUST_MC_STATE_INFO  **pState, //������״̬����ʱ,ֵΪNULL
		NJUST_MC_NAV_INFO  **pNav, //�����ǵ�����Ϣʱ,ֵΪNULL
		NJUST_MC_DRIVE_INFO  **pDrive  //������ִ������ʱ,ֵΪNULL
		);

	static int ToolsUtil::NJUST_MC_Decode_State(const void* pIPData, const int nBytes);

	static  int NJUST_MC_Decode_NAV(const void* pIPData, const int nBytes);

	static int NJUST_MC_Decode_Drive(const void* pIPData, const int nBytes);

};

