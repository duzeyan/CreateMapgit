// ShowSmallDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CreateMap.h"
#include "ShowSmallDlg.h"
#include "afxdialogex.h"


// ShowSmallDlg 对话框

IMPLEMENT_DYNAMIC(ShowSmallDlg, CDialogEx)

ShowSmallDlg::ShowSmallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ShowSmallDlg::IDD, pParent)
{
	_isTransPath=false;
	_isTransTask=false;
}

ShowSmallDlg::~ShowSmallDlg()
{
}

void ShowSmallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ShowSmallDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()


void ShowSmallDlg::init(CString imagepath){
	_smallImage.Destroy();
	_smallImage.Load(imagepath);
	return;
}


void ShowSmallDlg::setPosition(CPoint p){
	_nowIndexX=p.x;
	_nowIndexY=p.y;
}

CRect ShowSmallDlg::getDrawRect(){
	int imageH=_smallImage.GetHeight();
	int imageW=_smallImage.GetWidth();
	CRect rect;
	CPoint LTpoint,RBpoint;
	double scale; //缩放比例尺
	if(imageH>imageW){//图片高度为基准
		if(imageH<_picRect.Height()){//图片小于图片控件
			LTpoint.SetPoint((_picRect.Width()-imageW)/2,(_picRect.Height()-imageH)/2);
			RBpoint.SetPoint(LTpoint.x+imageW,LTpoint.y+imageH);
			rect.SetRect(LTpoint,RBpoint);
		}else{
			scale=_picRect.Height()*1.0/imageH;//<1
			LTpoint.SetPoint((_picRect.Width()-imageW*scale)/2,0);
			RBpoint.SetPoint(LTpoint.x+imageW*scale,_picRect.Height());
			rect.SetRect(LTpoint,RBpoint);
		}
	}else{ 
		if(imageW<_picRect.Width()){//图片小于图片控件
			LTpoint.SetPoint((_picRect.Width()-imageW)/2,(_picRect.Height()-imageH)/2);
			RBpoint.SetPoint(LTpoint.x+imageW,LTpoint.y+imageH);
			rect.SetRect(LTpoint,RBpoint);
		}else{
		
			scale=_picRect.Width()*1.0/imageW;//<1
			LTpoint.SetPoint(0,(_picRect.Height()-imageH*scale)/2);
			RBpoint.SetPoint(_picRect.Width(),LTpoint.y+imageH*scale);
			rect.SetRect(LTpoint,RBpoint);
		}
	}
	return rect;
}

void ShowSmallDlg::setSrcImageInfo(int width,int height,int blockW,int blockH){
	_srcW=width;
	_srcH=height;
	_srcBlockW=blockW;
	_srcBlockH=blockH;
	return;
}

//给出全局点
void ShowSmallDlg::setPathPix(vector<CPoint> &pixs){
	_isTransPath=false;
	_pathList=pixs;
	return;
}

//设置任务路点数据
void ShowSmallDlg::setTaskPix(vector<MAP_TASK_NODE_ZZ> &tasks){
	_isTransTask=false;
	_tasks=tasks;
	return;
}


