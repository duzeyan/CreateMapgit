
// CreateMapDlg.cpp : 实现文件
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


// CCreateMapDlg 对话框



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

//手动注册事件响应
//BEGIN_MESSAGE_MAP(CCreateMapDlg,CDialog)
//	ON_BN_CLICKED(IDC_LISTRECORD, &CCreateMapDlg::OnBnClickedListBox)
//END_MESSAGE_MAP()

// CCreateMapDlg 消息处理程序

BOOL CCreateMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_isMove=false;
	m_loadImage=NULL;
	m_canvas=NULL;
	m_canvas1=NULL;
	m_backUpImage=NULL;
	m_isDrawLine=false;
	CWnd *pWnd=GetDlgItem(IDC_PIC_MAIN);//获得pictrue控件窗口的句柄   
	pWnd->GetClientRect(&m_picRect);	//获得pictrue控件所在的矩形区域   
	m_pPicDC=pWnd->GetDC();
	m_nowCase=Case_None;
	control_bezier.index=0;
	m_lineDlg=NULL;
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCreateMapDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCreateMapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//打开 图片文件
void CCreateMapDlg::OnBnClickedButton1()
{
	//// TODO: 在此添加控件通知处理程序代
	CString FilePathName;
    CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"),
        NULL);
    if(dlg.DoModal()==IDOK)
    {
        FilePathName=dlg.GetPathName(); //文件名
		m_loadImage=new CImage();
		m_loadImage->Load(FilePathName); //根据图片路径加载图片  

		m_backUpImage=new CImage();
		m_backUpImage->Load(FilePathName); //根据图片路径加载图片  


		m_canvas=new CImage();
		m_canvas->Create(m_loadImage->GetWidth(),m_loadImage->GetHeight(),24);

		m_canvas1=new CImage();
		m_canvas1->Create(m_loadImage->GetWidth(),m_loadImage->GetHeight(),24);


		CWnd *pWnd=GetDlgItem(IDC_PIC_MAIN);//获得pictrue控件窗口的句柄      
     
		m_srcRect=m_picRect;
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); //将图片画到Picture控件表示的矩形区域  

		//初始化地图结构
		COMPUTE_GPS gps2[2];
		gps2[0]=COMPUTE_GPS(0,0,118.8537507276,32.0315140959);
		gps2[1]=COMPUTE_GPS(m_loadImage->GetWidth()-1
							,m_loadImage->GetHeight()-1
							,118.8583817276
							,32.0258740959);

		m_njustMap.init(gps2);
	}
}





//鼠标左键落下
void CCreateMapDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// -- Step 0 -- 初始化
	CRect rect;
	GetDlgItem(IDC_PIC_MAIN)->GetWindowRect(&rect);//获取控件基于全窗体的位置
	ScreenToClient(rect);//转换为对话框上的相对位置

	// -- Step 1 -- 判断鼠标动作是否在绘图区域内
	
	if(rect.PtInRect(point)){
		switch(m_nowCase){
			// -- Step 1.1 -- 绘制直线
			case Case_Line:   {
								DlgDrawLine(point,rect);
								break;		 
							   }
			// -- Step 1.2 -- 绘制曲线
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
			//默认移动
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

//检测到鼠标移动
void CCreateMapDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//左键点击 且 已经加载过图片
	if(m_isMove&&m_loadImage!=NULL){
		 SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_HAND));
		 //计算鼠标位移
		int dx=point.x-m_startPoint.x;
		int dy=point.y-m_startPoint.y;

		//更新鼠标移动起始点
		m_startPoint.x=point.x;
		m_startPoint.y=point.y;

		int imageW=m_loadImage->GetWidth();
		int imageH=m_loadImage->GetHeight();

		//定义图像位置块矩形类 
		CWnd *pWnd=GetDlgItem(IDC_PIC_MAIN);//获得pictrue控件窗口的句柄     
		//CDC *pDC=pWnd->GetDC();//获得pictrue控件的DC     

		//根据鼠标位移滑动视窗	
		m_srcRect.left-=dx;m_srcRect.right-=dx;
		m_srcRect.top-=dy;m_srcRect.bottom-=dy;

		////检查是否越界
		drawmap::CheckViewInImage(m_srcRect,imageW,imageH,m_picRect.Width(),m_picRect.Height());

		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); //将图片画到Picture控件表示的矩形区域  

	}
	CDialog::OnMouseMove(nFlags, point);
}

