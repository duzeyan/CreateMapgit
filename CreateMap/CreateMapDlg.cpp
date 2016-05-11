
// CreateMapDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CreateMap.h"
#include "CreateMapDlg.h"
#include "afxdialogex.h"

#include <stdio.h>

//#include"GDITest.h"
//#include"DrawMapMark.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCreateMapDlg �Ի���



CCreateMapDlg::CCreateMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateMapDlg::IDD, pParent)
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCreateMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC_MAIN, m_picMain);
	DDX_Control(pDX, IDC_LISTRECORD, m_listRecord);
	DDX_Control(pDX, IDC_LIST1, m_listMap);
}

BEGIN_MESSAGE_MAP(CCreateMapDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
//	ON_WM_KEYDOWN()
//ON_BN_CLICKED(IDC_BUTTONUP, &CCreateMapDlg::OnBnClickedButtonup)
//ON_BN_CLICKED(IDC_BUTTONLEFT, &CCreateMapDlg::OnBnClickedButtonleft)
//ON_BN_CLICKED(IDC_BUTTONRIGHT, &CCreateMapDlg::OnBnClickedButtonright)
//ON_BN_CLICKED(IDC_BUTTONUPLEFT, &CCreateMapDlg::OnBnClickedButtonupleft)
ON_BN_CLICKED(IDC_BUTTON2, &CCreateMapDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTNBEZIER, &CCreateMapDlg::OnBnClickedButnbezier)


ON_BN_CLICKED(IDC_BUTNPOINTS, &CCreateMapDlg::OnBnClickedButnpoints)
ON_BN_CLICKED(IDC_BUTTONDEL, &CCreateMapDlg::OnBnClickedButtondel)
ON_COMMAND(ID_MENU_DELDRAW, &CCreateMapDlg::OnBnClickedButtondel)
ON_BN_CLICKED(IDC_BUTTONSHOW, &CCreateMapDlg::OnBnClickedButtonshow)
ON_COMMAND(ID_MENU_SHOWDRAW, &CCreateMapDlg::OnBnClickedButtonshow)
ON_BN_CLICKED(IDC_BUTNMARKNODE, &CCreateMapDlg::OnBnClickedButnmarknode)
ON_BN_CLICKED(IDC_BUTTONMEG, &CCreateMapDlg::OnBnClickedButtonmeg)      //���ɵ�·
ON_COMMAND(ID_MENU_MEGROAD, &CCreateMapDlg::OnBnClickedButtonmeg)  
ON_MESSAGE(MAP_SETLINE_2ID, &CCreateMapDlg::OnMapSetline2id)
ON_LBN_SELCHANGE(IDC_LISTRECORD, &CCreateMapDlg::OnLbnSelchangeListrecord)
ON_BN_CLICKED(IDC_BUTTONSHOWROAD, &CCreateMapDlg::OnBnClickedButtonshowroad)//��ʾ��·
ON_COMMAND(ID_MENU_SHOW1, &CCreateMapDlg::OnBnClickedButtonshowroad)
ON_COMMAND(ID_MENU_REFRESH1, &CCreateMapDlg::OnBnClickedButtonf5)
ON_BN_CLICKED(IDC_BUTTONF5, &CCreateMapDlg::OnBnClickedButtonf5)
ON_BN_CLICKED(IDC_BUTTONDELINE, &CCreateMapDlg::OnBnClickedButtondeline) //ɾ����·
ON_COMMAND(ID_MENU_DEL1, &CCreateMapDlg::OnBnClickedButtondeline)
ON_BN_CLICKED(IDC_BUTTONDELINEOUT, &CCreateMapDlg::OnBnClickedButtondelineout) //�����·
ON_COMMAND(ID_MENU_SAVE1, &CCreateMapDlg::OnBnClickedButtondelineout)
ON_BN_CLICKED(IDCANCEL, &CCreateMapDlg::OnBnClickedCancel)

ON_WM_TIMER()
ON_BN_CLICKED(IDC_BUTTONMEG2, &CCreateMapDlg::OnBnClickedButtonmeg2)
ON_COMMAND(ID_MENU_MEGCROSS, &CCreateMapDlg::OnBnClickedButtonmeg2)
ON_MESSAGE(MAP_SETCROSS_2ID, &CCreateMapDlg::OnMapSetcross2id)

ON_COMMAND(ID_32775, &CCreateMapDlg::OnMenuBuildMap)
ON_COMMAND(ID_32776, &CCreateMapDlg::OnOpenMap)
ON_COMMAND(ID_32777, &CCreateMapDlg::OnSaveMap)
ON_COMMAND(ID_32778, &CCreateMapDlg::OnMenuShowGPS)
ON_WM_CONTEXTMENU()
ON_WM_SIZE()
ON_COMMAND(ID_MENU_P1, &CCreateMapDlg::OnMenuP1)
ON_COMMAND(ID_MENU_P2, &CCreateMapDlg::OnMenuP2)
ON_COMMAND(ID_MENU_DEVIATION, &CCreateMapDlg::OnMenuDeviation)
ON_WM_DESTROY()
ON_COMMAND(ID_RESOLUTION, &CCreateMapDlg::OnResolution)
END_MESSAGE_MAP()

//�ֶ�ע���¼���Ӧ
//BEGIN_MESSAGE_MAP(CCreateMapDlg,CDialog)
//	ON_BN_CLICKED(IDC_LISTRECORD, &CCreateMapDlg::OnBnClickedListBox)
//END_MESSAGE_MAP()

// CCreateMapDlg ��Ϣ�������

BOOL CCreateMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_isMove=false;
	m_loadImage=NULL;
	m_canvas=NULL;
	m_backUpImage=NULL;
	m_isDrawLine=false;
	CWnd *pWnd=GetDlgItem(IDC_PIC_MAIN);//���pictrue�ؼ����ڵľ��   
	pWnd->GetClientRect(&m_picRect);	//���pictrue�ؼ����ڵľ�������   
	m_pPicDC=pWnd->GetDC();
	m_nowCase=Case_None;
	control_bezier.index=0;
	m_lineDlg=NULL;
	m_crossDlg=NULL;
	m_curMapName="";
	m_Show_cur=0;

	//��ʼ���ؼ�
	initStatusBar();
	initCtlPosition();

	//��ʼ���궨���
	m_RealGPS.x=m_RealGPS.y=0.0f;
	m_getMCInfo.StartUdpCommunication(this);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCreateMapDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCreateMapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCreateMapDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_listRect.GetCount() > 0){
		CRect dlgNow;
		GetWindowRect(&dlgNow);
		POSITION pos = m_listRect.GetHeadPosition();//��һ��������ǶԻ����Rect
		CRect dlgSaved;
		dlgSaved = m_listRect.GetNext(pos);
		ScreenToClient(dlgNow);
		double x = dlgNow.Width() * 1.0 / dlgSaved.Width();//���ݵ�ǰ��֮ǰ����ĶԻ���Ŀ�������
		double y = dlgNow.Height()  *1.0 / dlgSaved.Height();
		ClientToScreen(dlgNow);
		CRect childSaved;
		CWnd* pWnd = GetWindow(GW_CHILD);
		while (pWnd)
		{
			childSaved = m_listRect.GetNext(pos);//���λ�ȡ�Ӵ����Rect
			childSaved.left = (LONG)(dlgNow.left + (childSaved.left - dlgSaved.left)*x);//���ݱ��������ؼ��������Ҿ���Ի���ľ���
			childSaved.right = (LONG)(dlgNow.right + (childSaved.right - dlgSaved.right)*x);
			childSaved.top = (LONG)(dlgNow.top + (childSaved.top - dlgSaved.top)*y);
			childSaved.bottom = (LONG)(dlgNow.bottom + (childSaved.bottom - dlgSaved.bottom)*y);
			ScreenToClient(childSaved);
			pWnd->MoveWindow(childSaved);
 

			InvalidateRect(childSaved);  //�����ػ� �����Ӱ
			pWnd = pWnd->GetNextWindow();
		}

	}
}

