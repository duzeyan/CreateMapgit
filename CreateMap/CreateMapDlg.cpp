
// CreateMapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateMap.h"
#include "CreateMapDlg.h"
#include "afxdialogex.h"
#include <WinUser.h>
#include <stdio.h>
#include<fstream>
#include<string>
#include<sstream>

//#include"GDITest.h"
//#include"DrawMapMark.h"
#include "MapDAO.h"
#include "ToolsUtil.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCreateMapDlg 对话框
CRITICAL_SECTION g_cs;
int S_DEBUG=0;

const unsigned int RECIVE_RATE=30;


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
ON_BN_CLICKED(IDC_BUTTONMEG, &CCreateMapDlg::OnBnClickedButtonmeg)      //生成道路
ON_COMMAND(ID_MENU_MEGROAD, &CCreateMapDlg::OnBnClickedButtonmeg)  
ON_MESSAGE(MAP_SETLINE_2ID, &CCreateMapDlg::OnMapSetline2id)
ON_LBN_SELCHANGE(IDC_LISTRECORD, &CCreateMapDlg::OnLbnSelchangeListrecord)
ON_BN_CLICKED(IDC_BUTTONSHOWROAD, &CCreateMapDlg::OnBnClickedButtonshowroad)//显示道路
ON_COMMAND(ID_MENU_SHOW1, &CCreateMapDlg::OnBnClickedButtonshowroad)
ON_COMMAND(ID_MENU_REFRESH1, &CCreateMapDlg::OnBnClickedButtonf5)
ON_BN_CLICKED(IDC_BUTTONF5, &CCreateMapDlg::OnBnClickedButtonf5)
ON_BN_CLICKED(IDC_BUTTONDELINE, &CCreateMapDlg::OnBnClickedButtondeline) //删除道路
ON_COMMAND(ID_MENU_DEL1, &CCreateMapDlg::OnBnClickedButtondeline)
ON_BN_CLICKED(IDC_BUTTONDELINEOUT, &CCreateMapDlg::OnBnClickedButtondelineout) //保存道路
ON_COMMAND(ID_MENU_SAVE1, &CCreateMapDlg::OnBnClickedButtondelineout)
ON_BN_CLICKED(IDCANCEL, &CCreateMapDlg::OnBnClickedCancel)

ON_WM_TIMER()
ON_BN_CLICKED(IDC_BUTTONMEG2, &CCreateMapDlg::OnBnClickedButtonmeg2)
ON_COMMAND(ID_MENU_MEGCROSS, &CCreateMapDlg::OnBnClickedButtonmeg2)
ON_MESSAGE(MAP_SETCROSS_2ID, &CCreateMapDlg::OnMapSetcross2id)

ON_COMMAND(ID_32775, &CCreateMapDlg::OnMenuBuildMap)
ON_COMMAND(ID_32776, &CCreateMapDlg::OnOpenMap)
ON_COMMAND(ID_32777, &CCreateMapDlg::OnSaveAsMap)
ON_COMMAND(ID_32778, &CCreateMapDlg::OnMenuShowGPS)
ON_WM_CONTEXTMENU()
ON_WM_SIZE()
ON_COMMAND(ID_MENU_P1, &CCreateMapDlg::OnMenuP1)
ON_COMMAND(ID_MENU_P2, &CCreateMapDlg::OnMenuP2)
ON_COMMAND(ID_MENU_DEVIATION, &CCreateMapDlg::OnMenuDeviation)
ON_WM_DESTROY()
ON_COMMAND(ID_RESOLUTION, &CCreateMapDlg::OnResolution)
ON_COMMAND(ID_MENU_DRAWCAR, &CCreateMapDlg::OnMenuDrawcar)
ON_BN_CLICKED(IDC_BTNOB, &CCreateMapDlg::OnBnClickedBtnob)
ON_COMMAND(ID_MENU_PRO1, &CCreateMapDlg::OnMenuPro1)
ON_MESSAGE(MAP_MODIFY_LINE, &CCreateMapDlg::OnMapModifyLine)
ON_COMMAND(ID_NODEPRO, &CCreateMapDlg::OnNodepro)
ON_MESSAGE(MAP_MODIF_NODE, &CCreateMapDlg::OnMapModifNode)
ON_COMMAND(ID_MENU_P3, &CCreateMapDlg::OnMenuP3)
ON_COMMAND(ID_SAVE_LINUX, &CCreateMapDlg::OnSaveLinux)
ON_COMMAND(ID_SAVETHEMAP, &CCreateMapDlg::OnSavethemap)
ON_COMMAND(ID_MENU_SHOWALL, &CCreateMapDlg::OnMenuShowall)
ON_COMMAND(ID_SAVEMAPTASK, &CCreateMapDlg::OnSaveMapTask)
ON_COMMAND(ID_RECORDGPS, &CCreateMapDlg::OnRecordgps)
ON_COMMAND(ID_CONDB, &CCreateMapDlg::OnCondb)
ON_MESSAGE(MAP_SELDBMAP, &CCreateMapDlg::OnMapSeldbmap)
ON_COMMAND(ID_CREATEDBMAP, &CCreateMapDlg::OnCreatedbmap)
ON_COMMAND(ID_SAVEDBMAP, &CCreateMapDlg::OnSaveAsdbmap)
ON_MESSAGE(MAP_SETNAME, &CCreateMapDlg::OnMapSetname)
ON_COMMAND(ID_32832, &CCreateMapDlg::OnSaveDBMap)
ON_BN_CLICKED(IDC_BUTTON3, &CCreateMapDlg::OnBnClickedButton3)
ON_MESSAGE(MAP_GETGPS, &CCreateMapDlg::OnMapGetgps)
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

	m_isMove=false;                    //是否正在按下 移动
	m_loadImage=NULL;
	m_canvas=NULL;
	m_backUpImage=NULL;
	m_isDrawLine=false;                 //是否正在绘制直线
	CWnd *pWnd=GetDlgItem(IDC_PIC_MAIN);//获得pictrue控件窗口的句柄   
	pWnd->GetClientRect(&m_picRect);	//获得pictrue控件所在的矩形区域   
	m_pPicDC=pWnd->GetDC();				 //保持图片控件DC
	m_nowCase=Case_None;				//设置鼠标状态
	control_bezier.index=0;				//曲线初始化
	m_lineDlg=NULL;						//构建道路地图框
	m_crossDlg=NULL;					 //构建路口地图框
	m_nodeDlg=NULL;                      //节点属性
	m_dbDlg=NULL;                        //数据库属性
	m_curMapFullPath="";					//当前地图名字
	m_Show_cur=0;						//反向绘制 当前索引
	m_clockGPS=0;						//GPS接受频率
	m_isDrawCar=false;					// 是否画出车体
	m_isHighLight=false;
	m_RecordGPS=false;

	//初始化控件
	initStatusBar();                    //初始化底部状态栏
	initCtlPosition();                  //初始化记录控件位置

	//连接数据库
	m_dbcon.Connect("127.0.0.1","root","111111","njustmapdb");
	memset(&m_dbstate,0,sizeof(DB_STATE)) ;

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

void CCreateMapDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_listRect.GetCount() > 0){
		CRect dlgNow;
		GetWindowRect(&dlgNow);
		POSITION pos = m_listRect.GetHeadPosition();//第一个保存的是对话框的Rect
		CRect dlgSaved;
		dlgSaved = m_listRect.GetNext(pos);
		ScreenToClient(dlgNow);
		double x = dlgNow.Width() * 1.0 / dlgSaved.Width();//根据当前和之前保存的对话框的宽高求比例
		double y = dlgNow.Height()  *1.0 / dlgSaved.Height();
		ClientToScreen(dlgNow);
		CRect childSaved;
		CWnd* pWnd = GetWindow(GW_CHILD);
		while (pWnd)
		{
			childSaved = m_listRect.GetNext(pos);//依次获取子窗体的Rect
			childSaved.left = (LONG)(dlgNow.left + (childSaved.left - dlgSaved.left)*x);//根据比例调整控件上下左右距离对话框的距离
			childSaved.right = (LONG)(dlgNow.right + (childSaved.right - dlgSaved.right)*x);
			childSaved.top = (LONG)(dlgNow.top + (childSaved.top - dlgSaved.top)*y);
			childSaved.bottom = (LONG)(dlgNow.bottom + (childSaved.bottom - dlgSaved.bottom)*y);
			ScreenToClient(childSaved);
			pWnd->MoveWindow(childSaved);
 

			InvalidateRect(childSaved);  //立即重绘 避免残影
			pWnd = pWnd->GetNextWindow();
		}

	}
}

//善后处理
void CCreateMapDlg::OnDestroy()
{
	//关闭特定端口监听
	m_getMCInfo.ShutDownCommunication();

	//释放内存
	 if(m_statusBar!=NULL) delete m_statusBar;
	if(m_lineDlg!=NULL) delete m_lineDlg;
	if(m_crossDlg!=NULL) delete m_crossDlg; 
	if(m_loadImage!=NULL) delete m_loadImage; 
	if(m_backUpImage!=NULL) delete m_backUpImage; 
	if(m_canvas!=NULL) delete m_canvas; 

	//释放图片控件DC
	GetDlgItem(IDC_PIC_MAIN)->ReleaseDC(m_pPicDC);
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}



///////////////////////////控件事件响应///////////////////////////////////////////////

