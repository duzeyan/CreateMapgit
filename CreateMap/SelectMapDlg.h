#pragma once
#include "afxwin.h"
#include "map_mysql_data.h"

// SelectMapDlg 对话框

class SelectMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SelectMapDlg)

public:
	SelectMapDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SelectMapDlg();

// 对话框数据
	enum { IDD = IDD_SELMAP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboxMapList;
	vector<MODEL_WINMAPNAME> m_winmapname;
public:
	  void init(const vector<MODEL_WINMAPNAME> &winmapname);
	  afx_msg void OnBnClickedOk();
};