//�ƺ���
void CCreateMapDlg::OnDestroy()
{
	//�ر��ض��˿ڼ���
	m_getMCInfo.ShutDownCommunication();

	//�ͷ��ڴ�
	 if(m_statusBar!=NULL) delete m_statusBar;
	if(m_lineDlg!=NULL) delete m_lineDlg;
	if(m_crossDlg!=NULL) delete m_crossDlg; 
	if(m_loadImage!=NULL) delete m_loadImage; 
	if(m_backUpImage!=NULL) delete m_backUpImage; 
	if(m_canvas!=NULL) delete m_canvas; 
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}



//////////////////////////////////////////////////////////////////////////
/// �ؼ��¼���Ӧ
//////////////////////////////////////////////////////////////////////////

//����������
void CCreateMapDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// -- Step 0 -- ��ʼ��
	CRect rect;
	GetDlgItem(IDC_PIC_MAIN)->GetWindowRect(&rect);//��ȡ�ؼ�����ȫ�����λ��
	ScreenToClient(rect);//ת��Ϊ�Ի����ϵ����λ��

	// -- Step 1 -- �ж���궯���Ƿ��ڻ�ͼ������
	if(rect.PtInRect(point)){
		if(!isLoad())
			return;
		switch(m_nowCase){
			// -- Step 1.1 -- ����ֱ��
			case Case_Line:   {
								DlgDrawLine(point,rect);
								break;		 
							   }
			// -- Step 1.2 -- ��������
			case Case_BEZIER:{
								DlgDrawBezier(point,rect);
								break;
							 }
			case Case_Points:{
								DlgDrawPoints(point,rect);
								break;
							 }
			case Case_MarkNode:{
									DlgDrawMark(point,rect);
									break;
							   }
			case Case_getP1:{
									
									setCalibration(point,rect,0);
									m_nowCase=Case_None;
									break;
							}
			case Case_getP2 :{     
									setCalibration(point,rect,1);
									m_nowCase=Case_None;
									break;
							 }
			case Case_Deviation:{
									coumputerDevication(point,rect);
									m_nowCase=Case_None;
									break;
								}
			//Ĭ���ƶ�
			default:{
							m_isMove=true;
							SetClassLong(this->GetSafeHwnd(),
										GCL_HCURSOR , 
										(LONG)LoadCursor(NULL , IDC_HAND));
							m_startPoint.x=point.x;
							m_startPoint.y=point.y;
					}	
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

//��⵽����ƶ�
void CCreateMapDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	
	//������ �� �Ѿ����ع�ͼƬ
	if(m_isMove&&m_loadImage!=NULL){
		 SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_HAND));
		 //�������λ��
		int dx=point.x-m_startPoint.x;
		int dy=point.y-m_startPoint.y;

		//��������ƶ���ʼ��
		m_startPoint.x=point.x;
		m_startPoint.y=point.y;

		int imageW=m_loadImage->GetWidth();
		int imageH=m_loadImage->GetHeight();

		//����ͼ��λ�ÿ������ 
		CWnd *pWnd=GetDlgItem(IDC_PIC_MAIN);//���pictrue�ؼ����ڵľ��     
		//CDC *pDC=pWnd->GetDC();//���pictrue�ؼ���DC     

		//�������λ�ƻ����Ӵ�	
		m_srcRect.left-=dx;m_srcRect.right-=dx;
		m_srcRect.top-=dy;m_srcRect.bottom-=dy;

		////����Ƿ�Խ��
		drawmap::CheckViewInImage(m_srcRect,imageW,imageH,m_picRect.Width(),m_picRect.Height());

		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); //��ͼƬ����Picture�ؼ���ʾ�ľ�������  

	}
	CDialog::OnMouseMove(nFlags, point);
}

//������̧��
void CCreateMapDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�����ƶ�����
	m_isMove=false;
	//������ƸĻؼ�ͷ
	SetClassLong(this->GetSafeHwnd(),
                             GCL_HCURSOR , 
                             (LONG)LoadCursor(NULL , IDC_ARROW));
	CDialog::OnLButtonUp(nFlags, point);
}

//��ֱ��
void CCreateMapDlg::OnBnClickedButton2()
{
	if(!isLoad())
		return;
	m_nowCase=Case_Line;			//�ı��ͼ״̬
}

//����������test
void CCreateMapDlg::OnBnClickedButnbezier()
{
	if(!isLoad())
		return;
	m_nowCase=Case_BEZIER;
}


//mark״̬
void CCreateMapDlg::OnBnClickedButnmarknode()
{
	if(!isLoad())
		return;
	m_nowCase=Case_MarkNode;
}


