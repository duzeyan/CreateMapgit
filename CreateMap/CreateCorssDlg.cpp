// CreateCorssDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateMap.h"
#include "CreateCorssDlg.h"
#include "afxdialogex.h"


// CreateCorssDlg 对话框

IMPLEMENT_DYNAMIC(CreateCorssDlg, CDialogEx)

CreateCorssDlg::CreateCorssDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CreateCorssDlg::IDD, pParent)
{

}

CreateCorssDlg::~CreateCorssDlg()
{
}

void CreateCorssDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_COMBO1, m_startComboBox);
	DDX_Control(pDX, IDC_END_COMBO1, m_endComboBox);
}


BEGIN_MESSAGE_MAP(CreateCorssDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CreateCorssDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CreateCorssDlg 消息处理程序




void CreateCorssDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
