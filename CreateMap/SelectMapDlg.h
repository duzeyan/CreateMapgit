#pragma once
#include "afxwin.h"
#include "map_mysql_data.h"

// SelectMapDlg �Ի���

class SelectMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SelectMapDlg)

public:
	SelectMapDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SelectMapDlg();

// �Ի�������
	enum { IDD = IDD_SELMAP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboxMapList;
	vector<MODEL_WINMAPNAME> m_winmapname;
public:
	  void init(const vector<MODEL_WINMAPNAME> &winmapname);
	  afx_msg void OnBnClickedOk();
};
