
// CreateMapDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CreateMap.h"
#include "CreateMapDlg.h"
#include "afxdialogex.h"

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
	ON_BN_CLICKED(IDC_BUTTON1, &CCreateMapDlg::OnBnClickedButton1)
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
ON_BN_CLICKED(IDC_BUTTONSHOW, &CCreateMapDlg::OnBnClickedButtonshow)
ON_BN_CLICKED(IDC_BUTNMARKNODE, &CCreateMapDlg::OnBnClickedButnmarknode)
ON_BN_CLICKED(IDC_BUTTONMEG, &CCreateMapDlg::OnBnClickedButtonmeg)
ON_MESSAGE(MAP_SETLINE_2ID, &CCreateMapDlg::OnMapSetline2id)
ON_LBN_SELCHANGE(IDC_LISTRECORD, &CCreateMapDlg::OnLbnSelchangeListrecord)
ON_BN_CLICKED(IDC_BUTTONSHOWROAD, &CCreateMapDlg::OnBnClickedButtonshowroad)
ON_BN_CLICKED(IDC_BUTTONF5, &CCreateMapDlg::OnBnClickedButtonf5)
ON_BN_CLICKED(IDC_BUTTONDELINE, &CCreateMapDlg::OnBnClickedButtondeline)
ON_BN_CLICKED(IDC_BUTTONDELINEOUT, &CCreateMapDlg::OnBnClickedButtondelineout)
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_isMove=false;
	m_loadImage=NULL;
	m_canvas=NULL;
	m_canvas1=NULL;
	m_backUpImage=NULL;
	m_isDrawLine=false;
	CWnd *pWnd=GetDlgItem(IDC_PIC_MAIN);//���pictrue�ؼ����ڵľ��   
	pWnd->GetClientRect(&m_picRect);	//���pictrue�ؼ����ڵľ�������   
	m_pPicDC=pWnd->GetDC();
	m_nowCase=Case_None;
	control_bezier.index=0;
	m_lineDlg=NULL;
	

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



//�� ͼƬ�ļ�
void CCreateMapDlg::OnBnClickedButton1()
{
	//// TODO: �ڴ���ӿؼ�֪ͨ��������
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

		m_canvas1=new CImage();
		m_canvas1->Create(m_loadImage->GetWidth(),m_loadImage->GetHeight(),24);


		CWnd *pWnd=GetDlgItem(IDC_PIC_MAIN);//���pictrue�ؼ����ڵľ��      
     
		m_srcRect=m_picRect;
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); //��ͼƬ����Picture�ؼ���ʾ�ľ�������  

		//��ʼ����ͼ�ṹ
		COMPUTE_GPS gps2[2];
		gps2[0]=COMPUTE_GPS(0,0,118.8537507276,32.0315140959);
		gps2[1]=COMPUTE_GPS(m_loadImage->GetWidth()-1
							,m_loadImage->GetHeight()-1
							,118.8583817276
							,32.0258740959);

		m_njustMap.init(gps2);
	}
}





//����������
void CCreateMapDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// -- Step 0 -- ��ʼ��
	CRect rect;
	GetDlgItem(IDC_PIC_MAIN)->GetWindowRect(&rect);//��ȡ�ؼ�����ȫ�����λ��
	ScreenToClient(rect);//ת��Ϊ�Ի����ϵ����λ��

	// -- Step 1 -- �ж���궯���Ƿ��ڻ�ͼ������
	
	if(rect.PtInRect(point)){
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	m_nowCase=Case_Line;			//�ı��ͼ״̬
}

//����������test
void CCreateMapDlg::OnBnClickedButnbezier()
{
	m_nowCase=Case_BEZIER;
}


//mark״̬
void CCreateMapDlg::OnBnClickedButnmarknode()
{
	m_nowCase=Case_MarkNode;
}


