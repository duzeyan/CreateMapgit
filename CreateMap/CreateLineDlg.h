#pragma once

#include "NJUSTMap.h"
#include"DrawMapMark.h"
#include "afxwin.h"
#include <iostream>
#include "resource.h"
using namespace std;

// CreateLineDlg �Ի���

class CreateLineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateLineDlg)

public:
	CreateLineDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CreateLineDlg();

// �Ի�������
	enum { IDD = IDD_CREATELINE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	vector<int> m_nodesIndex;  //·�ڱ������
	CComboBox m_startBox;   //ͷ���б�
	CComboBox m_endBox;     //β���б�
public:
	//���ñ�Ҫ���� �ڵ�����
	void Initialize(vector<int> nodes);

	void InitializeView(); //��ʼ���Ӵ�����

	
	afx_msg void OnBnClickedLinedlgOk();
};
