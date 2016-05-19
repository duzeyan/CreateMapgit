// ModifyNode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CreateMap.h"
#include "ModifyNode.h"
#include "afxdialogex.h"


// ModifyNode �Ի���

IMPLEMENT_DYNAMIC(ModifyNode, CDialogEx)

ModifyNode::ModifyNode(CWnd* pParent /*=NULL*/)
	: CDialogEx(ModifyNode::IDD, pParent)
	, m_radioNodeType(0)
{

}

ModifyNode::~ModifyNode()
{
}

void ModifyNode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_UNKNOW, m_radioNodeType);
}


void ModifyNode::setValue(MAP_NODE node){  //����ԭʼ����
}

void ModifyNode::initView(){			    //��ʼ���Ӵ�

}

BEGIN_MESSAGE_MAP(ModifyNode, CDialogEx)
	ON_BN_CLICKED(IDC_UNKNOW, &ModifyNode::OnBnClickedUnknow) //·������
	ON_BN_CLICKED(IDC_CROSSROADS, &ModifyNode::OnBnClickedUnknow)
	ON_BN_CLICKED(IDC_TNODE, &ModifyNode::OnBnClickedUnknow)
	ON_BN_CLICKED(IDC_YNODE, &ModifyNode::OnBnClickedUnknow) 
	ON_BN_CLICKED(IDC_RIGHTANGLENODE, &ModifyNode::OnBnClickedUnknow) 
	ON_BN_CLICKED(IDC_STRAIGHTLINENODE, &ModifyNode::OnBnClickedUnknow)


END_MESSAGE_MAP()


// ModifyNode ��Ϣ�������

//���·������
void ModifyNode::OnBnClickedUnknow()
{
	UpdateData(TRUE);
	switch (m_radioNodeType)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}