//鼠标左键抬起
void CCreateMapDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//结束移动跟踪
	m_isMove=false;
	//鼠标手势改回箭头
	SetClassLong(this->GetSafeHwnd(),
                             GCL_HCURSOR , 
                             (LONG)LoadCursor(NULL , IDC_ARROW));
	CDialog::OnLButtonUp(nFlags, point);
}

//绘直线
void CCreateMapDlg::OnBnClickedButton2()
{
	m_nowCase=Case_Line;			//改变绘图状态
}

//贝塞尔曲线test
void CCreateMapDlg::OnBnClickedButnbezier()
{
	m_nowCase=Case_BEZIER;
}


//mark状态
void CCreateMapDlg::OnBnClickedButnmarknode()
{
	m_nowCase=Case_MarkNode;
}


//孤立点集合
void CCreateMapDlg::OnBnClickedButnpoints()
{
	if(Case_None==m_nowCase) // 第一次点
		m_nowCase=Case_Points;
	else if(Case_Points==m_nowCase){
		//记录到 绘画动作表中
		DRAW_RECORD dr;
		dr.type=2;
		dr.drawPoints=control_points.points;
		m_records.push_back(dr);
		m_listRecord.AddString(drawmap::PrintRecord(dr));

		control_points.points.clear();
		m_nowCase=Case_None;
	}
		
}

//删除选中的条目
void CCreateMapDlg::OnBnClickedButtondel()
{
	int i;
	int nItemCount = m_listRecord.GetSelCount();
	
    if (0 != nItemCount){
		// --- Step.1 --- 删除选中的条目和对应的动作记录项
        int* indexBuf = new int[nItemCount];
        memset(indexBuf,0,nItemCount*sizeof(int));
        m_listRecord.GetSelItems(nItemCount,indexBuf);
        for (i = nItemCount-1 ;i >-1;i--)
        {
            m_listRecord.DeleteString(indexBuf[i]);           //删除显示条目
			if(m_records[i].type==3){						//删除路口 地图结构
				m_njustMap.deleteEleByID(true,m_records[indexBuf[i]].id+START_NODE_ID);
			}
			m_records.erase(m_records.begin()+indexBuf[i]);	  //删除动作记录表
        }
        delete[]indexBuf;

		// --- Step.2 --- 根据动作记录表重绘
		CRect rect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());
		HDC hdc=m_loadImage->GetDC();
		m_backUpImage->Draw(hdc,rect,rect);		//重置为原图
		m_loadImage->ReleaseDC();

		//绘制图形
		drawmap::DrawByRecord(m_loadImage,m_records,RGB(255,0,0));
		//刷新
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 
    }
}
	
//高亮 选中条目
void CCreateMapDlg::OnBnClickedButtonshow(){
		CString strTitle;
		GetDlgItem(IDC_BUTTONSHOW)->GetWindowTextW(strTitle);
		if(strTitle==L"显示"){
			// --- Step.1 --- 绘制选中的条目为蓝色
			int i;
			int nItemCount = m_listRecord.GetSelCount();
			if (0 != nItemCount){
				int* indexBuf = new int[nItemCount];
				memset(indexBuf,0,nItemCount*sizeof(int));
				m_listRecord.GetSelItems(nItemCount,indexBuf);
				vector<DRAW_RECORD> tempRecord;			//临时记录选中条目
				for (i = nItemCount-1 ;i >-1;i--)
				{
					tempRecord.push_back(m_records[indexBuf[i]]);
				}
				delete[]indexBuf;
	
				// --- Step.2 --- 根据选中列表绘制高亮
				drawmap::DrawByRecord(m_loadImage,tempRecord,RGB(0,255,255));
				//刷新
				m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
	
				GetDlgItem(IDC_BUTTONSHOW)->SetWindowTextW(L"复原");
			}
		}else{
				//根据动作表恢复原先绘制的图
				CRect rect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());
				HDC hdc=m_loadImage->GetDC();
				m_backUpImage->Draw(hdc,rect,rect);		//重置为原图
				m_loadImage->ReleaseDC();
				drawmap::DrawByRecord(m_loadImage,m_records,RGB(255,0,0));
				//刷新
				m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
				GetDlgItem(IDC_BUTTONSHOW)->SetWindowTextW(L"显示");
		}
}


