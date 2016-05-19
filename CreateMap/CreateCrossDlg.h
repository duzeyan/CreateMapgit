#pragma once
#include "NJUSTMap.h"
#include"DrawMapMark.h"
#include "afxwin.h"
#include <iostream>
using namespace std;

// CreateCrossDlg 对话框
class CreateCrossDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateCrossDlg)

public:
	CreateCrossDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CreateCrossDlg();

// 对话框数据
	enum { IDD = IDD_CREATECROSS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	vector<int> m_rodesIndex;  //道路编号序列
	CComboBox m_startComboBox;
	CComboBox m_endComboBox;
	afx_msg void OnBnClickedOk();

	//设置必要参数 道路ID序列
	void Initialize(vector<int> roads);

	void InitializeView(); //初始化视窗参数
};