//鼠标左键落下
void CCreateMapDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// -- Step 0 -- 初始化
	CRect rect;
	GetDlgItem(IDC_PIC_MAIN)->GetWindowRect(&rect);//获取控件基于全窗体的位置
	ScreenToClient(rect);//转换为对话框上的相对位置

	// -- Step 1 -- 判断鼠标动作是否在绘图区域内
	if(rect.PtInRect(point)){
		if(!isLoad())
			return;
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
			case Case_getP1:{
									
									setCalibration(point,rect,0);
									m_njustMap.buildGPS[2]=m_njustMap.buildGPS[0];
									m_nowCase=Case_None;
									break;
							}
			case Case_getP2 :{     
									setCalibration(point,rect,1);
									m_nowCase=Case_None;
									break;
							 }
			case Case_getP3:{
									setCalibration(point,rect,2);
									m_nowCase=Case_None;
									break;
							}
			case Case_Deviation:{
									coumputerDevication(point,rect);
									m_nowCase=Case_None;
									break;
								}
			case Case_MarkObstacle:{
									DlgDrawObstacle(point,rect);
									break;
								   }
			//默认移动
			default:{
							m_isMove=true;
							SetClassLong(this->GetSafeHwnd(),
										-12, 
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
	
	//左键点击 且 已经加载过图片
	if(m_isMove&&m_loadImage!=NULL){
		 SetClassLong(this->GetSafeHwnd(), -12,(LONG)LoadCursor(NULL , IDC_HAND));
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
	bool isDraw=false;

	//正在绘曲线
	if(control_bezier.index!=0){
		isDraw=true;
	}
	//正在绘点
	if(Case_Points==m_nowCase){
		isDraw=true;
	}
	//正在绘制节点
	if(m_nodeP.x!=0){
		isDraw=true;
	}
	//正在绘制障碍物
	if(m_obP.x!=0){
		isDraw=true;
	}

	//不在绘制状态 转回正常鼠标状态
	if(!isDraw){
		SetClassLong(this->GetSafeHwnd(),
                              -12,
                             (LONG)LoadCursor(NULL , IDC_ARROW));
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

//绘直线
void CCreateMapDlg::OnBnClickedButton2()
{
	if(!isLoad())
		return;
	m_nowCase=Case_Line;			//改变绘图状态
	SetClassLong(this->GetSafeHwnd(),-12, (LONG)LoadCursor(NULL , IDC_CROSS));
}

//贝塞尔曲线test
void CCreateMapDlg::OnBnClickedButnbezier()
{
	if(!isLoad())
		return;
	m_nowCase=Case_BEZIER;
	SetClassLong(this->GetSafeHwnd(), -12,(LONG)LoadCursor(NULL , IDC_CROSS));
}


//mark状态
void CCreateMapDlg::OnBnClickedButnmarknode()
{
	if(!isLoad())
		return;
	m_nowCase=Case_MarkNode;
	SetClassLong(this->GetSafeHwnd(), -12,(LONG)LoadCursor(NULL , IDC_CROSS));
}


//孤立点集合
void CCreateMapDlg::OnBnClickedButnpoints()
{
	if(!isLoad())
		return;
	if(Case_None==m_nowCase){ // 第一次点
		m_nowCase=Case_Points;
		SetClassLong(this->GetSafeHwnd(), -12,(LONG)LoadCursor(NULL , IDC_CROSS));
	}
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

//删除选中的绘画条目
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
			if(m_records[indexBuf[i]].type==4){ //cross信息不在绘画表中删除 pass
				continue;
			}

			// --删除显示条目
            m_listRecord.DeleteString(indexBuf[i]);  

			// -- 删除路口 地图结构
			if(m_records[indexBuf[i]].type==3){						
				m_njustMap.deleteEleByID(true,m_records[indexBuf[i]].id+START_NODE_ID);
			}
			if(m_records[indexBuf[i]].type==5){  
				m_njustMap.removeObstaclesByID(m_records[indexBuf[i]].id);
			}

			// -- 删除动作记录表
			m_records.erase(m_records.begin()+indexBuf[i]);	  
        }
        delete[]indexBuf;

		// --- Step.2 --- 根据动作记录表重绘
		DlgReDraw();
    }
}

void CCreateMapDlg::DlgReDraw(){
	if(!isLoad())
		return;

	CRect rect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());
	HDC hdc=m_loadImage->GetDC();
	m_backUpImage->Draw(hdc,rect,rect);		//重置为原图
	m_loadImage->ReleaseDC();

	//绘制图形
	drawmap::DrawByRecord(m_loadImage,m_records,RGB(255,0,0));
	//刷新
	m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 
}
	
//高亮 选中条目
void CCreateMapDlg::OnBnClickedButtonshow(){
		if(!isLoad())
			return;

		if(m_isHighLight==false){
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
	
				m_isHighLight=true;
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
				m_isHighLight=false;
		}
}


//合并按钮 生成道路信息
void CCreateMapDlg::OnBnClickedButtonmeg()
{
	if(!isLoad())
		return;

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


//生成路口信息
void CCreateMapDlg::OnBnClickedButtonmeg2()
{
	if(!isLoad())
		return;

	// --- Step.1 ---初始化参数
	unsigned int i;
	vector<int> IDIndex;
	for(i=0;i<m_njustMap.roads.size();i++)
		IDIndex.push_back(m_njustMap.roads[i].road.idself-START_LINE_ID);

	// --- Step.2 --- 构建子对话框
	if(m_crossDlg==NULL){   //第一次创建
		m_crossDlg=new CreateCrossDlg();
		m_crossDlg->Create(IDD_CREATECROSS_DIALOG,this);
	}
	m_crossDlg->Initialize(IDIndex);
	m_crossDlg->ShowWindow(SW_SHOW);	
}


//显示道路或者路口 
void CCreateMapDlg::OnBnClickedButtonshowroad()
{
	if(!isLoad())
		return;

	unsigned int k;
	int index=m_listMap.GetCurSel();
	if(index!=LB_ERR){
		HDC hdc=m_loadImage->GetDC();
		CDC *pDC = CDC::FromHandle(hdc);
		int roadsLen=m_njustMap.roads.size();
		if(index<roadsLen){   //显示最终的道路序列
			for(k=0;k<m_njustMap.roads[index].pInLine.size();k++)
				pDC->SetPixel(m_njustMap.roads[index].pInLine[k],RGB(255,255,255));
		}else{                //显示最终的路口序列
			for(k=0;k<m_njustMap.crosses[index-roadsLen].points.size();k++)
				pDC->SetPixel(m_njustMap.crosses[index-roadsLen].points[k],RGB(255,255,255));
		}
		
	
		m_loadImage->ReleaseDC();
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
	}
}

//显示所有道路
void CCreateMapDlg::OnMenuShowall()
{
	if(!isLoad())
		return;
	HDC hdc=m_loadImage->GetDC();
	CDC *pDC = CDC::FromHandle(hdc);
	int k,i;
	int roadsLen=m_njustMap.roads.size();
	
	//显示最终的道路序列
	for(auto &troad:m_njustMap.roads){   
		for(auto &point:troad.pInLine)
			pDC->SetPixel(point,RGB(255,255,255));
	}
	
	//显示最终的路口序列
	for(auto &tcross:m_njustMap.crosses){   
		for(auto &point:tcross.points)
			pDC->SetPixel(point,RGB(255,255,255));
	}
		
	
	m_loadImage->ReleaseDC();
	m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
}


//刷新
void CCreateMapDlg::OnBnClickedButtonf5()
{
	if(!isLoad())
		return;

	//绘制图形
	CRect rect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());
	HDC hdc=m_loadImage->GetDC();
	m_backUpImage->Draw(hdc,rect,rect);		//重置为原图
	m_loadImage->ReleaseDC();
	drawmap::DrawByRecord(m_loadImage,m_records,RGB(255,0,0));
	//刷新
	m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 
}

//删除道路或路口
void CCreateMapDlg::OnBnClickedButtondeline()
{
	if(!isLoad())
		return;

	int index=m_listMap.GetCurSel();
	if(index!=LB_ERR){
		int ls=m_njustMap.roads.size();
		if(index<ls){						//删除道路
			//删除绘画列表中的标注
			int showid=m_njustMap.roads[index].road.idself-START_LINE_ID+1;
			for(unsigned int i=0;i<m_records.size();i++){
				if(m_records[i].id==showid&&m_records[i].type==4){
					m_records.erase(m_records.begin()+i); //删除记录
					m_listRecord.DeleteString(i);         //删除显示条目
					break;
				}
			}
			//地图结构删除
			//m_njustMap.roads.erase(m_njustMap.roads.begin()+index);
			m_njustMap.deleteRoadByIndex(index);
			//列表中删除
			m_listMap.DeleteString(index);
			
			//重绘
			DlgReDraw();
		}else{								//删除路口
			//地图结构删除 路口
			m_njustMap.crosses.erase(m_njustMap.crosses.begin()+(index-ls));
			
			//列表中删除
			m_listMap.DeleteString(index);
		}
		//刷新
		OnBnClickedButtonf5();

	}
}

//保存道路
void CCreateMapDlg::OnBnClickedButtondelineout()
{
	if(!isLoad())
		return;

	if(!m_njustMap.CheckIsCali()){
		AfxMessageBox(L"地图未标定，请先标定",MB_OK);
		return;
	}

	CString path=L"D:\\map";
	if(!m_njustMap.writeRoad(path)){
		AfxMessageBox(L"保存失败",MB_OK);
	}else{
		m_njustMap.writeRoadTxt(path);

		CString msg;
		msg.Format(L"保存在%s目录下",path);
		AfxMessageBox(msg,MB_OK);
	}
}


//标记障碍物
void CCreateMapDlg::OnBnClickedBtnob()
{
	if(!isLoad())
		return;
	m_nowCase=Case_MarkObstacle;			//改变绘图状态
	SetClassLong(this->GetSafeHwnd(), -12,(LONG)LoadCursor(NULL , IDC_CROSS));
}


///////////////////////绘图逻辑代码//////////////////////////


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
			dr.id=dr.type=0;

			// --- Step.2.2 --- 根据两点 在图中绘制直线
			HDC hdc=m_loadImage->GetDC();
			CDC *pDC = CDC::FromHandle(hdc);
			point.x-=rect.left; point.y-=rect.top;
			point.x+=m_srcRect.left; point.y+=m_srcRect.top;
			//drawmap::DrawLine(pDC,m_lineP,point,RGB(255,0,0));
			drawmap::DrawLineBresenham(pDC,m_lineP,point,RGB(255,0,0));
			m_loadImage->ReleaseDC();

			// --- Step.2.3 --- 记录曲线上所有的点
			drawmap::LogLineBresenham(m_lineP,point,dr.drawPoints);

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

		//drawmap::DrawBezier(pDC,control_bezier.Points,control_bezier.index+1,RGB(255,0,0));
		drawmap::DrawMyBezier(pDC,control_bezier.Points,control_bezier.index+1,RGB(255,0,0));

		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); //更新picture control
		m_loadImage->ReleaseDC();

		// --- Step.3.2 --- 记录曲线上的点
		drawmap::LogLineBresenham(control_bezier.Points,dr.drawPoints);
		//CRect nrect(0,0,m_loadImage->GetWidth(),m_loadImage->GetHeight());		//TODO: 大图此处可优化为局部重置
		//nrect.NormalizeRect();
		//dr.drawPoints.reserve(nrect.Width()+nrect.Height());

		//pDC=CDC::FromHandle(m_canvas->GetDC());
		//drawmap::ResetImage(m_canvas,m_backUpImage,nrect); //清除之前的后台画板上痕迹 mask
		//drawmap::DrawBezier(pDC,control_bezier.Points,control_bezier.index+1,RGB(255,0,0));
		//drawmap::getPointsByImage(m_canvas,nrect,dr.drawPoints);//获取绘制的点
		//m_canvas->ReleaseDC();



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
	m_loadImage->ReleaseDC();

	// --- Step.2 --- 记录点
	control_points.points.push_back(point);

	// --- Step.3---  更新图示
	m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
}


