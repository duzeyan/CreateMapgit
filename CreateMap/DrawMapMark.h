#ifndef DRAWMAPMARK_H
#define DRAWMAPMARK_H


#include"stdafx.h"
#include"MapDrawControl.h"

namespace drawmap{

	//在目标上 绘制直线 p1首端 p2尾端
	void DrawLine(CDC *pdc,CPoint p1,CPoint p2,COLORREF color);

	//在目标上 用bresenham直线绘法绘图
	void DrawLineBresenham(CDC *pdc,CPoint p1,CPoint p2,COLORREF color);

	//记录breshenham 绘制的点序列
	void LogLineBresenham(CPoint p1,CPoint p2,vector<CPoint> &pV);

	//根据记录重绘
	void DrawByRecord(CImage *ptrImage,const vector<DRAW_RECORD>  &vRecord,COLORREF color);

	//绘制一个路口的标志 中心和半径 ID为路口赋予的ID
	void DrawNodeMark(CDC *pdc,CPoint p1,unsigned int r,COLORREF color,int ID);


	//根据一条记录重绘
	//void DrawOneRecord(CImage *ptrImage,const DRAW_RECORD &recoed);

	//画曲线
	void DrawBezier(CDC *pdc,CPoint points[],unsigned int len,COLORREF color);

	//检查点是否在 矩形内
	//bool CheckPInRect(CPoint p,CRect rect);  ptInRect

	//检测视窗是否 超过图片 如果超过则设置到边缘
	void CheckViewInImage(CRect &view,int imageW,int imageH,int viewW,int viewH);

	//重置图像设置为白色背景
	void ResetImage(CImage *ptrImage,CImage *ptrImage2,CRect rect);

	//返回红色的数目
	void getPointsByImage(CImage *ptrImage,CRect rect,vector<CPoint> &points);

	//输出列表项目
	CString PrintRecord(DRAW_RECORD record);

	

}


#endif