//�����㼯��
void CCreateMapDlg::OnBnClickedButnpoints()
{
	if(!isLoad())
		return;
	if(Case_None==m_nowCase) // ��һ�ε�
		m_nowCase=Case_Points;
	else if(Case_Points==m_nowCase){
		//��¼�� �滭��������
		DRAW_RECORD dr;
		dr.type=2;
		dr.drawPoints=control_points.points;
		m_records.push_back(dr);
		m_listRecord.AddString(drawmap::PrintRecord(dr));

		control_points.points.clear();
		m_nowCase=Case_None;
	}
		
}

//ɾ��ѡ�еĻ滭��Ŀ
void CCreateMapDlg::OnBnClickedButtondel()
{
	int i;
	int nItemCount = m_listRecord.GetSelCount();
	
    if (0 != nItemCount){
		// --- Step.1 --- ɾ��ѡ�е���Ŀ�Ͷ�Ӧ�Ķ�����¼��
        int* indexBuf = new int[nItemCount];
        memset(indexBuf,0,nItemCount*sizeof(int));
        m_listRecord.GetSelItems(nItemCount,indexBuf);
        for (i = nItemCount-1 ;i >-1;i--)
        {
			if(m_records[indexBuf[i]].type==4){ //cross��Ϣ���ڻ滭����ɾ��
				continue;
			}
            m_listRecord.DeleteString(indexBuf[i]);           //ɾ����ʾ��Ŀ
			if(m_records[indexBuf[i]].type==3){						//ɾ��·�� ��ͼ�ṹ
				m_njustMap.deleteEleByID(true,m_records[indexBuf[i]].id+START_NODE_ID);
			}
			m_records.erase(m_records.begin()+indexBuf[i]);	  //ɾ��������¼��
        }
        delete[]indexBuf;

		// --- Step.2 --- ���ݶ�����¼���ػ�
		DlgReDraw();
    }
}

void CCreateMapDlg::DlgReDraw(){
	if(!isLoad())
		return;

	CRect rect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());
	HDC hdc=m_loadImage->GetDC();
	m_backUpImage->Draw(hdc,rect,rect);		//����Ϊԭͼ
	m_loadImage->ReleaseDC();

	//����ͼ��
	drawmap::DrawByRecord(m_loadImage,m_records,RGB(255,0,0));
	//ˢ��
	m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 
}
	
//���� ѡ����Ŀ
void CCreateMapDlg::OnBnClickedButtonshow(){
		if(!isLoad())
			return;

		CString strTitle;
		GetDlgItem(IDC_BUTTONSHOW)->GetWindowTextW(strTitle);
		if(strTitle==L"��ʾ"){
			// --- Step.1 --- ����ѡ�е���ĿΪ��ɫ
			int i;
			int nItemCount = m_listRecord.GetSelCount();
			if (0 != nItemCount){
				int* indexBuf = new int[nItemCount];
				memset(indexBuf,0,nItemCount*sizeof(int));
				m_listRecord.GetSelItems(nItemCount,indexBuf);
				vector<DRAW_RECORD> tempRecord;			//��ʱ��¼ѡ����Ŀ
				for (i = nItemCount-1 ;i >-1;i--)
				{
					tempRecord.push_back(m_records[indexBuf[i]]);
				}
				delete[]indexBuf;
	
				// --- Step.2 --- ����ѡ���б���Ƹ���
				drawmap::DrawByRecord(m_loadImage,tempRecord,RGB(0,255,255));
				//ˢ��
				m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
	
				GetDlgItem(IDC_BUTTONSHOW)->SetWindowTextW(L"��ԭ");
			}
		}else{
				//���ݶ�����ָ�ԭ�Ȼ��Ƶ�ͼ
				CRect rect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());
				HDC hdc=m_loadImage->GetDC();
				m_backUpImage->Draw(hdc,rect,rect);		//����Ϊԭͼ
				m_loadImage->ReleaseDC();
				drawmap::DrawByRecord(m_loadImage,m_records,RGB(255,0,0));
				//ˢ��
				m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
				GetDlgItem(IDC_BUTTONSHOW)->SetWindowTextW(L"��ʾ");
		}
}


//�ϲ���ť ���ɵ�·��Ϣ
void CCreateMapDlg::OnBnClickedButtonmeg()
{
	if(!isLoad())
		return;

	// --- Step.1 ---��ʼ������
	unsigned int i;
	vector<int> IDIndex;
	for(i=0;i<m_njustMap.nodes.size();i++)
		IDIndex.push_back(m_njustMap.nodes[i].node.idself-START_NODE_ID);

	// --- Step.2 --- �����ӶԻ���
	if(m_lineDlg==NULL){   //��һ�δ���
		m_lineDlg=new CreateLineDlg();
		m_lineDlg->Create(IDD_CREATELINE_DIALOG,this);
	}
	m_lineDlg->Initialize(IDIndex);
	m_lineDlg->ShowWindow(SW_SHOW);	
}


//����·����Ϣ
void CCreateMapDlg::OnBnClickedButtonmeg2()
{
	if(!isLoad())
		return;

	// --- Step.1 ---��ʼ������
	unsigned int i;
	vector<int> IDIndex;
	for(i=0;i<m_njustMap.roads.size();i++)
		IDIndex.push_back(m_njustMap.roads[i].road.idself-START_LINE_ID);

	// --- Step.2 --- �����ӶԻ���
	if(m_crossDlg==NULL){   //��һ�δ���
		m_crossDlg=new CreateCrossDlg();
		m_crossDlg->Create(IDD_CREATECROSS_DIALOG,this);
	}
	m_crossDlg->Initialize(IDIndex);
	m_crossDlg->ShowWindow(SW_SHOW);	
}


//��ʾ��·
void CCreateMapDlg::OnBnClickedButtonshowroad()
{
	if(!isLoad())
		return;

	unsigned int k;
	int index=m_listMap.GetCurSel();
	if(index!=LB_ERR){
		HDC hdc=m_loadImage->GetDC();
		CDC *pDC = CDC::FromHandle(hdc);
		for(k=0;k<m_njustMap.roads[index].pInLine.size();k++)
			pDC->SetPixel(m_njustMap.roads[index].pInLine[k],RGB(255,255,255));

		m_loadImage->ReleaseDC();
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
	}
}

//ˢ��
void CCreateMapDlg::OnBnClickedButtonf5()
{
	if(!isLoad())
		return;

	//����ͼ��
	CRect rect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());
	HDC hdc=m_loadImage->GetDC();
	m_backUpImage->Draw(hdc,rect,rect);		//����Ϊԭͼ
	m_loadImage->ReleaseDC();
	drawmap::DrawByRecord(m_loadImage,m_records,RGB(255,0,0));
	//ˢ��
	m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 
}

