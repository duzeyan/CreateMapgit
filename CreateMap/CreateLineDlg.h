#pragma once

#include "NJUSTMap.h"
#include"DrawMapMark.h"
#include "afxwin.h"
#include <iostream>
#include "resource.h"
using namespace std;

// CreateLineDlg �Ի���
//�����������ô� 

//1.��������һ����·��Ϣ(����ģʽ)  ʹ��Initialize


//2.�����޸�һ����·����Ϣ(�޸�ģʽ) ��ʱֻ��Ҫ�򵥵ı������Լ���


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
	bool m_isModify;        //�Ƿ����޸�ģʽ ������Ϊ����ģʽ
	MAP_ROAD m_node;        //���ڴ洢��ԭʼ�ṹ������
public:
	//���ñ�Ҫ���� �ڵ�����
	void Initialize(vector<int> nodes);

	void InitializeView();		 //��ʼ���Ӵ�����

	bool packageMAP_ROAD(MAP_ROAD *package); //�������

	void setModify(const MAP_ROAD &package);    //ʹ���޸�ģʽ ����Ҫ����ԭ������Ϣ

	

	afx_msg void OnBnClickedLinedlgOk();
};