//�����㼯��
void CCreateMapDlg::OnBnClickedButnpoints()
{
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

//ɾ��ѡ�е���Ŀ
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
            m_listRecord.DeleteString(indexBuf[i]);           //ɾ����ʾ��Ŀ
			if(m_records[i].type==3){						//ɾ��·�� ��ͼ�ṹ
				m_njustMap.deleteEleByID(true,m_records[indexBuf[i]].id+START_NODE_ID);
			}
			m_records.erase(m_records.begin()+indexBuf[i]);	  //ɾ��������¼��
        }
        delete[]indexBuf;

		// --- Step.2 --- ���ݶ�����¼���ػ�
		CRect rect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());
		HDC hdc=m_loadImage->GetDC();
		m_backUpImage->Draw(hdc,rect,rect);		//����Ϊԭͼ
		m_loadImage->ReleaseDC();

		//����ͼ��
		drawmap::DrawByRecord(m_loadImage,m_records,RGB(255,0,0));
		//ˢ��
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 
    }
}
	
//���� ѡ����Ŀ
void CCreateMapDlg::OnBnClickedButtonshow(){
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


//�ϲ���ť
void CCreateMapDlg::OnBnClickedButtonmeg()
{
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


//��ʾ��·
void CCreateMapDlg::OnBnClickedButtonshowroad()
{
	int i,k;
	int index=m_listMap.GetCurSel();
	if(index!=LB_ERR){
		HDC hdc=m_loadImage->GetDC();
		CDC *pDC = CDC::FromHandle(hdc);
		for(k=0;k<m_njustMap.roads[index].pInLine.size();k++)
			pDC->SetPixel(m_njustMap.roads[index].pInLine[k],RGB(255,255,255));

		// --- Step.3---  ����ͼʾ
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
	}
}

//ˢ��
void CCreateMapDlg::OnBnClickedButtonf5()
{
	//����ͼ��
	CRect rect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());
	HDC hdc=m_loadImage->GetDC();
	m_backUpImage->Draw(hdc,rect,rect);		//����Ϊԭͼ
	m_loadImage->ReleaseDC();
	drawmap::DrawByRecord(m_loadImage,m_records,RGB(255,0,0));
	//ˢ��
	m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 
}

//ɾ��
void CCreateMapDlg::OnBnClickedButtondeline()
{
	int index=m_listMap.GetCurSel();
	if(index!=LB_ERR){
		//��ͼ�ṹɾ��
		m_njustMap.roads.erase(m_njustMap.roads.begin()+index);
		//�б���ɾ��
		m_listMap.DeleteString(index);
		//ˢ��
		OnBnClickedButtonf5();

	}
}

//�����·
void CCreateMapDlg::OnBnClickedButtondelineout()
{
	//m_njustMap.roads
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
			//CRect nrect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());			//TODO: ��ͼ�˴����Ż�Ϊ�ֲ�����
			//nrect.NormalizeRect();
			//dr.drawPoints.reserve(rect.Width()+rect.Height());

			//pDC=CDC::FromHandle(m_canvas->GetDC());
			//drawmap::ResetImage(m_canvas,m_canvas1,nrect); //���֮ǰ�ĺ�̨�����Ϻۼ� mask
			//drawmap::DrawLine(pDC,m_lineP,point,RGB(255,0,0));		  //�ں�̨�������ٴλ���

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
		drawmap::ResetImage(m_canvas,m_canvas1,nrect); //���֮ǰ�ĺ�̨�����Ϻۼ� mask
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
// ������������Ӧ MAP_SETLINE_2ID ��Ϣ,��ɵ��ڹ�����Ϣ
// ����: 	 WPARAM wParam
// ����: 	 LPARAM lParam
// ��������: LRESULT
// ���ڣ�	 2016/04/15
//************************************
afx_msg LRESULT CCreateMapDlg::OnMapSetline2id(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox(L"��ȡ��Ϣ",MB_OK);
	int i,k;
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
			//�����·�б�
			m_listMap.ResetContent();
			for(k=0;k<m_njustMap.roads.size();k++)
				m_listMap.AddString(m_njustMap.printRoad(k));

		}else{
			AfxMessageBox(L"�ϲ����󣬼���Ƿ���ӷǷ�����",MB_OK);
		}
		delete[] p; //�����ͷ��ڴ�
	}
	
	return 0;
}

//???
void CCreateMapDlg::OnLbnSelchangeListrecord()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}