//ɾ����·
void CCreateMapDlg::OnBnClickedButtondeline()
{
	if(!isLoad())
		return;

	int index=m_listMap.GetCurSel();
	if(index!=LB_ERR){
		int ls=m_njustMap.roads.size();
		if(index<ls){						//ɾ����·
			//ɾ���滭�б��еı�ע
			int showid=m_njustMap.roads[index].road.idself-START_LINE_ID+1;
			for(unsigned int i=0;i<m_records.size();i++){
				if(m_records[i].id==showid&&m_records[i].type==4){
					m_records.erase(m_records.begin()+i); //ɾ����¼
					m_listRecord.DeleteString(i);         //ɾ����ʾ��Ŀ
					break;
				}
			}
			//��ͼ�ṹɾ��
			m_njustMap.roads.erase(m_njustMap.roads.begin()+index);
			//�б���ɾ��
			m_listMap.DeleteString(index);
			
			//�ػ�
			DlgReDraw();
		}else{								//ɾ��·��
			//��ͼ�ṹɾ�� ·��
			m_njustMap.crosses.erase(m_njustMap.crosses.begin()+index-ls);
			//�б���ɾ��
			m_listMap.DeleteString(index);
		}
		//ˢ��
		OnBnClickedButtonf5();

	}
}

//�����·
void CCreateMapDlg::OnBnClickedButtondelineout()
{
	if(!isLoad())
		return;

	if(!m_njustMap.CheckIsCali()){
		AfxMessageBox(L"��ͼδ�궨�����ȱ궨",MB_OK);
		return;
	}

	CString path=L"D:\\map";
	if(!m_njustMap.writeRoad(path)){
		AfxMessageBox(L"����ʧ��",MB_OK);
	}else{
		m_njustMap.writeRoadTxt(path);

		CString msg;
		msg.Format(L"������%sĿ¼��",path);
		AfxMessageBox(msg,MB_OK);
	}
}



///////////////////////ҵ���߼�����//////////////////////////


//************************************
// ������:   DlgDrawLine
// ������������ֱ�߻�ͼ�ľ������
// ����: 	 CPoint point ��ǰ������(ȫ����)
// ����: 	 CRect rect   rect:�ؼ�����
// ��������: void
// ���ڣ�	 2016/04/04
//************************************
void CCreateMapDlg::DlgDrawLine(CPoint point,CRect rect){
		// ---Step.1--- �жϵ�ǰ����ֱ�ߵ��׶˻���β��
		if(m_lineP.x==0&&m_lineP.y==0){	  
			// --- Step.2.1 --- ��¼��һ����
			point.x-=rect.left; point.y-=rect.top;
			point.x+=m_srcRect.left; point.y+=m_srcRect.top;
			m_lineP=point;
		}else{
			DRAW_RECORD dr;

			// --- Step.2.2 --- �������� ��ͼ�л���ֱ��
			HDC hdc=m_loadImage->GetDC();
			CDC *pDC = CDC::FromHandle(hdc);
			point.x-=rect.left; point.y-=rect.top;
			point.x+=m_srcRect.left; point.y+=m_srcRect.top;
			//drawmap::DrawLine(pDC,m_lineP,point,RGB(255,0,0));
			drawmap::DrawLineBresenham(pDC,m_lineP,point,RGB(255,0,0));
			m_loadImage->ReleaseDC();

			// --- Step.2.3 --- ��¼���������еĵ�
			//drawmap::getPointsByImage(m_canvas,nrect,dr.drawPoints);//��ȡ���Ƶĵ�
			drawmap::LogLineBresenham(m_lineP,point,dr.drawPoints);
			/*m_canvas->ReleaseDC();*/

			// --- Step.2.4 --- ��¼��"�滭������"��	
			dr.type=0;
			m_records.push_back(dr);
			m_listRecord.AddString(drawmap::PrintRecord(dr));

			// --- Step.2.5 --- ����,�������
			m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 
			
			m_lineP.SetPoint(0,0);
		}
		// --- Step.3 --- ���û���״̬
		m_nowCase=Case_None;
}


//************************************
// ������:   DlgDrawBezier
// �����������������ߵľ������
// ����: 	 CPoint piont ��ǰ������(ȫ����)
// ����: 	 CRect rect	  rect:�ؼ�����
// ��������: void
// ���ڣ�	 2016/04/04
//************************************
void CCreateMapDlg::DlgDrawBezier(CPoint point ,CRect rect){
	// --- Step.1 --- �ж��Ѿ����Ƶĵ���
	if(control_bezier.index<3){
		// --- Step.2 --- ת����ͼƬ���꣬����¼
		point.x-=rect.left; point.y-=rect.top;
		point.x+=m_srcRect.left; point.y+=m_srcRect.top;

		control_bezier.Points[control_bezier.index++]=point;
	}else if(control_bezier.index == 3){
		DRAW_RECORD dr;
		// --- Step.3.1 --- ��¼���һ��(4)������꣬������
		point.x-=rect.left; point.y-=rect.top;
		point.x+=m_srcRect.left; point.y+=m_srcRect.top;
		control_bezier.Points[control_bezier.index]=point;

		HDC hdc=m_loadImage->GetDC();
		CDC *pDC = CDC::FromHandle(hdc);
		drawmap::DrawBezier(pDC,control_bezier.Points,control_bezier.index+1,RGB(255,0,0));
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); //����picture control
		m_loadImage->ReleaseDC();

		// --- Step.3.2 --- ��¼�����ϵĵ�
		CRect nrect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());		//TODO: ��ͼ�˴����Ż�Ϊ�ֲ�����
		nrect.NormalizeRect();
		dr.drawPoints.reserve(nrect.Width()+nrect.Height());

		pDC=CDC::FromHandle(m_canvas->GetDC());
		drawmap::ResetImage(m_canvas,m_backUpImage,nrect); //���֮ǰ�ĺ�̨�����Ϻۼ� mask
		drawmap::DrawBezier(pDC,control_bezier.Points,control_bezier.index+1,RGB(255,0,0));
		drawmap::getPointsByImage(m_canvas,nrect,dr.drawPoints);//��ȡ���Ƶĵ�
		m_canvas->ReleaseDC();

		// --- Step.3.3 --- ��¼��"�滭������"��	
		dr.type=1;
		m_records.push_back(dr);
		m_listRecord.AddString(drawmap::PrintRecord(dr));


		// --- Step.3.4 --- ���û���״̬,����״̬
		m_nowCase=Case_None;
		control_bezier.index=0;
	}
}