//************************************
// 函数名:   DlgDrawObstacle
// 函数描述：绘制障碍物
// 参数: 	 CPoint point 当前点坐标(全窗体)
// 参数: 	 CRect rect rect:控件坐标
// 返回类型: void
// 日期：	 2016/05/17
//************************************
void CCreateMapDlg::DlgDrawObstacle(CPoint point ,CRect rect){
	// --- Step.1 --- 判断第几个点
	if(m_obP.x==0&&m_obP.y==0){
		// --- Step.2.1 --- 记录第一个点
			point.x-=rect.left; point.y-=rect.top;
			point.x+=m_srcRect.left; point.y+=m_srcRect.top;
			m_obP=point;
	}else{
		// --- Step.2.2 --- 绘制
			DRAW_RECORD dr;
	

			HDC hdc=m_loadImage->GetDC();
			CDC *pDC = CDC::FromHandle(hdc);
			point.x-=rect.left; point.y-=rect.top;
			point.x+=m_srcRect.left; point.y+=m_srcRect.top;
	
			

			// --- Step.2.4 --- 记录圆心和边上的一点
            dr.drawPoints.push_back(m_obP);
			dr.drawPoints.push_back(point);

			// --- Step.2.5 --- 记录到"绘画动作表"中	
			int theID=0;
			reverse(m_records.begin(),m_records.end());  //倒置
			auto it=find(m_records.begin(),m_records.end(),5); //找到倒置后的第一个障碍物
			if(it!=m_records.end()){   //找到最后一个
				theID=it->id+1;
			}

			reverse(m_records.begin(),m_records.end()); //倒置回去
			dr.id=theID;
			dr.type=5;
			m_records.push_back(dr);
			m_listRecord.AddString(drawmap::PrintRecord(dr)); 

			drawmap::DrawObstacles(pDC,m_obP,point,RGB(255,255,0),theID);
			m_loadImage->ReleaseDC();


			// --- Step.2.6 --- 记录到地图结构中	
			CREATE_MAP_OBSTACLE ob;
			ob.id=theID;
			ob.x=m_obP.x;
			ob.y=m_obP.y;
			CPoint p=point-m_obP;
			ob.rPix=sqrt(p.x*p.x+p.y*p.y);
			m_njustMap.obstacles.push_back(ob);
			

			// --- Step.3 --- 绘制和重置状态
			m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 	
			m_obP.SetPoint(0,0);
			m_nowCase=Case_None;
	}
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
			memset(&tNode.node,0,sizeof(MAP_NODE));
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
// 函数描述：响应 MAP_SETLINE_2ID 消息,完成道路构造信息
// 参数: 	 WPARAM wParam
// 参数: 	 LPARAM lParam
// 返回类型: LRESULT
// 日期：	 2016/04/15
//************************************
afx_msg LRESULT CCreateMapDlg::OnMapSetline2id(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox(L"获取消息",MB_OK);
	unsigned int i,k;
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
		MAP_ROAD *pPackage=(MAP_ROAD*)lParam;
		if(m_njustMap.merge2Line(tempRecord,*pPackage)){
			//绘画动作列表添加动作
			DRAW_RECORD dr;
			// --- Step.2.2 --- 根据两点 在图中绘制直线
			HDC hdc=m_loadImage->GetDC();
			CDC *pDC = CDC::FromHandle(hdc);
			m_loadImage->ReleaseDC();
			//填写记录
			dr.type=4;
			dr.id=m_njustMap.roads.back().road.idself-START_LINE_ID+1;
			dr.drawPoints=m_njustMap.roads.back().pInLine;
			//绘制图形
			//drawmap::DrawByRecord(m_loadImage,m_records,RGB(255,0,0));
			drawmap::DrawRoadMark(pDC,dr.drawPoints,dr.id);
		
			m_records.push_back(dr);
			//这里记录和道路列表同步 切记
			m_listRecord.AddString(drawmap::PrintRecord(dr));  

			// --- Step.2.5 --- 绘制,重置
			DlgReDraw();
			m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); 



			//重填道路列表
			m_listMap.ResetContent();
			for(k=0;k<m_njustMap.roads.size();k++)
				m_listMap.AddString(m_njustMap.printRoad(k));
			for(k=0;k<m_njustMap.crosses.size();k++)
				m_listMap.AddString(m_njustMap.printCross(k));
		}else{
			AfxMessageBox(L"合并有误，检查是否添加非法动作",MB_OK);
		}
		delete[] pPackage; //用完释放内存
	}else{
		AfxMessageBox(L"未选取绘图动作",MB_OK);
	}
	
	return 0;
}




//处理合并路口信息
//************************************
// 函数名:   OnMapSetcross2id
// 函数描述：响应 MAP_SETCROSS_2ID 消息,完成路口构造信息
// 参数: 	 WPARAM wParam
// 参数: 	 LPARAM lParam
// 返回类型: LRESULT
// 日期：	 2016/04/27
//************************************
afx_msg LRESULT CCreateMapDlg::OnMapSetcross2id(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox(L"获取消息",MB_OK);
	unsigned int i,k;
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
		if(m_njustMap.merge2Cross(tempRecord,*p)){

			//重填道路列表
			m_listMap.ResetContent();
			for(k=0;k<m_njustMap.roads.size();k++)
				m_listMap.AddString(m_njustMap.printRoad(k));
			for(k=0;k<m_njustMap.crosses.size();k++)
				m_listMap.AddString(m_njustMap.printCross(k));
		}else{
			AfxMessageBox(L"合并有误，检查是否添加非法动作",MB_OK);
		}
		delete[] p; 
	}else{
		AfxMessageBox(L"未选取绘图动作",MB_OK);
	}
	
	return 0;
}


//************************************
// 函数名:   serial
// 函数描述：序列化数据
// 返回类型: void
// 日期：	 2016/04/29
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

//反序列化
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
		AfxMessageBox(L"请先新建或载入地图",MB_OK);
		return false;
	}
	return true;
}

////////////////////////////////////控件初始化//////////////////////////////////////



//动态调整控件位置 
void CCreateMapDlg::initCtlPosition(){
	CRect rect;
	GetWindowRect(&rect);
	m_listRect.AddTail(rect);//对话框的区域
	CWnd *pWnd = GetWindow(GW_CHILD);//获取子窗体
	while (pWnd)
	{
		pWnd->GetWindowRect(rect);//子窗体的区域
		m_listRect.AddTail(rect);           //CList<CRect,CRect> m_listRect成员变量
		pWnd = pWnd->GetNextWindow();//取下一个子窗体
	}
}

void CCreateMapDlg::initStatusBar(){
	m_statusBar=new CStatusBarCtrl;
	CRect m_Rect; 
	GetClientRect(&m_Rect); //获取对话框的矩形区域
	m_Rect.top=m_Rect.bottom-20; //设置状态栏的矩形区域
	m_statusBar->Create(WS_BORDER|WS_VISIBLE|CBRS_BOTTOM,m_Rect,this,3);
	int nParts[2]= {m_Rect.Width()/2,-1}; //分割尺寸
	m_statusBar->SetParts(2, nParts); //分割状态栏
	m_statusBar->SetText(L"当前地图:【未打开地图】",0,0); //第一个分栏加入"这是第一个指示器"
	
	
}





////////////////////////////GPS序列动态显示在图中//////////////////////////////////////////////



void CCreateMapDlg::OnLbnSelchangeListrecord()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CCreateMapDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
	
}

void CCreateMapDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(m_Show_cur!=m_Show_GPSList.size()){
		// 计算
		COMPUTE_GPS gps;
		gps.lng=m_Show_GPSList[m_Show_cur].x/60.0; //文件中是分 
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
		//更新
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);

	}else{
		CWnd::KillTimer(1);
		m_Show_cur=0;
		if(m_Show_GPSList.size()!=0)
		AfxMessageBox(L"绘制完成",MB_OK);
	}
	CDialog::OnTimer(nIDEvent);
}



////////////////////////////主菜单栏操作//////////////////////////////////////////////

//新建地图
void CCreateMapDlg::OnMenuBuildMap()
{
	//已经有数据
	if(m_loadImage!=NULL){
		INT_PTR re=AfxMessageBox(L"是否放弃当前工作空间,新建地图?",MB_OKCANCEL);
		if(re!=IDOK)
			return;
	}

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



		CWnd *pWnd=GetDlgItem(IDC_PIC_MAIN);//获得pictrue控件窗口的句柄      
    

		//初始化地图结构
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
		m_njustMap.init();  //重新初始化 reset
		m_records.clear();    //删除已有地图

		//清除列表
		m_listMap.ResetContent();
		m_listRecord.ResetContent();
		//修改状态栏
		CString strbar;
		strbar.Format(L"当前地图:【%s】",L"*未命名");
		
		m_statusBar->SetText(strbar,0,0);

		m_srcRect=m_picRect;
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); //将图片画到Picture控件表示的矩形区域  
	}
}

