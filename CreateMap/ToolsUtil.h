#pragma once

#include "NJUST_MAP_data.h"
class ToolsUtil
{
public:
	ToolsUtil(void);
	~ToolsUtil(void);
public:
	//纬度，经度，  大地坐标x,y 单位是度,单位是厘米
	 static void  GPS2Earthy(double lat, double lng, int &earthx, int &earthy);

	//计算两对经纬度获取之间的直线距离(m)
	static double GetDistanceByGPS(double lng1,double lat1,double lng2,double lat2);

	//宽字符转化为ASSIC dstLen单位为字节
	static void WtoA(char* dst,unsigned int dstLen,const CString *src);


	//解析惯性导航信息
	static int ToolsUtil::NJUST_MC_Decode_IP_Data( const void* pIPData, const int nBytes,
		NJUST_MC_STATE_INFO  **pState, //当不是状态数据时,值为NULL
		NJUST_MC_NAV_INFO  **pNav, //当不是导航信息时,值为NULL
		NJUST_MC_DRIVE_INFO  **pDrive  //当不是执行数据时,值为NULL
		);

	static int ToolsUtil::NJUST_MC_Decode_State(const void* pIPData, const int nBytes);

	static  int NJUST_MC_Decode_NAV(const void* pIPData, const int nBytes);

	static int NJUST_MC_Decode_Drive(const void* pIPData, const int nBytes);

};

