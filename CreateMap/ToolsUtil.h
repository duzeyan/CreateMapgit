#pragma once

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
};

