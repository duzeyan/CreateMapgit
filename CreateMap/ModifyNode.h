#pragma once

#include"MAP__BASIC_data.h"
// ModifyNode �Ի���

class ModifyNode : public CDialogEx
{
	DECLARE_DYNAMIC(ModifyNode)

public:
	ModifyNode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ModifyNode();

// �Ի�������
	enum { IDD = IDD_MODIFYNODE_DIALOG };

public :
	void setValue(MAP_NODE node);   //����ԭʼ����
	void initView();			    //��ʼ���Ӵ�

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_radioNodeType;
	afx_msg void OnBnClickedUnknow();
};
