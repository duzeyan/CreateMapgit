#pragma once

class ToolsUtil
{
public:
	ToolsUtil(void);
	~ToolsUtil(void);
public:
	//γ�ȣ����ȣ�  �������x,y ��λ�Ƕ�,��λ������
	void static GPS2Earthy(double lat, double lng, int &earthx, int &earthy);
};

