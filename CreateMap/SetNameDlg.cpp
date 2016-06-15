// SetNameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateMap.h"
#include "SetNameDlg.h"
#include "afxdialogex.h"
#include "ToolsUtil.h"

// SetNameDlg 对话框

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


// SetNameDlg 消息处理程序

//初始化
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

	//Step 1 -----------检查是否跟已有名重复--------------
	for(auto &winame:m_winmapname){
		if(strcmp(tstr,winame.name)==0){
			isHave=true;
			break;
		}
	}
	if(isHave){
		AfxMessageBox(L"命名重复",MB_OK);
		return;
	}

    CString* pStrName=new CString(strName);
	::SendMessage(GetParent()->GetSafeHwnd(),MAP_SETNAME,0,(LPARAM)pStrName);//向父窗体通知获取ID
	CDialogEx::OnOK();
}
