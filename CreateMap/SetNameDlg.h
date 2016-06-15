#pragma once
#include "afxwin.h"
#include "map_mysql_data.h"

// SetNameDlg 对话框

class SetNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetNameDlg)

public:
	SetNameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetNameDlg();

// 对话框数据
	enum { IDD = IDD_NAMEMAP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void init(const vector<MODEL_WINMAPNAME> &winmapname);
public:
	CEdit m_edit;
	vector<MODEL_WINMAPNAME> m_winmapname;
	afx_msg void OnBnClickedOk();
};