// ShowSmallDlg 消息处理程序
void ShowSmallDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// -- Step 0 -- 初始化
	CRect rect;
	GetDlgItem(IDC_PICSMALL)->GetWindowRect(&rect);//获取控件基于全窗体的位置
	GetDlgItem(IDC_PICSMALL)->MoveWindow(CRect(0,0,100,100));
	ScreenToClient(rect);//转换为对话框上的相对位置
	//_drawRect
	//Step 1 -----------定位--------------
	if(_drawRect.PtInRect(point)){
		CWnd *pWnd=GetDlgItem(IDC_PICSMALL);
		_picDC=pWnd->GetDC();
		CPoint imageP=point-_drawRect.TopLeft();//缩略图片
		double dx=_drawRect.Width()*1.0/_srcBlockW;
		double dy=_drawRect.Height()*1.0/_srcBlockH;
		int indexX=imageP.x/dx;
		int indexY=imageP.y/dy;
		CString strShow;
		strShow.Format(L"点定位在(%d,%d)",indexX,indexY);
		//AfxMessageBox(strShow,MB_OK);
		int *pDxDy=new int[2];
		pDxDy[0]=_nowIndexX=indexX;
		pDxDy[1]=_nowIndexY=indexY;
		::SendMessage(GetParent()->GetSafeHwnd(),MAP_LOCATION,0,(LPARAM)pDxDy);//向父窗体通知

		//高亮选择区域
		drawCorr();
		higthLightBlock();

		pWnd->ReleaseDC(_picDC);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

//画出分割线
void ShowSmallDlg::drawCorr(){
	int baseH=_drawRect.top;
	int baseW=_drawRect.left;
	int deltaW=_drawRect.Width()/_srcBlockW;
	int deltaH=_drawRect.Height()/_srcBlockH;
	int i,j;

	//Step 1 -----------准备画笔--------------
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
	_picDC->SelectObject(pen);
	CPoint p;

	//Step 2 -----------绘制水平线--------------
	for(i=0;i<_drawRect.Height();i+=deltaH){
		p.SetPoint(baseW,i+baseH);
		_picDC->MoveTo(p);
		p.SetPoint(_drawRect.Width()+baseW,i+baseH);
		_picDC->LineTo(p);
	}

	//Step 3 -----------绘制竖直线--------------
	for(i=0;i<_drawRect.Width();i+=deltaW){
		p.SetPoint(baseW+i,baseH);
		_picDC->MoveTo(p);
		p.SetPoint(baseW+i,baseH+_drawRect.Height());
		_picDC->LineTo(p);
	}
	return;
}

//高亮块
void ShowSmallDlg::higthLightBlock(){
	int baseH=_drawRect.top;
	int baseW=_drawRect.left;
	int deltaW=_drawRect.Width()/_srcBlockW;
	int deltaH=_drawRect.Height()/_srcBlockH;

	//Step 1 -----------准备画笔--------------
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)
	GetStockObject(NULL_BRUSH));
	CPen pen;
	pen.CreatePen(PS_SOLID,3,RGB(255,0,255));
	_picDC->SelectObject(pen);
	_picDC->SelectObject(pBrush);
	CPoint p1,p2;

	//Step 2 -----------绘制矩形--------------
	p1.SetPoint(baseW+_nowIndexX*deltaW,baseH+_nowIndexY*deltaH);
	p2.SetPoint(baseW+(_nowIndexX+1)*deltaW,baseH+(_nowIndexY+1)*deltaH);
	_picDC->Rectangle(p1.x,p1.y,p2.x,p2.y);
}


void ShowSmallDlg::OnPaint()
{
	//CPaintDC dc(this); // 

	//Step 1 -----------加载图片控件DC--------------
	CWnd *pWnd=GetDlgItem(IDC_PICSMALL);
	pWnd->MoveWindow(CRect(0,0,600,600));
	pWnd->GetClientRect(&_picRect);
	_picDC=pWnd->GetDC();

	//Step 2 -----------绘制图片--------------
	_drawRect=getDrawRect();
	CRect imageRect;
	imageRect.SetRect(0,0,_smallImage.GetWidth(),_smallImage.GetHeight());
	_picDC->SetStretchBltMode(COLORONCOLOR); //避免图片缩放失真
	_smallImage.Draw(_picDC->m_hDC,_drawRect,imageRect);

	//Step 3 -----------显示path--------------
	if(!_isTransPath){ //未转化过
		double scale=_drawRect.Width()*1.0/_srcW; //计算比例尺
		double x,y;
		int baseH=_drawRect.top;
		int baseW=_drawRect.left;
		for (auto &p:_pathList)
		{
			x=p.x*scale;
			y=p.y*scale;
			p.x=(int)x+baseW;//向下取整
			p.y=(int)y+baseH;
		}
		_isTransPath=true;//标记转化过
	}
	//Step 4 -----------显示task--------------
	if(!_isTransTask){ //未转化过
		double scale=_drawRect.Width()*1.0/_srcW; //计算比例尺
		double x,y;
		int baseH=_drawRect.top;
		int baseW=_drawRect.left;
		for (auto &ts:_tasks) //经纬度中存的是转化过的坐标
		{
			x=ts.longtitude*scale;
			y=ts.latitude*scale;
			ts.longtitude=(int)x+baseW;//向下取整
			ts.latitude=(int)y+baseH;
		}
		_isTransTask=true;//标记转化过
	}
	drawmap::DrawTaskPoint(_picDC,_tasks); //绘制任务点
	drawmap::DrawPlanPath(_picDC,_pathList);//绘制预演结果
	//Step 5 -----------绘制分割线--------------
	drawCorr();
	higthLightBlock();

	pWnd->ReleaseDC(_picDC);
	CDialogEx::OnPaint();
}

afx_msg void ShowSmallDlg::OnDestroy(){
	CDialog::OnDestroy();
}
