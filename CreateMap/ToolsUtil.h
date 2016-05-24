#pragma once

class ToolsUtil
{
public:
	ToolsUtil(void);
	~ToolsUtil(void);
public:
	//纬度，经度，  大地坐标x,y 单位是度,单位是厘米
	void static GPS2Earthy(double lat, double lng, int &earthx, int &earthy);
};