//打开地图
void CCreateMapDlg::OnOpenMap()
{
	//根据载入的地图重绘
	 // --- Step.1 ---选择保存位置	
	 CString fileName;
	 CFileDialog dlg(TRUE, 
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("二进制文件 (*.map)|*.map|All Files (*.*)|*.*||"),
        NULL);
	if(dlg.DoModal()==IDOK){
		fileName=dlg.GetPathName(); //文件名
		m_curMapFullPath=fileName;
		CFile file;
		// --- Step.2 --- 数据
		if(file.Open(fileName,CFile::modeRead)){
			//读取m_njustmap
			m_njustMap.enserial(file);
			//保存m_record
			enserial(file);
			file.Close();
		}
		int nPos= fileName.ReverseFind('.');
		CString bmpFileName = fileName.Left(nPos); //yyy
	    bmpFileName+=L".bmp";
		

		if(m_loadImage!=NULL) delete m_loadImage;
		if(m_backUpImage!=NULL) delete m_backUpImage;
		if(m_canvas!=NULL) delete m_canvas;
		m_loadImage=new CImage();
		m_loadImage->Load(bmpFileName); //根据图片路径加载图片  
		m_backUpImage=new CImage();
		m_backUpImage->Load(bmpFileName); //根据图片路径加载图片  
		m_canvas=new CImage();
		m_canvas->Create(m_loadImage->GetWidth(),m_loadImage->GetHeight(),24);
		//重置视窗
		m_srcRect=m_picRect;

		//重绘图像
		DlgReDraw();
		//重置list
		m_listRecord.ResetContent();
		unsigned int k;
		for(k=0;k<m_records.size();k++)
			m_listRecord.AddString(drawmap::PrintRecord(m_records[k]));
		m_listMap.ResetContent();
		for(k=0;k<m_njustMap.roads.size();k++)
				m_listMap.AddString(m_njustMap.printRoad(k));
		for(k=0;k<m_njustMap.crosses.size();k++)
				m_listMap.AddString(m_njustMap.printCross(k));

		//修改状态栏
		int nPos1=fileName.ReverseFind('\\');
		CString strbar=fileName.Right(fileName.GetLength()-nPos1-1);
		strbar=strbar.Left(strbar.GetLength()-4); //.map(4)
		m_statusBar->SetText(L"当前地图:【"+strbar+L"】",0,0);
		
	}
}

//保存当前地图
void CCreateMapDlg::OnSavethemap()
{
	CFile file;
	if(m_curMapFullPath!=""){
		if(file.Open(m_curMapFullPath,CFile::modeCreate|CFile::modeWrite)){
				//保存m_njustmap
				m_njustMap.serial(file);
				//保存绘画记录
				serial(file);
				file.Close();
				AfxMessageBox(L"保存成功",MB_OK);	   
		}
	}
}

//另存地图
void CCreateMapDlg::OnSaveAsMap()
{
	if(!isLoad())
		return;
	 // --- Step.1 ---选择保存位置	
	 CString fileName;
	 CFileDialog dlg(FALSE, 
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("定义地图名 |All Files (*.*)|*.*||"),
        NULL);
	if(dlg.DoModal()==IDOK){
		fileName=dlg.GetPathName(); //文件名 C:\\xxx\\yyy
		CFile file;
		// --- Step.2 --- 保存m_njustmap(.map)
		if(file.Open(fileName+L".map",CFile::modeCreate|CFile::modeWrite)){
			//保存m_njustmap
			m_njustMap.serial(file);
			//保存绘画记录
			serial(file);
			file.Close();
		}
		// --- Step.3 --- 保存图片
		m_backUpImage->Save(fileName+L".bmp");

		//修改状态栏
		int nPos1=fileName.ReverseFind('\\');
		CString strbar=fileName.Right(fileName.GetLength()-nPos1-1);
		m_statusBar->SetText(L"当前地图:【"+strbar+L"】",0,0);
		AfxMessageBox(L"保存成功",MB_OK);	    
	}
}

//读取显示GPS序列
void CCreateMapDlg::OnMenuShowGPS()
{
	if(!isLoad())
		return;
	if(!m_njustMap.CheckIsCali()){
		AfxMessageBox(L"地图未标定，请先标定",MB_OK);
		return;
	}

	CString FilePathName;
    CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("DB Files (*.db)|*.db|All Files (*.*)|*.*||"),
        NULL);
	if(dlg.DoModal()==IDOK){
		FilePathName=dlg.GetPathName(); //文件名
		m_Show_GPSList.clear();

		CFile pf;
		MAP_DOUBLE_POINT tPoint;  //读db
		if(pf.Open(FilePathName,CFile::modeRead|CFile::modeNoTruncate)){
			int GPSnum = pf.GetLength() / sizeof(MAP_DOUBLE_POINT);
			for(int i=0;i<GPSnum;i++){
					pf.Read(&tPoint,sizeof(MAP_DOUBLE_POINT)); //fread(&tPoint, sizeof(MAP_DOUBLE_POINT), 1, pf);
					m_Show_GPSList.push_back(tPoint);  //记录
			}
		}else{
			AfxMessageBox(L"读取DB文件失败",MB_OK);
		}
	}
	CWnd::SetTimer(1,100,NULL);
}

//导出linux地图
void CCreateMapDlg::OnSaveLinux()
{
	if(!isLoad())
		return;
	m_njustMap.saveBuildMapForLinux(L"D:\\map\\board.db");
	m_njustMap.saveAdjForLinux(L"D:\\map\\adjust.db");
}

//生成任务文件
void CCreateMapDlg::OnSaveMapTask()
{
	if(!isLoad())
		return;

	vector<string> strLines;
	string oneline;
	ifstream  fTask;
	string filename="D:\\map\\task.txt";
	vector<MAP_TASK_NODE> vTask; //字符串解析成路点结构
	vector<MAP_TASK_NODE> vOutTask; //用于生成initalNodequeue
	fTask.open(filename);
	if(fTask.is_open()){
		//按行读入strLines
		while (getline(fTask,oneline)){
			strLines.push_back(oneline);
		}
		fTask.close();

		//解析到结构体中
		for(auto &tline:strLines){
			istringstream stream(tline);
			MAP_TASK_NODE tasknode;
			double height;     //没用到高度 暂时剔除
			stream >> tasknode.num      >> tasknode.longtitude
				   >> tasknode.latitude >> height 
				   >> tasknode.shuxing1 >> tasknode.shuxing2;
			vTask.push_back(tasknode);
		}

		int num=0; //用于tOutTask的编号
		for (auto &tTasknode:vTask)
		{
			if (tTasknode.shuxing1 == 0 || tTasknode.shuxing1 == 7 || tTasknode.shuxing1 == 1)
			{
				int result = m_njustMap.getIndexByGPS(tTasknode.longtitude, tTasknode.latitude);
				//找到最近的点
				if (result == -1) //没有ID返回
					continue;
				if(vOutTask.empty()){ //第一次不用跟前帧对比
					MAP_TASK_NODE tasknode;
					tasknode.num = num;
					tasknode.longtitude = tTasknode.longtitude;//以分为单位
					tasknode.latitude = tTasknode.latitude;
					tasknode.noderesult = result;//不在路网中的点一次减小
					tasknode.shuxing1 = tTasknode.shuxing1;
					tasknode.shuxing2 = tTasknode.shuxing2;
					tasknode.duiyingludianbianhao=tTasknode.num;
					vOutTask.push_back(tasknode);
					continue;
				}
				//重复不考虑
				if(vOutTask.back().noderesult!=result){
					MAP_TASK_NODE tasknode;
					tasknode.num = num;
					tasknode.longtitude = tTasknode.longtitude;//以分为单位
					tasknode.latitude = tTasknode.latitude;
					tasknode.noderesult = result;//不在路网中的点一次减小
					tasknode.shuxing1 = tTasknode.shuxing1;
					tasknode.shuxing2 = tTasknode.shuxing2;
					tasknode.duiyingludianbianhao=tTasknode.num;
					vOutTask.push_back(tasknode);
				}
			}
		}

		//写原始任务点文件InitialNodeQueue
		CFile fileOut;
		fileOut.Open(L"D:\\map\\InitialNodeQueue.db",CFile::modeCreate|CFile::modeWrite);
		FILE *pft = fopen("D:\\map\\InitialNodeQueue.txt", "w");
		for (auto &tTasknode:vOutTask){
			fileOut.Write(&tTasknode, sizeof(MAP_TASK_NODE));
			fprintf(pft,"%d\n",tTasknode.noderesult);
		}
		fileOut.Close();
		fclose(pft);

		TRACE("结构体长度：%d",sizeof(ROADNODE));
		//测试读
		pft = fopen("D:\\map\\InitialNodeQueue.db", "rb");
		fseek(pft, 0L, SEEK_END);
		int len = ftell(pft) / sizeof(ROADNODE);
		fseek(pft, 0L, SEEK_SET);
		ROADNODE tROAD;
		for(int i=0;i<len;i++){
			fread(&tROAD, sizeof(ROADNODE), 1, pft);
		}
		fclose(pft);
	}else{
		AfxMessageBox(L"未找到文件",MB_OK);
	}
}

