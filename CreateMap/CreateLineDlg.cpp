// CreateLineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateMap.h"
#include "CreateLineDlg.h"
#include "afxdialogex.h"
#include "CreateMapDlg.h"

// CreateLineDlg 对话框

IMPLEMENT_DYNAMIC(CreateLineDlg, CDialogEx)

CreateLineDlg::CreateLineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CreateLineDlg::IDD, pParent)
{

}

CreateLineDlg::~CreateLineDlg()
{
}

void CreateLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_COMBO, m_startBox);
	DDX_Control(pDX, IDC_END_COMBO, m_endBox);
}


BEGIN_MESSAGE_MAP(CreateLineDlg, CDialogEx)
	ON_BN_CLICKED(IDC_LINEDLG_OK, &CreateLineDlg::OnBnClickedLinedlgOk)
END_MESSAGE_MAP()


// CreateLineDlg 消息处理程序


//内部处理函数

void CreateLineDlg::Initialize(vector<int> nodes){
	m_nodesIndex=nodes;
	InitializeView();
}


void CreateLineDlg::InitializeView(){
	unsigned int i;
	CString strPrint;
	m_startBox.ResetContent();
	m_endBox.ResetContent();
	for(i=0;i<m_nodesIndex.size();i++){
		strPrint.Format(L"道路:%d",m_nodesIndex[i]+1);
		m_startBox.AddString(strPrint);
		m_endBox.AddString(strPrint);
	}
}

void CreateLineDlg::OnBnClickedLinedlgOk()
{
	CString strShow,strR;
	int id1,id2;
	int which=m_startBox.GetCurSel();
	m_startBox.GetLBText(which,strShow);
	AfxExtractSubString(strR,strShow,1,':');
	id1=_ttoi(strR);

	which=m_endBox.GetCurSel();
	m_endBox.GetLBText(which,strShow);
	AfxExtractSubString(strR,strShow,1,':');
	id2=_ttoi(strR);
	if(id1==id2){
		AfxMessageBox(L"道路两端的路口编号重复，重新选择",MB_OK);
	}else{
		//((CCreateMapDlg*)GetDlgItem(IDD_CREATEMAP_DIALOG))->m_Line2ID.SetPoint(id1,id2);//给父窗口传值
		auto *pPoint=new CPoint(id1,id2);
		::SendMessage(GetParent()->GetSafeHwnd(),MAP_SETLINE_2ID,0,(LPARAM)pPoint);//向父窗体通知
		CDialogEx::OnOK();
	}
	
}
