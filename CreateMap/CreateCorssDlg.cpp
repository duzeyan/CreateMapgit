// CreateCorssDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CreateMap.h"
#include "CreateCorssDlg.h"
#include "afxdialogex.h"


// CreateCorssDlg �Ի���

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


// CreateCorssDlg ��Ϣ�������




void CreateCorssDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
