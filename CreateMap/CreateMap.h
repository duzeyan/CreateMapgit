
// CreateMap.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CCreateMapApp:
// 有关此类的实现，请参阅 CreateMap.cpp
//

class CCreateMapApp : public CWinApp
{
public:
	CCreateMapApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	afx_msg void OnOpenMap();
	afx_msg void On32778();
};

extern CCreateMapApp theApp;