#pragma once
#include "afxwin.h"
#include "map_mysql_data.h"

// SetNameDlg �Ի���

class SetNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetNameDlg)

public:
	SetNameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetNameDlg();

// �Ի�������
	enum { IDD = IDD_NAMEMAP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void init(const vector<MODEL_WINMAPNAME> &winmapname);
public:
	CEdit m_edit;
	vector<MODEL_WINMAPNAME> m_winmapname;
	afx_msg void OnBnClickedOk();
};
