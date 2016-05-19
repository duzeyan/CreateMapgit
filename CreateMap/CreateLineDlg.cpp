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
	m_isModify=false; //默认创造模式
	InitializeView();
}


void CreateLineDlg::InitializeView(){
	//设置路口列表
	unsigned int i;
	CString strPrint;
	m_startBox.ResetContent();
	m_endBox.ResetContent();
	for(i=0;i<m_nodesIndex.size();i++){
		strPrint.Format(L"路口:%d",m_nodesIndex[i]+1);
		m_startBox.AddString(strPrint);
		m_endBox.AddString(strPrint);
	}
	//设置默认参数
	((CButton*)GetDlgItem(IDC_STRUC2))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_STRUC3))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_STRUC1))->SetCheck(TRUE); //默认选择 结构化道路

	((CButton*)GetDlgItem(IDC_SRC1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_SRC2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_SRC3))->SetCheck(TRUE);	//默认选择   人工编辑
	
	((CEdit*)GetDlgItem(IDC_EDITSPEED))->SetWindowTextW(L"20"); //建议车速
	((CEdit*)GetDlgItem(IDC_ROADNUM))->SetWindowTextW(L"2");    //车道个数
}

//根据传进来的数据 重新填写界面表格
void CreateLineDlg::setModify(const MAP_ROAD &package){
	m_isModify=true;
	m_node=package;

	////根据属性填表

	//设置起始路口ID
	CString strTemp;
	int defSel=-1; 
	strTemp.Format(L"路口:%d",package.idstart-START_NODE_ID+1);
	defSel=m_startBox.FindStringExact(0,strTemp);
	assert(defSel!=-1);
	m_startBox.SetCurSel(defSel);

	//设置终点路口ID
	strTemp.Format(L"路口:%d",package.idend-START_NODE_ID+1);
	defSel=m_endBox.FindStringExact(0,strTemp);
	assert(defSel!=-1);
	m_endBox.SetCurSel(defSel);


	//设置结构化参数
	((CButton*)GetDlgItem(IDC_STRUC1))->SetCheck(FALSE); //清除默认选项
	switch (package.structure)
	{
	case NJUST_MAP_STRUCTURED_ROAD:
			((CButton*)GetDlgItem(IDC_STRUC1))->SetCheck(TRUE); //结构化
			break;
	case NJUST_MAP_HALF_STRUCTURED_ROAD:
			((CButton*)GetDlgItem(IDC_STRUC2))->SetCheck(TRUE); //半结构化
			break;
	case NJUST_MAP_NON_STRUCTURED_ROAD:
			((CButton*)GetDlgItem(IDC_STRUC3))->SetCheck(TRUE); //非结构化
			break;
	default:
		break;
	}
	

	//设置GPS来源参数
	((CButton*)GetDlgItem(IDC_SRC3))->SetCheck(FALSE); //清除默认选项
	switch (package.GPSDataFrom)
	{
	case NJUST_MAP_GPS_FROM_CAR:
			((CButton*)GetDlgItem(IDC_SRC1))->SetCheck(TRUE); //车采
			break;
	case NJUST_MAP_GPS_FROM_HAND_DEVICE:
			((CButton*)GetDlgItem(IDC_SRC2))->SetCheck(TRUE); //手持
			break;
	case NJUST_MAP_GPS_FROM_HAND_DRAW:
			((CButton*)GetDlgItem(IDC_SRC3))->SetCheck(TRUE); //手绘
			break;
	default:
		break;
	}

	CString strShow;
	//设置建议车速
	strShow.Format(L"%d",package.idealspeed);
	((CEdit*)GetDlgItem(IDC_EDITSPEED))->SetWindowTextW(strShow);

	//设置车道个数
	strShow.Format(L"%d",package.chedaonum);
	((CEdit*)GetDlgItem(IDC_ROADNUM))->SetWindowTextW(strShow);
}


bool CreateLineDlg::packageMAP_ROAD(MAP_ROAD *package){
	//获取 ID
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
		return false;
	}
	package->idstart=id1;
	package->idend=id2;

	//获取 速度，道路个数
	((CEdit*)GetDlgItem(IDC_EDITSPEED))->GetWindowTextW(strShow);
	int speedi=_ttoi(strShow);
	if(speedi<0||speedi>200){
		AfxMessageBox(L"速度需要设置在[0,200]中",MB_OK);
		return false;
	}
	package->idealspeed=speedi;

	((CEdit*)GetDlgItem(IDC_ROADNUM))->GetWindowTextW(strShow);
	int numi=_ttoi(strShow);
	if(numi<0||numi>10){
		AfxMessageBox(L"车道数目需要设置在[0,10]中",MB_OK);
		return false;
	}
	package->chedaonum=numi;
	
	//获取 结构属性 界面上设置3选1
	if(((CButton*)GetDlgItem(IDC_STRUC1))->GetCheck()){
		package->structure=NJUST_MAP_STRUCTURED_ROAD; //结构化
	}
	if(((CButton*)GetDlgItem(IDC_STRUC2))->GetCheck()){
		package->structure=NJUST_MAP_HALF_STRUCTURED_ROAD;  //半结构
	}
	if(((CButton*)GetDlgItem(IDC_STRUC3))->GetCheck()){
		package->structure=NJUST_MAP_NON_STRUCTURED_ROAD;  //非结构化
	}

	//获得 数据来源 界面上设置3选1
	if(((CButton*)GetDlgItem(IDC_SRC1))->GetCheck()){
		package->GPSDataFrom=NJUST_MAP_GPS_FROM_CAR;         //车采
	}
	if(((CButton*)GetDlgItem(IDC_SRC2))->GetCheck()){
		package->GPSDataFrom=NJUST_MAP_GPS_FROM_HAND_DEVICE; //手持
	}
	if(((CButton*)GetDlgItem(IDC_SRC3))->GetCheck()){
		package->GPSDataFrom=NJUST_MAP_GPS_FROM_HAND_DRAW;  //手绘
	}

	return true;
}

void CreateLineDlg::OnBnClickedLinedlgOk()
{
	MAP_ROAD *package=new MAP_ROAD(); 
	if(packageMAP_ROAD(package)){ //输入有效
		if(!m_isModify){ //创建模式{
			::SendMessage(GetParent()->GetSafeHwnd(),MAP_SETLINE_2ID,0,(LPARAM)package);//向父窗体通知 合并
		}
		else{
			package->idself=m_node.idself;  //需要ID去搜索修改的道路结构
			m_isModify=false; //恢复到创建模式。除非指认为修改模式
			::SendMessage(GetParent()->GetSafeHwnd(),MAP_MODIFY_LINE,0,(LPARAM)package);//向父窗体通知 修改属性
		}
		CDialogEx::OnOK();
	}
}
