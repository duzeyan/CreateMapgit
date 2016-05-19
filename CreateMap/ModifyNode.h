#pragma once

#include"MAP__BASIC_data.h"
// ModifyNode 对话框

class ModifyNode : public CDialogEx
{
	DECLARE_DYNAMIC(ModifyNode)

public:
	ModifyNode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ModifyNode();

// 对话框数据
	enum { IDD = IDD_MODIFYNODE_DIALOG };

public :
	void setValue(MAP_NODE node);   //传递原始数据
	void initView();			    //初始化视窗

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_radioNodeType;
	afx_msg void OnBnClickedUnknow();
};