//************************************
// ������:   DlgDrawPoints
// ��������������һϵ�е㼯 ��Ҫ���ڱ�ע����·����·��
// ����: 	 CPoint point ��ǰ������(ȫ����)
// ����: 	 CRect rect rect:�ؼ�����
// ��������: void
// ���ڣ�	 2016/04/06
//************************************
void CCreateMapDlg::DlgDrawPoints(CPoint point ,CRect rect){
	// --- Step.1 ���� ---
	point.x-=rect.left; point.y-=rect.top;
	point.x+=m_srcRect.left; point.y+=m_srcRect.top;

	HDC hdc=m_loadImage->GetDC();
	CDC *pDC = CDC::FromHandle(hdc);
	pDC->SetPixel(point,RGB(255,0,0));

	// --- Step.2 --- ��¼��
	control_points.points.push_back(point);

	// --- Step.3---  ����ͼʾ
	m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
}


//************************************
// ������:   DlgDrawMark
// ��������������·��
// ����: 	 CPoint point
// ����: 	 CRect rect
// ��������: void
// ���ڣ�	 2016/04/12
//************************************
void CCreateMapDlg::DlgDrawMark(CPoint point,CRect rect){
	// --- Step.1 --- �жϵڼ�����
	if(m_nodeP.x==0&&m_nodeP.y==0){
		// --- Step.2.1 --- ��¼��һ����
			point.x-=rect.left; point.y-=rect.top;
			point.x+=m_srcRect.left; point.y+=m_srcRect.top;
			m_nodeP=point;
	}else{
		// --- Step.2.2 --- ����
			DRAW_RECORD dr;


			// --- Step.2.3 --- ��¼����ͼ�ṹ��  TODO ��ֳɺ���
			CREATE_MAP_NODE tNode;
			if(m_njustMap.nodes.size()==0){
				
				tNode.position.SetPoint(m_nodeP.x,m_nodeP.y); //·������λ����������
				tNode.node.idself=START_NODE_ID;				//ID��START_NODE_ID��ʼ
				m_njustMap.nodes.push_back(tNode);           
			}else{
				tNode.position.SetPoint(m_nodeP.x,m_nodeP.y); //·������λ����������
				int lastID=m_njustMap.nodes.back().node.idself+1;	//ID����һ���ڵ�+1 �����ظ�
				tNode.node.idself=lastID;
				m_njustMap.nodes.push_back(tNode);
			}


			HDC hdc=m_loadImage->GetDC();
			CDC *pDC = CDC::FromHandle(hdc);
			point.x-=rect.left; point.y-=rect.top;
			point.x+=m_srcRect.left; point.y+=m_srcRect.top;
			double r=(point.x-m_nodeP.x)*(point.x-m_nodeP.x)+(point.y-m_nodeP.y)*(point.y-m_nodeP.y);
			r=sqrt(r);
			drawmap::DrawNodeMark(pDC,m_nodeP,int(r),RGB(255,0,0),tNode.node.idself-START_NODE_ID);
			m_loadImage->ReleaseDC();

			// --- Step.2.4 --- ��¼Բ�ĺͱ��ϵ�һ��
            dr.drawPoints.push_back(m_nodeP);
			dr.drawPoints.push_back(point);

			// --- Step.2.5 --- ��¼��"�滭������"��	
			dr.type=3;
			dr.id=tNode.node.idself-START_NODE_ID;
			m_records.push_back(dr);
			m_listRecord.AddString(drawmap::PrintRecord(dr)); 
			//m_listMap.AddString(drawmap::PrintRecord(dr));
			

			// --- Step.3 --- ���ƺ�����״̬
			m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 	
			m_nodeP.SetPoint(0,0);
			m_nowCase=Case_None;
	}
	
}





//************************************
// ������:   OnMapSetline2id
// ������������Ӧ MAP_SETLINE_2ID ��Ϣ,��ɵ�·������Ϣ
// ����: 	 WPARAM wParam
// ����: 	 LPARAM lParam
// ��������: LRESULT
// ���ڣ�	 2016/04/15
//************************************
afx_msg LRESULT CCreateMapDlg::OnMapSetline2id(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox(L"��ȡ��Ϣ",MB_OK);
	unsigned int i,k;
	int nItemCount = m_listRecord.GetSelCount();
	if (0 != nItemCount){
		int* indexBuf = new int[nItemCount];
		memset(indexBuf,0,nItemCount*sizeof(int));
		m_listRecord.GetSelItems(nItemCount,indexBuf);
		vector<DRAW_RECORD> tempRecord;			//��ʱ��¼ѡ����Ŀ
		for (i = 0 ;i<nItemCount;i++)
		{
			tempRecord.push_back(m_records[indexBuf[i]]);
		}
		delete[]indexBuf;
		//�ϲ����ڵ�
		CPoint *p=(CPoint*)lParam;
		if(m_njustMap.merge2Line(tempRecord,*p)){
			//�滭�����б���Ӷ���
			DRAW_RECORD dr;
			// --- Step.2.2 --- �������� ��ͼ�л���ֱ��
			HDC hdc=m_loadImage->GetDC();
			CDC *pDC = CDC::FromHandle(hdc);
			m_loadImage->ReleaseDC();
			//��д��¼
			dr.type=4;
			dr.id=m_njustMap.roads.back().road.idself-START_LINE_ID+1;
			dr.drawPoints=m_njustMap.roads.back().pInLine;
			//����ͼ��
			//drawmap::DrawByRecord(m_loadImage,m_records,RGB(255,0,0));
			drawmap::DrawRoadMark(pDC,dr.drawPoints,dr.id);
		
			m_records.push_back(dr);
			//�����¼�͵�·�б�ͬ�� �м�
			m_listRecord.AddString(drawmap::PrintRecord(dr));  

			// --- Step.2.5 --- ����,����
			DlgReDraw();
			m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 



			//�����·�б�
			m_listMap.ResetContent();
			for(k=0;k<m_njustMap.roads.size();k++)
				m_listMap.AddString(m_njustMap.printRoad(k));
			for(k=0;k<m_njustMap.crosses.size();k++)
				m_listMap.AddString(m_njustMap.printCross(k));
		}else{
			AfxMessageBox(L"�ϲ����󣬼���Ƿ���ӷǷ�����",MB_OK);
		}
		delete[] p; //�����ͷ��ڴ�
	}else{
		AfxMessageBox(L"δѡȡ��ͼ����",MB_OK);
	}
	
	return 0;
}