/////////////////////////////上下文菜单栏操作/////////////////////////////////////////////

void CCreateMapDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
   CRect rectDraw,rectMap;
   m_listRecord.GetWindowRect(&rectDraw);
   m_listMap.GetWindowRect(&rectMap);
   CPoint p;
   GetCursorPos(&p);
   //绘画记录点击
   if (rectDraw.PtInRect(p)||rectMap.PtInRect(p))
    {
       CMenu m_Menu;
       m_Menu.LoadMenu(IDR_MENUPOP);//编辑好的菜单资源
	   CMenu *m_SubMenu=NULL;
	   if(rectDraw.PtInRect(p))
		  m_SubMenu= m_Menu.GetSubMenu(0);
	   else
		  m_SubMenu= m_Menu.GetSubMenu(1);
       m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);
   }

}

//修改道路属性
void CCreateMapDlg::OnMenuPro1()
{
	if(!isLoad())
		return;

	int index=m_listMap.GetCurSel();
	if(index!=LB_ERR){
		int ls=m_njustMap.roads.size();
		if(index<ls){						//选中了道路
			//删除绘画列表中的标注
			int showid=m_njustMap.roads[index].road.idself-START_LINE_ID+1;

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

			m_lineDlg->setModify(m_njustMap.roads[index].road);
			m_lineDlg->ShowWindow(SW_SHOW);	

			
	
		}else{								//选中了路口
			//地图结构删除 路口
			//m_njustMap.crosses.erase(m_njustMap.crosses.begin()+index-ls);
			//列表中删除
			//m_listMap.DeleteString(index);
		}


	}
}

//响应修改道路信息消息
afx_msg LRESULT CCreateMapDlg::OnMapModifyLine(WPARAM wParam, LPARAM lParam)
{
	MAP_ROAD* package=(MAP_ROAD*)lParam;

	//id需要转化
	package->idstart=package->idstart+START_NODE_ID-1;
	package->idend=package->idend+START_NODE_ID-1;

	//补充设置ID
	for(int i=0;i<m_njustMap.roads.size();i++){
		if(m_njustMap.roads[i].road.idself==package->idself){ //找到目标
			m_njustMap.roads[i].road=*package;				 //替换
			break;
		}
	}

	return 0;
}

//修改节点(node)属性
void CCreateMapDlg::OnNodepro()
{
	if(!isLoad())
		return;

	int nItemCount = m_listRecord.GetSelCount();
	if (0 != nItemCount){
		if(nItemCount==1){ //此时只能选中一条记录且为 路口
			int* indexBuf = new int;
			memset(indexBuf,0,nItemCount*sizeof(int));
			m_listRecord.GetSelItems(nItemCount,indexBuf);
			//vector<DRAW_RECORD> tempRecord;			//临时记录选中条目

			//必须是道路节点
			if(m_records[*indexBuf].type==3){
				//节点窗口
				if(m_nodeDlg==NULL){
					m_nodeDlg=new ModifyNode();
					m_nodeDlg->Create(IDD_MODIFYNODE_DIALOG,this);
				}
				int searchID=m_records[*indexBuf].id+START_NODE_ID ; //通过ID查找到指定节点的索引
				int searchIndex=-1; 
				for(int i=0;i<m_njustMap.nodes.size();i++){
					if(m_njustMap.nodes[i].node.idself==searchID){
						searchIndex=i;
					}
				}

				m_nodeDlg->setValue(m_njustMap.nodes[searchIndex].node);
				m_nodeDlg->ShowWindow(SW_SHOW);	
			}
			delete[]indexBuf;
		}
	}
}

afx_msg LRESULT CCreateMapDlg::OnMapModifNode(WPARAM wParam, LPARAM lParam)
{
	auto pnode=(MAP_NODE*)lParam;
	int searchID=pnode->idself ; //通过ID查找到指定节点的索引
	int searchIndex=-1; 
	for(int i=0;i<m_njustMap.nodes.size();i++){
		if(m_njustMap.nodes[i].node.idself==searchID){
			searchIndex=i;
		}
	}
	m_njustMap.nodes[searchIndex].node=*pnode;
	return 0;
}


/////////////////////////////标定相关操作/////////////////////////////////////////////
void CCreateMapDlg::showNowGPS(char *buff,long len){
	//Step1 控制接受频率
	CString s;
	s.Format(L"m_clockGPS: %d \n",m_clockGPS);
	TRACE(s);
	m_clockGPS++;
	m_clockGPS%=10000; //避免越界
	if(m_clockGPS%RECIVE_RATE!=0){ //50取1
		return ;
	}

	double longlat[2];

	m_getMCInfo.getGPSAndPostion(buff,len,longlat); //TODO
	if((longlat[0]-0)<0.001f) //获取0   不更新
		return;
	//转化为度
	longlat[0]/=60;
	longlat[1]/=60;
	///Test 118.85644481,32.02762140
	//longlat[0]=118.85690144;
	//longlat[1]=32.02764064;
	///

	CString strShow;
	strShow.Format(L"经度(°):%.8lf 纬度(°):%.8lf",longlat[0],longlat[1]);

	m_RealGPS.x=longlat[0];
	m_RealGPS.y=longlat[1];

	drawMyCar(longlat);      //在车中绘制
	m_statusBar->SetText(strShow,1,0); 
}


//获取第一个点
void CCreateMapDlg::OnMenuP1()
{
	 m_nowCase=Case_getP1;
	SetClassLong(this->GetSafeHwnd(), -12,(LONG)LoadCursor(NULL , IDC_CROSS));

}

//获取第二个点
void CCreateMapDlg::OnMenuP2()
{
	m_nowCase=Case_getP2;
	SetClassLong(this->GetSafeHwnd(), -12,(LONG)LoadCursor(NULL , IDC_CROSS));

}