//合并按钮
void CCreateMapDlg::OnBnClickedButtonmeg()
{
	// --- Step.1 ---初始化参数
	unsigned int i;
	vector<int> IDIndex;
	for(i=0;i<m_njustMap.nodes.size();i++)
		IDIndex.push_back(m_njustMap.nodes[i].node.idself-START_NODE_ID);

	// --- Step.2 --- 构建子对话框
	if(m_lineDlg==NULL){   //第一次创建
		m_lineDlg=new CreateLineDlg();
		m_lineDlg->Create(IDD_CREATELINE_DIALOG,this);
	}
	m_lineDlg->Initialize(IDIndex);
	m_lineDlg->ShowWindow(SW_SHOW);	
}


//显示道路
void CCreateMapDlg::OnBnClickedButtonshowroad()
{
	int i,k;
	int index=m_listMap.GetCurSel();
	if(index!=LB_ERR){
		HDC hdc=m_loadImage->GetDC();
		CDC *pDC = CDC::FromHandle(hdc);
		for(k=0;k<m_njustMap.roads[index].pInLine.size();k++)
			pDC->SetPixel(m_njustMap.roads[index].pInLine[k],RGB(255,255,255));

		// --- Step.3---  更新图示
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
	}
}

//刷新
void CCreateMapDlg::OnBnClickedButtonf5()
{
	//绘制图形
	CRect rect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());
	HDC hdc=m_loadImage->GetDC();
	m_backUpImage->Draw(hdc,rect,rect);		//重置为原图
	m_loadImage->ReleaseDC();
	drawmap::DrawByRecord(m_loadImage,m_records,RGB(255,0,0));
	//刷新
	m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 
}

//删除
void CCreateMapDlg::OnBnClickedButtondeline()
{
	int index=m_listMap.GetCurSel();
	if(index!=LB_ERR){
		//地图结构删除
		m_njustMap.roads.erase(m_njustMap.roads.begin()+index);
		//列表中删除
		m_listMap.DeleteString(index);
		//刷新
		OnBnClickedButtonf5();

	}
}

//保存道路
void CCreateMapDlg::OnBnClickedButtondelineout()
{
	//m_njustMap.roads
}

///////////////////////业务逻辑代码//////////////////////////


//************************************
// 函数名:   DlgDrawLine
// 函数描述：对直线绘图的具体操作
// 参数: 	 CPoint point 当前点坐标(全窗体)
// 参数: 	 CRect rect   rect:控件坐标
// 返回类型: void
// 日期：	 2016/04/04
//************************************
void CCreateMapDlg::DlgDrawLine(CPoint point,CRect rect){
		// ---Step.1--- 判断当前点是直线的首端还是尾端
		if(m_lineP.x==0&&m_lineP.y==0){	  
			// --- Step.2.1 --- 记录第一个点
			point.x-=rect.left; point.y-=rect.top;
			point.x+=m_srcRect.left; point.y+=m_srcRect.top;
			m_lineP=point;
		}else{
			DRAW_RECORD dr;

			// --- Step.2.2 --- 根据两点 在图中绘制直线
			HDC hdc=m_loadImage->GetDC();
			CDC *pDC = CDC::FromHandle(hdc);
			point.x-=rect.left; point.y-=rect.top;
			point.x+=m_srcRect.left; point.y+=m_srcRect.top;
			//drawmap::DrawLine(pDC,m_lineP,point,RGB(255,0,0));
			drawmap::DrawLineBresenham(pDC,m_lineP,point,RGB(255,0,0));
			m_loadImage->ReleaseDC();

			// --- Step.2.3 --- 记录曲线上所有的点
			//CRect nrect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());			//TODO: 大图此处可优化为局部重置
			//nrect.NormalizeRect();
			//dr.drawPoints.reserve(rect.Width()+rect.Height());

			//pDC=CDC::FromHandle(m_canvas->GetDC());
			//drawmap::ResetImage(m_canvas,m_canvas1,nrect); //清除之前的后台画板上痕迹 mask
			//drawmap::DrawLine(pDC,m_lineP,point,RGB(255,0,0));		  //在后台画板上再次绘制

			//drawmap::getPointsByImage(m_canvas,nrect,dr.drawPoints);//获取绘制的点
			drawmap::LogLineBresenham(m_lineP,point,dr.drawPoints);
			/*m_canvas->ReleaseDC();*/

			// --- Step.2.4 --- 记录到"绘画动作表"中	
			dr.type=0;
			m_records.push_back(dr);
			m_listRecord.AddString(drawmap::PrintRecord(dr));

			// --- Step.2.5 --- 绘制,重置起点
			m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 
			
			m_lineP.SetPoint(0,0);
		}
		// --- Step.3 --- 重置画笔状态
		m_nowCase=Case_None;
}


