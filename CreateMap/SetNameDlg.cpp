// SetNameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CreateMap.h"
#include "SetNameDlg.h"
#include "afxdialogex.h"
#include "ToolsUtil.h"

// SetNameDlg �Ի���

IMPLEMENT_DYNAMIC(SetNameDlg, CDialogEx)

SetNameDlg::SetNameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetNameDlg::IDD, pParent)
{

}

SetNameDlg::~SetNameDlg()
{
}

void SetNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITNAME, m_edit);
}


BEGIN_MESSAGE_MAP(SetNameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SetNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SetNameDlg ��Ϣ�������

//��ʼ��
void SetNameDlg::init(const vector<MODEL_WINMAPNAME> &winmapname){
	unsigned int i;
	for(i=0;i<winmapname.size();i++){
		m_winmapname.push_back(winmapname[i]);
	}
}

void SetNameDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	char tstr[255];
	bool isHave=false;
	CString strName;
	m_edit.GetWindowTextW(strName);
	ToolsUtil::WtoA(tstr,255,&strName);

	//Step 1 -----------����Ƿ���������ظ�--------------
	for(auto &winame:m_winmapname){
		if(strcmp(tstr,winame.name)==0){
			isHave=true;
			break;
		}
	}
	if(isHave){
		AfxMessageBox(L"�����ظ�",MB_OK);
		return;
	}

    CString* pStrName=new CString(strName);
	::SendMessage(GetParent()->GetSafeHwnd(),MAP_SETNAME,0,(LPARAM)pStrName);//�򸸴���֪ͨ��ȡID
	CDialogEx::OnOK();
}