void CCreateMapDlg::OnMenuP3()
{
	m_nowCase=Case_getP3;
	SetClassLong(this->GetSafeHwnd(), -12,(LONG)LoadCursor(NULL , IDC_CROSS));

}


//计算误差
void CCreateMapDlg::OnMenuDeviation()
{
	m_nowCase=Case_Deviation;
	SetClassLong(this->GetSafeHwnd(), -12,(LONG)LoadCursor(NULL , IDC_CROSS));
}

//设置标定数据 鼠标点击位置(客户区) 图片控件大小(rect) 标定点索引
void CCreateMapDlg::setCalibration(CPoint point,CRect rect,int index){
		if(!isLoad())
			return;

		if(abs(m_RealGPS.x-.0l)<0.001)
			AfxMessageBox(L"未能获取GPS信息",MB_OK);
		else{
			point.x-=rect.left; point.y-=rect.top;
			point.x+=m_srcRect.left; point.y+=m_srcRect.top;
			m_njustMap.buildGPS[index].x=point.x;
			m_njustMap.buildGPS[index].y=point.y;
			m_njustMap.buildGPS[index].lng=m_RealGPS.x;
			m_njustMap.buildGPS[index].lat=m_RealGPS.y;
			//默认基础点
			CString strShow;
			strShow.Format(L"P%d 设置完成:px(%d,%d) GPS(%.8lf ,%.8lf)",
				index+1,point.x,point.y,m_RealGPS.x,m_RealGPS.y);
			AfxMessageBox(strShow,MB_OK);
		}
}

//计算误差
void CCreateMapDlg::coumputerDevication(CPoint point,CRect rect){
	if(!isLoad())
		return;
	if(abs(m_RealGPS.x-.0l)<0.001){
			AfxMessageBox(L"未能获取GPS信息",MB_OK);
			return;
	}

	point.x-=rect.left; point.y-=rect.top;
	point.x+=m_srcRect.left; point.y+=m_srcRect.top;
	if(m_njustMap.CheckIsCali()){
		m_njustMap.computeScale();//计算尺度
		COMPUTE_GPS cGPS;
		cGPS.x=point.x;cGPS.y=point.y;
		m_njustMap.pixel2GPS(cGPS);

		CString strShow;
		//计算距离
		int cx,cy;
		int rx,ry;
		m_getMCInfo.blh2xy(cGPS.lat,cGPS.lng,cx,cy);
		m_getMCInfo.blh2xy(m_RealGPS.y,m_RealGPS.x,rx,ry);
		double delta=(cx-rx)*(cx-rx)+(cy-ry)*(cy-ry);
		delta=sqrt(delta);
		strShow.Format(L"计算GPS:(%.8lf,%.8lf) 距离为%.5lfcm",cGPS.lng,cGPS.lat,delta);
		AfxMessageBox(strShow,MB_OK);
	}else{
		AfxMessageBox(L"地图未标定，请先标定",MB_OK);
	}
}


//计算分辨率
void CCreateMapDlg::OnResolution()
{
	if(m_njustMap.CheckIsCali()){
		COMPUTE_GPS cGPS,cGPS1;  //设置两个坐标
		cGPS.x=0;cGPS.y=0;
		cGPS1.x=1;cGPS1.y=0;

		m_njustMap.pixel2GPS(cGPS);  //根据标定结果换算成GPS
		m_njustMap.pixel2GPS(cGPS1);

		int x,y;
		int x1,y1; 
		m_getMCInfo.blh2xy(cGPS.lat,cGPS.lng,x,y);   //换算成cm
		m_getMCInfo.blh2xy(cGPS1.lat,cGPS1.lng,x1,y1);
		double delta1=(x1-x)*(x1-x)+(y1-y)*(y1-y);
		delta1=sqrt(delta1);

		//同样步骤计算Y方向上的分辨率
		cGPS1.x=0;cGPS1.y=1;
		m_getMCInfo.blh2xy(cGPS1.lat,cGPS1.lng,x1,y1);
		double delta2=(x1-x)*(x1-x)+(y1-y)*(y1-y);
		delta2=sqrt(delta2);

		CString strShow;
		strShow.Format(L"x,y方向上分辨率为 %.5lfcm/pix %.5lfcm/pix",delta1,delta2);
		AfxMessageBox(strShow,MB_OK);
	}else{
		AfxMessageBox(L"地图未标定，请先标定",MB_OK);
	}
}

//修改绘制车体开关
void CCreateMapDlg::OnMenuDrawcar()
{
	// --- Step.1 --- 检查操作合法性
	if(!isLoad())    //检查是否载入地图
		return;
	
	if(!m_njustMap.CheckIsCali()){   //检查地图是否标定
		AfxMessageBox(L"未能获取GPS信息",MB_OK);
			return;
	}
	if(abs(m_RealGPS.x-.0l)<0.001){   //检查能否活惯导GPS
			AfxMessageBox(L"未能获取GPS信息",MB_OK);
			return;
	}
	// --- Step.2---  打开/关闭
	m_isDrawCar=m_isDrawCar?false:true;
}

//绘制车体
void CCreateMapDlg::drawMyCar(double longlat[2]){

	// --- Step.0 --- 检查开关是否打开
	if(!m_isDrawCar)  
		return ;


	// --- Step.1 --- 获取图片DC
	HDC hdc=m_loadImage->GetDC();
	CDC *pDC = CDC::FromHandle(hdc);
	

	// --- Step.2 --- 记录点
	pDC->SelectStockObject(WHITE_BRUSH);
	COMPUTE_GPS cg;
	cg.lng=longlat[0];
	cg.lat=longlat[1];
	m_njustMap.GPS2pexel(cg);
	CPoint p(cg.x,cg.y);
	pDC->MoveTo(p);
	int r=2;  //半径
	pDC->Ellipse(p.x-r,p.y-r,p.x+r,p.y+r);

	m_loadImage->ReleaseDC();
	// --- Step.3---  更新图示
	m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect);
}





void CCreateMapDlg::OnRecordgps()
{
	

}

//////////////////////////数据库操作////////////////////////////////////////

//打开地图(从数据库)
void CCreateMapDlg::OnCondb()
{
	// --- Step.1 --- 查询所有的地图
	mysql_ping(m_dbcon.GetMysqlObject());
	vector<MODEL_WINMAPNAME> vMapNames;
	auto winmapname=WinMapNameDAO::getInstance();
	winmapname->init(m_dbcon.GetMysqlObject());
	winmapname->getAllEntities(vMapNames);

	// --- Step.2 --- 构建子对话框
	if(m_dbDlg==NULL){   //第一次创建
		m_dbDlg=new SelectMapDlg();
		m_dbDlg->Create(IDD_SELMAP_DIALOG,this);
	}
	m_dbDlg->init(vMapNames);
	m_dbDlg->ShowWindow(SW_SHOW);	
}

