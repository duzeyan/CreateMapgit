#pragma once
#include "NJUSTMap.h"
#include"DrawMapMark.h"
#include "afxwin.h"
#include <iostream>
using namespace std;

// CreateCorssDlg �Ի���

class CreateCorssDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateCorssDlg)

public:
	CreateCorssDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CreateCorssDlg();

// �Ի�������
	enum { IDD = IDD_CREATECROSS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	vector<int> m_rodesIndex;  //��·�������
	CComboBox m_startComboBox;
	CComboBox m_endComboBox;
	afx_msg void OnBnClickedOk();

		//���ñ�Ҫ���� �ڵ�����
	void Initialize(vector<int> roads);

	void InitializeView(); //��ʼ���Ӵ�����
};