//����ϲ�·����Ϣ
//************************************
// ������:   OnMapSetcross2id
// ������������Ӧ MAP_SETCROSS_2ID ��Ϣ,���·�ڹ�����Ϣ
// ����: 	 WPARAM wParam
// ����: 	 LPARAM lParam
// ��������: LRESULT
// ���ڣ�	 2016/04/27
//************************************
afx_msg LRESULT CCreateMapDlg::OnMapSetcross2id(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox(L"��ȡ��Ϣ",MB_OK);
	unsigned int i,k;
	int nItemCount = m_listRecord.GetSelCount();
	if (0 != nItemCount){
		int* indexBuf = new int[nItemCount];
		memset(indexBuf,0,nItemCount*sizeof(int));
		m_listRecord.GetSelItems(nItemCount,indexBuf);
		vector<DRAW_RECORD> tempRecord;			//��ʱ��¼ѡ����Ŀ
		for (i = 0 ;i<nItemCount;i++)
		{
			tempRecord.push_back(m_records[indexBuf[i]]);
		}
		delete[]indexBuf;
		//�ϲ����ڵ�
		CPoint *p=(CPoint*)lParam;
		if(m_njustMap.merge2Cross(tempRecord,*p)){

		//�����·�б�
		m_listMap.ResetContent();
		for(k=0;k<m_njustMap.roads.size();k++)
			m_listMap.AddString(m_njustMap.printRoad(k));
		for(k=0;k<m_njustMap.crosses.size();k++)
			m_listMap.AddString(m_njustMap.printCross(k));

		}else{
			AfxMessageBox(L"�ϲ����󣬼���Ƿ���ӷǷ�����",MB_OK);
		}
		delete[] p; 
	}else{
		AfxMessageBox(L"δѡȡ��ͼ����",MB_OK);
	}
	
	return 0;
}


//************************************
// ������:   serial
// �������������л�����
// ��������: void
// ���ڣ�	 2016/04/29
//************************************
//
//
//
//
//
//************************************
void CCreateMapDlg::serial(CFile &file){
	unsigned int i,j;
	unsigned int len= m_records.size();
	MAP_CPOINT tp(0,0);
	file.Write(&len,sizeof(unsigned int));
	for(i=0;i<m_records.size();i++){
		len=m_records[i].drawPoints.size();
		file.Write(&len,sizeof(unsigned int));
		for (j=0;j<len;j++){
			 tp.x=m_records[i].drawPoints[j].x;
			 tp.y=m_records[i].drawPoints[j].y;
			 file.Write(&tp,sizeof(MAP_CPOINT));
		}
		file.Write(&(m_records[i].id),sizeof(int));
		file.Write(&(m_records[i].type),sizeof(int));
	}
}

//�����л�
void CCreateMapDlg::enserial(CFile &file){
	m_records.clear();

	unsigned int i,j;
	unsigned int len1,len2;
	MAP_CPOINT tp(0,0);
	file.Read(&len1,sizeof(unsigned int));
	for(i=0;i<len1;i++){
		DRAW_RECORD trecord;
		file.Read(&len2,sizeof(unsigned int));
		for (j=0;j<len2;j++){
			 file.Read(&tp,sizeof(MAP_CPOINT));
			 trecord.drawPoints.push_back(CPoint(tp.x,tp.y));
		}
		file.Read(&(trecord.id),sizeof(int));
		file.Read(&(trecord.type),sizeof(int));
		m_records.push_back(trecord);
	}
}

bool CCreateMapDlg::isLoad(){
	if(m_loadImage==NULL){
		AfxMessageBox(L"�����½��������ͼ",MB_OK);
		return false;
	}
	return true;
}

////////////////////////////////////�ؼ���ʼ��//////////////////////////////////////



//��̬�����ؼ�λ�� 
void CCreateMapDlg::initCtlPosition(){
	CRect rect;
	GetWindowRect(&rect);
	m_listRect.AddTail(rect);//�Ի��������
	CWnd *pWnd = GetWindow(GW_CHILD);//��ȡ�Ӵ���
	while (pWnd)
	{
		pWnd->GetWindowRect(rect);//�Ӵ��������
		m_listRect.AddTail(rect);           //CList<CRect,CRect> m_listRect��Ա����
		pWnd = pWnd->GetNextWindow();//ȡ��һ���Ӵ���
	}
}

void CCreateMapDlg::initStatusBar(){
	m_statusBar=new CStatusBarCtrl;
	CRect m_Rect; 
	GetClientRect(&m_Rect); //��ȡ�Ի���ľ�������
	m_Rect.top=m_Rect.bottom-20; //����״̬���ľ�������
	m_statusBar->Create(WS_BORDER|WS_VISIBLE|CBRS_BOTTOM,m_Rect,this,3);
	int nParts[2]= {m_Rect.Width()/2,-1}; //�ָ�ߴ�
	m_statusBar->SetParts(2, nParts); //�ָ�״̬��
	m_statusBar->SetText(L"��ǰ��ͼ:��δ�򿪵�ͼ��",0,0); //��һ����������"���ǵ�һ��ָʾ��"
	
	
}





////////////////////////////GPS���ж�̬��ʾ��ͼ��//////////////////////////////////////////////



void CCreateMapDlg::OnLbnSelchangeListrecord()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CCreateMapDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
	
}

void CCreateMapDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(m_Show_cur!=m_Show_GPSList.size()){
		// ����
		COMPUTE_GPS gps;
		gps.lng=m_Show_GPSList[m_Show_cur].x/60.0; //�ļ����Ƿ� 
		gps.lat=m_Show_GPSList[m_Show_cur].y/60.0;
		m_Show_cur++;
		m_njustMap.GPS2pexel(gps);

		//Draw
		HDC hdc=m_loadImage->GetDC();
		CDC *pDC = CDC::FromHandle(hdc);
		pDC->SelectStockObject(WHITE_BRUSH);

		CPoint p=CPoint(gps.x,gps.y);int r=4;
		pDC->MoveTo(p);
		pDC->Ellipse(p.x-r,p.y-r,p.x+r,p.y+r);
		m_loadImage->ReleaseDC();
		//����
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);

	}else{
		CWnd::KillTimer(1);
		m_Show_cur=0;
		if(m_Show_GPSList.size()!=0)
		AfxMessageBox(L"�������",MB_OK);
	}
	CDialog::OnTimer(nIDEvent);
}



