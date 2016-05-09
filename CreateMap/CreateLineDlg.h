#pragma once

#include "NJUSTMap.h"
#include"DrawMapMark.h"
#include "afxwin.h"
#include <iostream>
#include "resource.h"
using namespace std;

// CreateLineDlg 对话框

class CreateLineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateLineDlg)

public:
	CreateLineDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CreateLineDlg();

// 对话框数据
	enum { IDD = IDD_CREATELINE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	vector<int> m_nodesIndex;  //路口编号序列
	CComboBox m_startBox;   //头部列表
	CComboBox m_endBox;     //尾部列表
public:
	//设置必要参数 节点序列
	void Initialize(vector<int> nodes);

	void InitializeView(); //初始化视窗参数

	
	afx_msg void OnBnClickedLinedlgOk();
};
