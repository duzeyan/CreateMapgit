#pragma once
#include "NJUSTMap.h"
#include"DrawMapMark.h"
#include "afxwin.h"
#include <iostream>
using namespace std;

// CreateCrossDlg �Ի���
class CreateCrossDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateCrossDlg)

public:
	CreateCrossDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CreateCrossDlg();

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

	//���ñ�Ҫ���� ��·ID����
	void Initialize(vector<int> roads);

	void InitializeView(); //��ʼ���Ӵ�����
};