////////////////////////////���˵�������//////////////////////////////////////////////

//�½���ͼ
void CCreateMapDlg::OnMenuBuildMap()
{
	//�Ѿ�������
	if(m_loadImage!=NULL){
		INT_PTR re=AfxMessageBox(L"�Ƿ������ǰ�����ռ�,�½���ͼ?",MB_OKCANCEL);
		if(re!=IDOK)
			return;
	}

	CString FilePathName;
    CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"),
        NULL);
    if(dlg.DoModal()==IDOK)
    {
        FilePathName=dlg.GetPathName(); //�ļ���
		m_loadImage=new CImage();
		m_loadImage->Load(FilePathName); //����ͼƬ·������ͼƬ  

		m_backUpImage=new CImage();
		m_backUpImage->Load(FilePathName); //����ͼƬ·������ͼƬ  


		m_canvas=new CImage();
		m_canvas->Create(m_loadImage->GetWidth(),m_loadImage->GetHeight(),24);



		CWnd *pWnd=GetDlgItem(IDC_PIC_MAIN);//���pictrue�ؼ����ڵľ��      
    

		//��ʼ����ͼ�ṹ
		COMPUTE_GPS gps2[2];
		/*gps2[0]=COMPUTE_GPS(484,311,118.8559087276,32.0303230959);
		gps2[1]=COMPUTE_GPS(m_loadImage->GetWidth()-1
							,m_loadImage->GetHeight()-1
							,118.8583817276
							,32.0258740959);*/
		//gps2[0]=COMPUTE_GPS(484,311,118.8558887276,32.0302730959); //5- 2 ;5-5
		//gps2[1]=COMPUTE_GPS(m_loadImage->GetWidth()-1
		//					,m_loadImage->GetHeight()-1
		//					,118.8583617276
		//					,32.0258240959);
		m_njustMap.init();  //���³�ʼ�� reset
		m_records.clear();    //ɾ�����е�ͼ

		//����б�
		m_listMap.ResetContent();
		m_listRecord.ResetContent();
		//�޸�״̬��
		CString strbar;
		strbar.Format(L"��ǰ��ͼ:��%s��",L"*δ����");
		m_statusBar->SetText(strbar,0,0);

		m_srcRect=m_picRect;
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
	}
}

//�򿪵�ͼ
void CCreateMapDlg::OnOpenMap()
{
	//��������ĵ�ͼ�ػ�
	 // --- Step.1 ---ѡ�񱣴�λ��	
	 CString fileName;
	 CFileDialog dlg(TRUE, 
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("�������ļ� (*.map)|*.map|All Files (*.*)|*.*||"),
        NULL);
	if(dlg.DoModal()==IDOK){
		fileName=dlg.GetPathName(); //�ļ���
		CFile file;
		// --- Step.2 --- ����
		if(file.Open(fileName,CFile::modeRead)){
			//��ȡm_njustmap
			m_njustMap.enserial(file);
			//����m_record
			enserial(file);
			file.Close();
		}
		int nPos= fileName.ReverseFind('.');
		CString bmpFileName = fileName.Left(nPos); //yyy
	    bmpFileName+=L".bmp";
		//m_loadImage->
		if(m_loadImage!=NULL) delete m_loadImage;
		if(m_backUpImage!=NULL) delete m_backUpImage;
		if(m_canvas!=NULL) delete m_canvas;
		m_loadImage=new CImage();
		m_loadImage->Load(bmpFileName); //����ͼƬ·������ͼƬ  
		m_backUpImage=new CImage();
		m_backUpImage->Load(bmpFileName); //����ͼƬ·������ͼƬ  
		m_canvas=new CImage();
		m_canvas->Create(m_loadImage->GetWidth(),m_loadImage->GetHeight(),24);
		//�����Ӵ�
		m_srcRect=m_picRect;

		//�ػ�ͼ��
		DlgReDraw();
		//����list
		m_listRecord.ResetContent();
		unsigned int k;
		for(k=0;k<m_records.size();k++)
			m_listRecord.AddString(drawmap::PrintRecord(m_records[k]));
		m_listMap.ResetContent();
		for(k=0;k<m_njustMap.roads.size();k++)
				m_listMap.AddString(m_njustMap.printRoad(k));
		for(k=0;k<m_njustMap.crosses.size();k++)
				m_listMap.AddString(m_njustMap.printCross(k));

		//�޸�״̬��
		int nPos1=fileName.ReverseFind('\\');
		CString strbar=fileName.Right(fileName.GetLength()-nPos1-1);
		strbar=strbar.Left(strbar.GetLength()-4); //.map(4)
		m_statusBar->SetText(L"��ǰ��ͼ:��"+strbar+L"��",0,0);
	}
}

//�����ͼ
void CCreateMapDlg::OnSaveMap()
{
	if(!isLoad())
		return;
	 // --- Step.1 ---ѡ�񱣴�λ��	
	 CString fileName;
	 CFileDialog dlg(FALSE, 
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("�����ͼ�� |All Files (*.*)|*.*||"),
        NULL);
	if(dlg.DoModal()==IDOK){
		fileName=dlg.GetPathName(); //�ļ��� C:\\xxx\\yyy
		CFile file;
		// --- Step.2 --- ����m_njustmap(.map)
		if(file.Open(fileName+L".map",CFile::modeCreate|CFile::modeWrite)){
			//����m_njustmap
			m_njustMap.serial(file);
			//����滭��¼
			serial(file);
			file.Close();
		}
		// --- Step.3 --- ����ͼƬ
		m_backUpImage->Save(fileName+L".bmp");

		//�޸�״̬��
		int nPos1=fileName.ReverseFind('\\');
		CString strbar=fileName.Right(fileName.GetLength()-nPos1-1);
		m_statusBar->SetText(L"��ǰ��ͼ:��"+strbar+L"��",0,0);
		AfxMessageBox(L"����ɹ�",MB_OK);	    
	}
}