//接送子窗口发送的自定义消息 MAP_SELDBMAP
afx_msg LRESULT CCreateMapDlg::OnMapSeldbmap(WPARAM wParam, LPARAM lParam){
	int* pID=(int*)lParam;
	auto windr=WinDrawRecordDAO::getInstance();
	windr->init(m_dbcon.GetMysqlObject());
	auto winmapname=WinMapNameDAO::getInstance();
	winmapname->init(m_dbcon.GetMysqlObject());

	//Step 1 -----------获取地图ID--------------
	m_dbstate.mapid=*pID;
	delete pID;

	//Step 2 -----------加载地图结构--------------
	m_njustMap.enserial2DB(m_dbcon.GetMysqlObject(),m_dbstate.mapid);

	//Step 3 -----------加载绘图信息--------------
	windr->getEntitiesByMapID(m_dbstate.mapid,m_records);

	//Step 4 -----------加载地图图片--------------
	MODEL_WINMAPNAME winn;
	memset(&winn,0,sizeof(MODEL_WINMAPNAME));
	winmapname->getEntityByID(m_dbstate.mapid,winn);
	CString bmpFileName(winn.imagepath);
	CString mapName(winn.name);

	//Step 4 -----------根据记录绘制--------------
	if(m_loadImage!=NULL) delete m_loadImage;
	if(m_backUpImage!=NULL) delete m_backUpImage;
	if(m_canvas!=NULL) delete m_canvas;
	m_loadImage=new CImage();
	m_loadImage->Load(bmpFileName); //根据图片路径加载图片  
	m_backUpImage=new CImage();
	m_backUpImage->Load(bmpFileName); //根据图片路径加载图片  
	m_canvas=new CImage();
	m_canvas->Create(m_loadImage->GetWidth(),m_loadImage->GetHeight(),24);
	m_srcRect=m_picRect;	//重置视窗	

	//重绘图像
	DlgReDraw();

	//Step 5 -----------重置列表--------------
	m_listRecord.ResetContent(); //绘画记录列表
	unsigned int k;
	for(k=0;k<m_records.size();k++)
		m_listRecord.AddString(drawmap::PrintRecord(m_records[k]));
	m_listMap.ResetContent();

	for(k=0;k<m_njustMap.roads.size();k++)  //地图信息列表
		m_listMap.AddString(m_njustMap.printRoad(k));
	for(k=0;k<m_njustMap.crosses.size();k++)
		m_listMap.AddString(m_njustMap.printCross(k));

	//Step 5 ----------修改状态栏--------------
	m_statusBar->SetText(L"当前地图:【"+mapName+L"】",0,0);

	return 0;
}

//在数据库上 新建地图(没有保存)
void CCreateMapDlg::OnCreatedbmap()
{
	//已经有数据
	if(m_loadImage!=NULL){
		INT_PTR re=AfxMessageBox(L"是否放弃当前工作空间,新建地图?",MB_OKCANCEL);
		if(re!=IDOK)
			return;
	}

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

		CWnd *pWnd=GetDlgItem(IDC_PIC_MAIN);//获得pictrue控件窗口的句柄      
		//初始化地图结构
		m_njustMap.init();  //重新初始化 reset
		m_records.clear();    //删除已有地图

		//清除列表
		m_listMap.ResetContent();
		m_listRecord.ResetContent();


		//修改状态栏
		CString strbar;
		strbar.Format(L"当前地图:【%s】",L"*未命名");
		m_statusBar->SetText(strbar,0,0);
		m_srcRect=m_picRect;
		m_loadImage->Draw(m_pPicDC->m_hDC,m_picRect,m_srcRect); //将图片画到Picture控件表示的矩形区域  
	}
}


//在数据库上 另存地图(命名)
void CCreateMapDlg::OnSaveAsdbmap()
{
	// --- Step.1 --- 查询所有的地图
	mysql_ping(m_dbcon.GetMysqlObject());
	vector<MODEL_WINMAPNAME> vMapNames;
	auto winmapname=WinMapNameDAO::getInstance();
	winmapname->init(m_dbcon.GetMysqlObject());
	winmapname->getAllEntities(vMapNames);

	// --- Step.2 --- 构建子对话框
	if(m_nameDlg==NULL){   //第一次创建
		m_nameDlg=new SetNameDlg();
		m_nameDlg->Create(IDD_NAMEMAP_DIALOG,this);
	}
	m_nameDlg->init(vMapNames);
	m_nameDlg->ShowWindow(SW_SHOW);	
}

//MAP_SETNAME
afx_msg LRESULT CCreateMapDlg::OnMapSetname(WPARAM wParam, LPARAM lParam)
{
	CString* pStrName=(CString*)lParam;
	MODEL_WINMAPNAME model;

	//Step 1 -----------设置地图名和文件位置--------------
	unsigned int strlen=0;
	CString path=L"D:\\\\map\\\\image\\\\";
	CString imagepath=path+*pStrName+L".bmp";
	m_backUpImage->Save(imagepath);

	ToolsUtil::WtoA(model.name,255,pStrName);
	ToolsUtil::WtoA(model.imagepath,255,&imagepath);

	//Step 2 -----------存入数据库--------------
	mysql_ping(m_dbcon.GetMysqlObject());
	auto winmapname=WinMapNameDAO::getInstance();
	winmapname->init(m_dbcon.GetMysqlObject());
	winmapname->insertEntity(model);
	m_dbstate.mapid=model.ID;              //获取插入后反馈的ID

	//Step 3 -----------修改状态栏--------------
	CString strbar;
	strbar.Format(L"当前地图:【%s】",*pStrName);
	m_statusBar->SetText(strbar,0,0);

	delete pStrName;
	return 0;
}


//保存地图
void CCreateMapDlg::OnSaveDBMap()
{
	//Step 1 -----------判断是否可以保存--------------
	if(m_dbstate.mapid==0){
		AfxMessageBox(L"请先另存数据库!",MB_OK);
		return;
	}
	
	//Step 2 -----------确认连接--------------
	mysql_ping(m_dbcon.GetMysqlObject()); 

	//Step 3 -----------更新地图结构数据到数据库--------------
	m_njustMap.serial2DB(m_dbcon.GetMysqlObject(), //存入地图拓扑结构和属性
							m_dbstate.mapid); 

	//Step 4 ----------更新绘制地图信息到数据库--------------
	auto winDrawRecord=WinDrawRecordDAO::getInstance();
	winDrawRecord->init(m_dbcon.GetMysqlObject());
	winDrawRecord->deleteAllByMapID(m_dbstate.mapid); //先删除原有数据
	winDrawRecord->insertEntities(m_records,m_dbstate.mapid);
}


void CCreateMapDlg::OnBnClickedButton3()
{
	//初始化标定相关
	m_RealGPS.x=m_RealGPS.y=0.0f;			 //默认接受的GPS为0  表示没有接受到数据
	m_getMCInfo.StartUdpCommunication(this); //建立与惯导的通信
}

//消息机制 获取GPS
afx_msg LRESULT CCreateMapDlg::OnMapGetgps(WPARAM wParam, LPARAM lParam)
{
	//废弃代码
	//MAP_DOUBLE_POINT *p=(MAP_DOUBLE_POINT*)lParam;
	//double longlat[2];
	//CString str;
	//str.Format(L"%lf,%lf\n",p->x,p->y);
	////TRACE(str);
	//longlat[0]=p->x;
	//longlat[1]=p->y;

	//m_clockGPS++;
	//m_clockGPS%=10000; //避免越界
	//if(m_clockGPS%RECIVE_RATE!=0){ //50取1
	//	return 0;
	//}

	////if((p->x-0)<0.001f) //获取0   不更新
	////	return 0;
	//////转化为度
	//longlat[0]/=60;
	//longlat[1]/=60;
	///////Test 118.85644481,32.02762140
	//////longlat[0]=118.85690144;
	//////longlat[1]=32.02764064;
	///////

	//CString strShow;
	//strShow.Format(L"经度(°):%.8lf 纬度(°):%.8lf",longlat[0],longlat[1]);

	////m_RealGPS.x=longlat[0];
	////m_RealGPS.y=longlat[1];


	////drawMyCar(longlat);      //在车中绘制
	//m_statusBar->SetText(strShow,1,0);
	//delete p;
	//return 0;
}