//************************************
// 函数名:   DlgDrawBezier
// 函数描述：绘制曲线的具体操作
// 参数: 	 CPoint piont 当前点坐标(全窗体)
// 参数: 	 CRect rect	  rect:控件坐标
// 返回类型: void
// 日期：	 2016/04/04
//************************************
void CCreateMapDlg::DlgDrawBezier(CPoint point ,CRect rect){
	// --- Step.1 --- 判断已经绘制的点数
	if(control_bezier.index<3){
		// --- Step.2 --- 转化到图片坐标，并记录
		point.x-=rect.left; point.y-=rect.top;
		point.x+=m_srcRect.left; point.y+=m_srcRect.top;

		control_bezier.Points[control_bezier.index++]=point;
	}else if(control_bezier.index == 3){
		DRAW_RECORD dr;
		// --- Step.3.1 --- 记录最后一个(4)点的坐标，并绘制
		point.x-=rect.left; point.y-=rect.top;
		point.x+=m_srcRect.left; point.y+=m_srcRect.top;
		control_bezier.Points[control_bezier.index]=point;

		HDC hdc=m_loadImage->GetDC();
		CDC *pDC = CDC::FromHandle(hdc);
		drawmap::DrawBezier(pDC,control_bezier.Points,control_bezier.index+1,RGB(255,0,0));
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); //更新picture control
		m_loadImage->ReleaseDC();

		// --- Step.3.2 --- 记录曲线上的点
		CRect nrect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());		//TODO: 大图此处可优化为局部重置
		nrect.NormalizeRect();
		dr.drawPoints.reserve(nrect.Width()+nrect.Height());

		pDC=CDC::FromHandle(m_canvas->GetDC());
		drawmap::ResetImage(m_canvas,m_canvas1,nrect); //清除之前的后台画板上痕迹 mask
		drawmap::DrawBezier(pDC,control_bezier.Points,control_bezier.index+1,RGB(255,0,0));
		drawmap::getPointsByImage(m_canvas,nrect,dr.drawPoints);//获取绘制的点
		m_canvas->ReleaseDC();

		// --- Step.3.3 --- 记录到"绘画动作表"中	
		dr.type=1;
		m_records.push_back(dr);
		m_listRecord.AddString(drawmap::PrintRecord(dr));


		// --- Step.3.4 --- 重置画笔状态,曲线状态
		m_nowCase=Case_None;
		control_bezier.index=0;
	}
}


//************************************
// 函数名:   DlgDrawPoints
// 函数描述：绘制一系列点集 主要用于标注曲线路径和路口
// 参数: 	 CPoint point 当前点坐标(全窗体)
// 参数: 	 CRect rect rect:控件坐标
// 返回类型: void
// 日期：	 2016/04/06
//************************************
void CCreateMapDlg::DlgDrawPoints(CPoint point ,CRect rect){
	// --- Step.1 绘制 ---
	point.x-=rect.left; point.y-=rect.top;
	point.x+=m_srcRect.left; point.y+=m_srcRect.top;

	HDC hdc=m_loadImage->GetDC();
	CDC *pDC = CDC::FromHandle(hdc);
	pDC->SetPixel(point,RGB(255,0,0));

	// --- Step.2 --- 记录点
	control_points.points.push_back(point);

	// --- Step.3---  更新图示
	m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
}


