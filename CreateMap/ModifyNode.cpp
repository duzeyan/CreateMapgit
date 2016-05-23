// ModifyNode.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateMap.h"
#include "ModifyNode.h"
#include "afxdialogex.h"


// ModifyNode 对话框

IMPLEMENT_DYNAMIC(ModifyNode, CDialogEx)

ModifyNode::ModifyNode(CWnd* pParent /*=NULL*/)
	: CDialogEx(ModifyNode::IDD, pParent)
	, m_radioNodeType(0)
	, m_radioHLDP(0)
	, m_radioHLDType(0)
	, m_radioz(0)
{

}

ModifyNode::~ModifyNode()
{
}

void ModifyNode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_UNKNOW, m_radioNodeType);
	DDX_Radio(pDX, IDC_UNKNOW2, m_radioHLDP);
	DDX_Radio(pDX, IDC_NODEL, m_radioHLDType);
	DDX_Radio(pDX, IDC_HAVEZ, m_radioz);
}


void ModifyNode::setValue(MAP_NODE node){  //传递原始数据
	//路口类型
	m_node=node;
	switch(node.lukou){
		case 0:
			m_radioNodeType=0; 
			break;
		case 1:
			m_radioNodeType=1;
			break;
		case 2:
			m_radioNodeType=2;
			break;
		case 3:
			m_radioNodeType=3;
			break;
		case 4:
			m_radioNodeType=4;
			break;
		default:
			break;
	}

	//红绿灯位置
	switch (node.HLD)
	{
	case 0:
		m_radioHLDP=0;
		break;
	case 1:
		m_radioHLDP=1;
		break;
	case 2:
		m_radioHLDP=2;
		break;
	case 3:
		m_radioHLDP=3;
		break;
	default:
		break;
	}

	//红绿灯类型
	switch (node.HLDkind)
	{
	case 0:
		m_radioHLDType=0;
		break;
	case 1:
		m_radioHLDType=1;
		break;
	case 2:
		m_radioHLDType=2;
		break;
	case 3:
		m_radioHLDType=3;
		break;
	case 4:
		m_radioHLDType=4;
		break;
	case 5:
		m_radioHLDType=5;
		break;
	default:
		break;
	}

	//是否有斑马线
	switch (node.zebra)
	{
	case 0:
		m_radioz=1; //有
		break;
	case 1:
		m_radioz=0; //没有
		break;
	default:
		break;
	}
	UpdateData(FALSE);
}

void ModifyNode::initView(){			    //初始化视窗

}

BEGIN_MESSAGE_MAP(ModifyNode, CDialogEx)
	ON_BN_CLICKED(IDC_UNKNOW, &ModifyNode::OnBnClickedUnknow) //路口类型
	ON_BN_CLICKED(IDC_CROSSROADS, &ModifyNode::OnBnClickedUnknow)
	ON_BN_CLICKED(IDC_TNODE, &ModifyNode::OnBnClickedUnknow)
	ON_BN_CLICKED(IDC_YNODE, &ModifyNode::OnBnClickedUnknow) 
	ON_BN_CLICKED(IDC_RIGHTANGLENODE, &ModifyNode::OnBnClickedUnknow) 
	ON_BN_CLICKED(IDC_STRAIGHTLINENODE, &ModifyNode::OnBnClickedUnknow)


	ON_BN_CLICKED(IDC_UNKNOW2, &ModifyNode::OnBnClickedUnknow2)
	ON_BN_CLICKED(IDC_NODEL, &ModifyNode::OnBnClickedNodel)
	ON_BN_CLICKED(IDC_HAVEZ, &ModifyNode::OnBnClickedHavez)
	ON_BN_CLICKED(IDOK, &ModifyNode::OnBnClickedOk)
END_MESSAGE_MAP()


// ModifyNode 消息处理程序

//点击路口类型
void ModifyNode::OnBnClickedUnknow()
{
	UpdateData(TRUE);
	switch (m_radioNodeType)
	{
	case 0:
		m_node.lukou=0; 
		break;
	case 1:
		m_node.lukou=1;
		break;
	case 2:
		m_node.lukou=2;
		break;
	case 3:
		m_node.lukou=3;
		break;
	case 4:
		m_node.lukou=4;
		break;
	default:
		break;
	}
}

//红绿位置
void ModifyNode::OnBnClickedUnknow2()
{
	UpdateData(TRUE);
	switch (m_radioHLDP)
	{
	case 0:
		m_node.HLD=0;
		break;
	case 1:
		m_node.HLD=1;
		break;
	case 2:
		m_node.HLD=2;
		break;
	case 3:
		m_node.HLD=3;
		break;
	default:
		break;
	}
}

//红绿灯类型
void ModifyNode::OnBnClickedNodel()
{
	UpdateData(TRUE);
	switch (m_radioHLDType)
	{
	case 0:
		m_node.HLDkind=0;
		break;
	case 1:
		m_node.HLDkind=1;
		break;
	case 2:
		m_node.HLDkind=2;
		break;
	case 3:
		m_node.HLDkind=3;
		break;
	case 4:
		m_node.HLDkind=4;
		break;
	case 5:
		m_node.HLDkind=5;
		break;
	default:
		break;
	}
}


void ModifyNode::OnBnClickedHavez()
{
	UpdateData(TRUE);
	switch (m_radioz)
	{
	case 0:
		m_node.zebra=1; //有
		break;
	case 1:
		m_node.zebra=0; //没有
		break;
	default:
		break;
	}
}


void ModifyNode::OnBnClickedOk()
{
	auto node=new MAP_NODE();
	*node=m_node;
	::SendMessage(GetParent()->GetSafeHwnd(),MAP_MODIF_NODE,0,(LPARAM)node);//向父窗体通知
	CDialogEx::OnOK();
}
