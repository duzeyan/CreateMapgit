#pragma once

#include"MAP_BASIC_data.h"
#include "NJUSTMap.h"


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

	//在直线模型下修补断层
	static void CompleGPS(vector<MAP_DOUBLE_POINT> &GPSList);

	//解析路点文件 总装版本
	static int AnalyticalZZ(NJUSTMap &map); 

	//解析路点文件 自然基金版本
	static int AnalyticalZR(NJUSTMap &map); 

	//读取任务路点文件,显示在图中
	static int ReadTaskFile();

	//在vps中查找和给定GPS序列最近的点的索引
	static size_t getIndexByDis(double lng,double lat,const vector<MAP_DOUBLE_POINT> &vps);

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

