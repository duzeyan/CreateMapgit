#pragma once

#include"MAP_BASIC_data.h"
#include "NJUSTMap.h"


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

	//��ֱ��ģ�����޲��ϲ�
	static void CompleGPS(vector<MAP_DOUBLE_POINT> &GPSList);

	//����·���ļ� ��װ�汾
	static int AnalyticalZZ(NJUSTMap &map); 

	//����·���ļ� ��Ȼ����汾
	static int AnalyticalZR(NJUSTMap &map); 

	//��ȡ����·���ļ�,��ʾ��ͼ��
	static int ReadTaskFile();

	//��vps�в��Һ͸���GPS��������ĵ������
	static size_t getIndexByDis(double lng,double lat,const vector<MAP_DOUBLE_POINT> &vps);

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

