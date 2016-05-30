#pragma once

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
};