//************************************
// 函数名:   DlgDrawMark
// 函数描述：绘制路口
// 参数: 	 CPoint point
// 参数: 	 CRect rect
// 返回类型: void
// 日期：	 2016/04/12
//************************************
void CCreateMapDlg::DlgDrawMark(CPoint point,CRect rect){
	// --- Step.1 --- 判断第几个点
	if(m_nodeP.x==0&&m_nodeP.y==0){
		// --- Step.2.1 --- 记录第一个点
			point.x-=rect.left; point.y-=rect.top;
			point.x+=m_srcRect.left; point.y+=m_srcRect.top;
			m_nodeP=point;
	}else{
		// --- Step.2.2 --- 绘制
			DRAW_RECORD dr;


			// --- Step.2.3 --- 记录到地图结构中  TODO 拆分成函数
			CREATE_MAP_NODE tNode;
			if(m_njustMap.nodes.size()==0){
				
				tNode.position.SetPoint(m_nodeP.x,m_nodeP.y); //路口中心位置像素坐标
				tNode.node.idself=START_NODE_ID;				//ID从START_NODE_ID开始
				m_njustMap.nodes.push_back(tNode);           
			}else{
				tNode.position.SetPoint(m_nodeP.x,m_nodeP.y); //路口中心位置像素坐标
				int lastID=m_njustMap.nodes.back().node.idself+1;	//ID从上一个节点+1 不会重复
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

			// --- Step.2.4 --- 记录圆心和边上的一点
            dr.drawPoints.push_back(m_nodeP);
			dr.drawPoints.push_back(point);

			// --- Step.2.5 --- 记录到"绘画动作表"中	
			dr.type=3;
			dr.id=tNode.node.idself-START_NODE_ID;
			m_records.push_back(dr);
			m_listRecord.AddString(drawmap::PrintRecord(dr)); 
			//m_listMap.AddString(drawmap::PrintRecord(dr));
			

			// --- Step.3 --- 绘制和重置状态
			m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 	
			m_nodeP.SetPoint(0,0);
			m_nowCase=Case_None;
	}
	
}





//************************************
// 函数名:   OnMapSetline2id
// 函数描述：响应 MAP_SETLINE_2ID 消息,完成道口构造信息
// 参数: 	 WPARAM wParam
// 参数: 	 LPARAM lParam
// 返回类型: LRESULT
// 日期：	 2016/04/15
//************************************
afx_msg LRESULT CCreateMapDlg::OnMapSetline2id(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox(L"获取消息",MB_OK);
	int i,k;
	int nItemCount = m_listRecord.GetSelCount();
	if (0 != nItemCount){
		int* indexBuf = new int[nItemCount];
		memset(indexBuf,0,nItemCount*sizeof(int));
		m_listRecord.GetSelItems(nItemCount,indexBuf);
		vector<DRAW_RECORD> tempRecord;			//临时记录选中条目
		for (i = 0 ;i<nItemCount;i++)
		{
			tempRecord.push_back(m_records[indexBuf[i]]);
		}
		delete[]indexBuf;
		//合并到节点
		CPoint *p=(CPoint*)lParam;
		if(m_njustMap.merge2Line(tempRecord,*p)){
			//重填道路列表
			m_listMap.ResetContent();
			for(k=0;k<m_njustMap.roads.size();k++)
				m_listMap.AddString(m_njustMap.printRoad(k));

		}else{
			AfxMessageBox(L"合并有误，检查是否添加非法动作",MB_OK);
		}
		delete[] p; //用完释放内存
	}
	
	return 0;
}

//???
void CCreateMapDlg::OnLbnSelchangeListrecord()
{
	// TODO: 在此添加控件通知处理程序代码
}








