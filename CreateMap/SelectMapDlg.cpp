// SelectMapDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CreateMap.h"
#include "SelectMapDlg.h"
#include "afxdialogex.h"


// SelectMapDlg �Ի���

IMPLEMENT_DYNAMIC(SelectMapDlg, CDialogEx)

SelectMapDlg::SelectMapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SelectMapDlg::IDD, pParent)
{

}

SelectMapDlg::~SelectMapDlg()
{
}

void SelectMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SELMAP, m_comboxMapList);
}




BEGIN_MESSAGE_MAP(SelectMapDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SelectMapDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SelectMapDlg ��Ϣ�������

//��ʼ��
void SelectMapDlg::init(const vector<MODEL_WINMAPNAME> &winmapname){
	unsigned int i;
	m_comboxMapList.ResetContent();
	m_winmapname.clear();
	for(i=0;i<winmapname.size();i++){
		m_winmapname.push_back(winmapname[i]);
		CString strPrint(winmapname[i].name);
		m_comboxMapList.InsertString(i,strPrint);
	}
}

void SelectMapDlg::OnBnClickedOk()
{
	int index=m_comboxMapList.GetCurSel();
	if(index!=LB_ERR){
		int* t=new int;
		*t=m_winmapname[index].ID;;
		::SendMessage(GetParent()->GetSafeHwnd(),MAP_SELDBMAP,0,(LPARAM)t);//�򸸴���֪ͨ��ȡID
	}
	CDialogEx::OnOK();
}
