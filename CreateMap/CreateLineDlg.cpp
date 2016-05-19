// CreateLineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CreateMap.h"
#include "CreateLineDlg.h"
#include "afxdialogex.h"
#include "CreateMapDlg.h"

// CreateLineDlg �Ի���

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


// CreateLineDlg ��Ϣ�������


//�ڲ�������

void CreateLineDlg::Initialize(vector<int> nodes){
	m_nodesIndex=nodes;
	m_isModify=false; //Ĭ�ϴ���ģʽ
	InitializeView();
}


void CreateLineDlg::InitializeView(){
	//����·���б�
	unsigned int i;
	CString strPrint;
	m_startBox.ResetContent();
	m_endBox.ResetContent();
	for(i=0;i<m_nodesIndex.size();i++){
		strPrint.Format(L"·��:%d",m_nodesIndex[i]+1);
		m_startBox.AddString(strPrint);
		m_endBox.AddString(strPrint);
	}
	//����Ĭ�ϲ���
	((CButton*)GetDlgItem(IDC_STRUC2))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_STRUC3))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_STRUC1))->SetCheck(TRUE); //Ĭ��ѡ�� �ṹ����·

	((CButton*)GetDlgItem(IDC_SRC1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_SRC2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_SRC3))->SetCheck(TRUE);	//Ĭ��ѡ��   �˹��༭
	
	((CEdit*)GetDlgItem(IDC_EDITSPEED))->SetWindowTextW(L"20"); //���鳵��
	((CEdit*)GetDlgItem(IDC_ROADNUM))->SetWindowTextW(L"2");    //��������
}

//���ݴ����������� ������д������
void CreateLineDlg::setModify(const MAP_ROAD &package){
	m_isModify=true;
	m_node=package;

	////�����������

	//������ʼ·��ID
	CString strTemp;
	int defSel=-1; 
	strTemp.Format(L"·��:%d",package.idstart-START_NODE_ID+1);
	defSel=m_startBox.FindStringExact(0,strTemp);
	assert(defSel!=-1);
	m_startBox.SetCurSel(defSel);

	//�����յ�·��ID
	strTemp.Format(L"·��:%d",package.idend-START_NODE_ID+1);
	defSel=m_endBox.FindStringExact(0,strTemp);
	assert(defSel!=-1);
	m_endBox.SetCurSel(defSel);


	//���ýṹ������
	((CButton*)GetDlgItem(IDC_STRUC1))->SetCheck(FALSE); //���Ĭ��ѡ��
	switch (package.structure)
	{
	case NJUST_MAP_STRUCTURED_ROAD:
			((CButton*)GetDlgItem(IDC_STRUC1))->SetCheck(TRUE); //�ṹ��
			break;
	case NJUST_MAP_HALF_STRUCTURED_ROAD:
			((CButton*)GetDlgItem(IDC_STRUC2))->SetCheck(TRUE); //��ṹ��
			break;
	case NJUST_MAP_NON_STRUCTURED_ROAD:
			((CButton*)GetDlgItem(IDC_STRUC3))->SetCheck(TRUE); //�ǽṹ��
			break;
	default:
		break;
	}
	

	//����GPS��Դ����
	((CButton*)GetDlgItem(IDC_SRC3))->SetCheck(FALSE); //���Ĭ��ѡ��
	switch (package.GPSDataFrom)
	{
	case NJUST_MAP_GPS_FROM_CAR:
			((CButton*)GetDlgItem(IDC_SRC1))->SetCheck(TRUE); //����
			break;
	case NJUST_MAP_GPS_FROM_HAND_DEVICE:
			((CButton*)GetDlgItem(IDC_SRC2))->SetCheck(TRUE); //�ֳ�
			break;
	case NJUST_MAP_GPS_FROM_HAND_DRAW:
			((CButton*)GetDlgItem(IDC_SRC3))->SetCheck(TRUE); //�ֻ�
			break;
	default:
		break;
	}

	CString strShow;
	//���ý��鳵��
	strShow.Format(L"%d",package.idealspeed);
	((CEdit*)GetDlgItem(IDC_EDITSPEED))->SetWindowTextW(strShow);

	//���ó�������
	strShow.Format(L"%d",package.chedaonum);
	((CEdit*)GetDlgItem(IDC_ROADNUM))->SetWindowTextW(strShow);
}


bool CreateLineDlg::packageMAP_ROAD(MAP_ROAD *package){
	//��ȡ ID
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
		AfxMessageBox(L"��·���˵�·�ڱ���ظ�������ѡ��",MB_OK);
		return false;
	}
	package->idstart=id1;
	package->idend=id2;

	//��ȡ �ٶȣ���·����
	((CEdit*)GetDlgItem(IDC_EDITSPEED))->GetWindowTextW(strShow);
	int speedi=_ttoi(strShow);
	if(speedi<0||speedi>200){
		AfxMessageBox(L"�ٶ���Ҫ������[0,200]��",MB_OK);
		return false;
	}
	package->idealspeed=speedi;

	((CEdit*)GetDlgItem(IDC_ROADNUM))->GetWindowTextW(strShow);
	int numi=_ttoi(strShow);
	if(numi<0||numi>10){
		AfxMessageBox(L"������Ŀ��Ҫ������[0,10]��",MB_OK);
		return false;
	}
	package->chedaonum=numi;
	
	//��ȡ �ṹ���� ����������3ѡ1
	if(((CButton*)GetDlgItem(IDC_STRUC1))->GetCheck()){
		package->structure=NJUST_MAP_STRUCTURED_ROAD; //�ṹ��
	}
	if(((CButton*)GetDlgItem(IDC_STRUC2))->GetCheck()){
		package->structure=NJUST_MAP_HALF_STRUCTURED_ROAD;  //��ṹ
	}
	if(((CButton*)GetDlgItem(IDC_STRUC3))->GetCheck()){
		package->structure=NJUST_MAP_NON_STRUCTURED_ROAD;  //�ǽṹ��
	}

	//��� ������Դ ����������3ѡ1
	if(((CButton*)GetDlgItem(IDC_SRC1))->GetCheck()){
		package->GPSDataFrom=NJUST_MAP_GPS_FROM_CAR;         //����
	}
	if(((CButton*)GetDlgItem(IDC_SRC2))->GetCheck()){
		package->GPSDataFrom=NJUST_MAP_GPS_FROM_HAND_DEVICE; //�ֳ�
	}
	if(((CButton*)GetDlgItem(IDC_SRC3))->GetCheck()){
		package->GPSDataFrom=NJUST_MAP_GPS_FROM_HAND_DRAW;  //�ֻ�
	}

	return true;
}

void CreateLineDlg::OnBnClickedLinedlgOk()
{
	MAP_ROAD *package=new MAP_ROAD(); 
	if(packageMAP_ROAD(package)){ //������Ч
		if(!m_isModify){ //����ģʽ{
			::SendMessage(GetParent()->GetSafeHwnd(),MAP_SETLINE_2ID,0,(LPARAM)package);//�򸸴���֪ͨ �ϲ�
		}
		else{
			package->idself=m_node.idself;  //��ҪIDȥ�����޸ĵĵ�·�ṹ
			m_isModify=false; //�ָ�������ģʽ������ָ��Ϊ�޸�ģʽ
			::SendMessage(GetParent()->GetSafeHwnd(),MAP_MODIFY_LINE,0,(LPARAM)package);//�򸸴���֪ͨ �޸�����
		}
		CDialogEx::OnOK();
	}
}
