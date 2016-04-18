#ifndef GDITEST_H
#define GDITEST_H


#include"stdafx.h"

class GDITest{
public:
	void DrawLine(CDC *pdc,CRect clientRect);
	void DraRect(CDC *pdc,CRect clientRect);
	void DrawText(CDC *pdc);

};


//draw lines
void GDITest::DrawLine(CDC *pdc,CRect clientRect){
	//pen
	CPen pen[7],*oldpen;
	//7 types
	int style[7]={PS_SOLID,PS_DASH,PS_DOT,PS_DASHDOT,PS_DASHDOTDOT,PS_NULL,PS_INSIDEFRAME};
	CPoint startPoint,endPoint;
	//CRect clientRect;
	
	startPoint.x=clientRect.left+5;
	startPoint.y=clientRect.top+5;
	endPoint.x=clientRect.right/2-5;
	endPoint.y=clientRect.top+5;

	for(int i=0;i<7;i++){
		pen[i].CreatePen(style[i],1,RGB(0,0,0));
		oldpen=pdc->SelectObject(&pen[i]);
		pdc->MoveTo(startPoint);
		pdc->LineTo(endPoint);

		pdc->SelectObject(oldpen);

		startPoint.y+=30;
		endPoint.y+=30;
		pen[i].DeleteObject();

	}



}

void GDITest::DraRect(CDC *pdc,CRect clientRect){
	CBrush * brush[6],* oldBrush;
	int style[6]={HS_BDIAGONAL,HS_CROSS,HS_DIAGCROSS,HS_FDIAGONAL,HS_HORIZONTAL,HS_VERTICAL};

	CPoint startPoint,endPoint;
	startPoint.x=clientRect.right/2+5;
	startPoint.y=clientRect.top+5;
	endPoint.x=clientRect.right-5;
	endPoint.y=clientRect.top+25;

	for(int i=0;i<6;i++){
		brush[i]=new CBrush(RGB(255,0,0));
		oldBrush=pdc->SelectObject(brush[i]);
		pdc->Rectangle(startPoint.x,startPoint.y,endPoint.x,endPoint.y);
		pdc->SelectObject(oldBrush);
		startPoint.y+=30;
		endPoint.y+=30;
		brush[i]->DeleteObject();
		delete brush[i];
	}




}

#endif