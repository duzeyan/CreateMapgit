// CreateCorssDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateMap.h"
#include "CreateCrossDlg.h"
#include "afxdialogex.h"
#include "CreateMapDlg.h"

// CreateCrossDlg 对话框

IMPLEMENT_DYNAMIC(CreateCrossDlg, CDialogEx)

CreateCrossDlg::CreateCrossDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CreateCrossDlg::IDD, pParent)
{

}

CreateCrossDlg::~CreateCrossDlg()
{
}

void CreateCrossDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_COMBO1, m_startComboBox);
	DDX_Control(pDX, IDC_END_COMBO1, m_endComboBox);
}


BEGIN_MESSAGE_MAP(CreateCrossDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CreateCrossDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CreateCrossDlg 消息处理程序




void CreateCrossDlg::OnBnClickedOk()
{
	CString strShow,strR;
	int id1,id2;
	int which=m_startComboBox.GetCurSel();
	m_startComboBox.GetLBText(which,strShow);
	AfxExtractSubString(strR,strShow,1,':');
	id1=_ttoi(strR);

	which=m_endComboBox.GetCurSel();
	m_endComboBox.GetLBText(which,strShow);
	AfxExtractSubString(strR,strShow,1,':');
	id2=_ttoi(strR);
	if(id1==id2){
		AfxMessageBox(L"道路两端的道路编号重复，重新选择",MB_OK);
	}else{
		//((CCreateMapDlg*)GetDlgItem(IDD_CREATEMAP_DIALOG))->m_Line2ID.SetPoint(id1,id2);//给父窗口传值
		auto *pPoint=new CPoint(id1,id2);
		::SendMessage(GetParent()->GetSafeHwnd(),MAP_SETCROSS_2ID,0,(LPARAM)pPoint);//向父窗体通知
		CDialogEx::OnOK();
	}

	CDialogEx::OnOK();
}



//设置必要参数 道路ID序列
void CreateCrossDlg::Initialize(vector<int> roads){
	m_rodesIndex=roads;
	InitializeView();
}

//初始化下拉框等
void CreateCrossDlg::InitializeView(){
	unsigned int i;
	CString strPrint;
	m_startComboBox.ResetContent();
	m_endComboBox.ResetContent();
	for(i=0;i<m_rodesIndex.size();i++){
		strPrint.Format(L"道路:%d",m_rodesIndex[i]+1);
		m_startComboBox.AddString(strPrint);
		m_endComboBox.AddString(strPrint);
	}
}