//��ȡ��ʾGPS����
void CCreateMapDlg::OnMenuShowGPS()
{
	if(!isLoad())
		return;
	if(!m_njustMap.CheckIsCali()){
		AfxMessageBox(L"��ͼδ�궨�����ȱ궨",MB_OK);
		return;
	}

	CString FilePathName;
    CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("DB Files (*.db)|*.db|All Files (*.*)|*.*||"),
        NULL);
	if(dlg.DoModal()==IDOK){
		FilePathName=dlg.GetPathName(); //�ļ���
		m_Show_GPSList.clear();

		CFile pf;
		MAP_DOUBLE_POINT tPoint;  //��db
		if(pf.Open(FilePathName,CFile::modeRead|CFile::modeNoTruncate)){
			int GPSnum = pf.GetLength() / sizeof(MAP_DOUBLE_POINT);
			for(int i=0;i<GPSnum;i++){
					pf.Read(&tPoint,sizeof(MAP_DOUBLE_POINT)); //fread(&tPoint, sizeof(MAP_DOUBLE_POINT), 1, pf);
					m_Show_GPSList.push_back(tPoint);  //��¼
			}
		}else{
			AfxMessageBox(L"��ȡDB�ļ�ʧ��",MB_OK);
		}
	}
	CWnd::SetTimer(1,100,NULL);
}

/////////////////////////////�����Ĳ˵�������/////////////////////////////////////////////

void CCreateMapDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
   CRect rectDraw,rectMap;
   m_listRecord.GetWindowRect(&rectDraw);
   m_listMap.GetWindowRect(&rectMap);
   CPoint p;
   GetCursorPos(&p);
   //�滭��¼���
   if (rectDraw.PtInRect(p)||rectMap.PtInRect(p))
    {
       CMenu m_Menu;
       m_Menu.LoadMenu(IDR_MENUPOP);//�༭�õĲ˵���Դ
	   CMenu *m_SubMenu=NULL;
	   if(rectDraw.PtInRect(p))
		  m_SubMenu= m_Menu.GetSubMenu(0);
	   else
		  m_SubMenu= m_Menu.GetSubMenu(1);
       m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);
   }

}





/////////////////////////////�궨��ز���/////////////////////////////////////////////
void CCreateMapDlg::showNowGPS(char *buff,long len){
	double longlat[2];
	m_getMCInfo.getGPSAndPostion(buff,len,longlat);
	///Test 118.85644481,32.02762140
	longlat[0]=118.85690144;
	longlat[1]=32.02764064;
	///

	CString strShow;
	strShow.Format(L"����(��):%.8lf γ��(��):%.8lf",longlat[0],longlat[1]);

	m_RealGPS.x=longlat[0];
	m_RealGPS.y=longlat[1];

	m_statusBar->SetText(strShow,1,0); 
}


//��ȡ��һ����
void CCreateMapDlg::OnMenuP1()
{
	 m_nowCase=Case_getP1;
	
}

//��ȡ�ڶ�����
void CCreateMapDlg::OnMenuP2()
{
	m_nowCase=Case_getP2;
}

//�������
void CCreateMapDlg::OnMenuDeviation()
{
	m_nowCase=Case_Deviation;
}

//���ñ궨���� �����λ��(�ͻ���) ͼƬ�ؼ���С(rect) �궨������
void CCreateMapDlg::setCalibration(CPoint point,CRect rect,int index){
		if(!isLoad())
			return;

		if(abs(m_RealGPS.x-.0l)<0.001)
			AfxMessageBox(L"δ�ܻ�ȡGPS��Ϣ",MB_OK);
		else{
			point.x-=rect.left; point.y-=rect.top;
			point.x+=m_srcRect.left; point.y+=m_srcRect.top;
			m_njustMap.buildGPS[index].x=point.x;
			m_njustMap.buildGPS[index].y=point.y;
			m_njustMap.buildGPS[index].lng=m_RealGPS.x;
			m_njustMap.buildGPS[index].lat=m_RealGPS.y;
			CString strShow;
			strShow.Format(L"P%d �������:px(%d,%d) GPS(%.8lf ,%.8lf)",
				index+1,point.x,point.y,m_RealGPS.x,m_RealGPS.y);
			AfxMessageBox(strShow,MB_OK);
		}
}

//�������
void CCreateMapDlg::coumputerDevication(CPoint point,CRect rect){
	if(!isLoad())
		return;

	point.x-=rect.left; point.y-=rect.top;
	point.x+=m_srcRect.left; point.y+=m_srcRect.top;
	if(m_njustMap.CheckIsCali()){
		m_njustMap.computeScale();//����߶�
		COMPUTE_GPS cGPS;
		cGPS.x=point.x;cGPS.y=point.y;
		m_njustMap.pixel2GPS(cGPS);

		CString strShow;
		//�������
		int cx,cy;
		int rx,ry;
		m_getMCInfo.blh2xy(cGPS.lat,cGPS.lng,cx,cy);
		m_getMCInfo.blh2xy(m_RealGPS.y,m_RealGPS.x,rx,ry);
		double delta=(cx-rx)*(cx-rx)+(cy-ry)*(cy-ry);
		delta=sqrt(delta);
		strShow.Format(L"����GPS:(%.8lf,%.8lf) ����Ϊ%.5lfcm",cGPS.lng,cGPS.lat,delta);
		AfxMessageBox(strShow,MB_OK);
	}else{
		AfxMessageBox(L"��ͼδ�궨�����ȱ궨",MB_OK);
	}
}



void CCreateMapDlg::OnResolution()
{
	if(m_njustMap.CheckIsCali()){
		COMPUTE_GPS cGPS,cGPS1;  //������������
		cGPS.x=0;cGPS.y=0;
		cGPS1.x=1;cGPS1.y=0;

		m_njustMap.pixel2GPS(cGPS);  //���ݱ궨��������GPS
		m_njustMap.pixel2GPS(cGPS1);

		int x,y;
		int x1,y1; 
		m_getMCInfo.blh2xy(cGPS.lat,cGPS.lng,x,y);   //�����cm
		m_getMCInfo.blh2xy(cGPS1.lat,cGPS1.lng,x1,y1);
		double delta1=(x1-x)*(x1-x)+(y1-y)*(y1-y);
		delta1=sqrt(delta1);

		//ͬ���������Y�����ϵķֱ���
		cGPS1.x=0;cGPS1.y=1;
		m_getMCInfo.blh2xy(cGPS1.lat,cGPS1.lng,x1,y1);
		double delta2=(x1-x)*(x1-x)+(y1-y)*(y1-y);
		delta2=sqrt(delta2);

		CString strShow;
		strShow.Format(L"x,y�����Ϸֱ���Ϊ %.5lfcm/pix %.5lfcm/pix",delta1,delta2);
		AfxMessageBox(strShow,MB_OK);
	}
}
