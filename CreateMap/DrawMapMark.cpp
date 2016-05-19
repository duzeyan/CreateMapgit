#include"stdafx.h"
#include"DrawMapMark.h"


//在目标上 绘制直线 p1首端 p2尾端
void drawmap::DrawLine(CDC *pdc,CPoint p1,CPoint p2,COLORREF color){
	CPen pen;
	pen.CreatePen(PS_SOLID,1,color);
	pdc->SelectObject(pen);
	pdc->MoveTo(p1);
	pdc->LineTo(p2);
}

void drawmap::DrawLineBresenham(CDC *pdc,CPoint p1,CPoint p2,COLORREF color){
     int dx = p2.x - p1.x;
     int dy = p2.y - p1.y;
     int ux = ((dx > 0) << 1) - 1;//x的增量方向，取或-1
     int uy = ((dy > 0) << 1) - 1;//y的增量方向，取或-1
     int x = p1.x, y = p1.y, eps;//eps为累加误差

     eps = 0;dx = abs(dx); dy = abs(dy); 
     if (dx > dy) 
     {
         for (x = p1.x; x != p2.x+ux; x += ux)
         {
              pdc->SetPixel(x,y,color);
              eps += dy;
              if ((eps << 1) >= dx)
              {
                   y += uy; eps -= dx;
              }
         }
     }
     else
     {
         for (y = p1.y; y != p2.y+uy; y += uy)
         {
              pdc->SetPixel(x,y,color);
              eps += dx;
              if ((eps << 1) >= dy)
              {
                   x += ux; eps -= dy;
              }
         }
     }             
}

//记录breshenham 绘制的点序列
void drawmap::LogLineBresenham(CPoint p1,CPoint p2,vector<CPoint> &pV){
	pV.clear();

	 int dx = p2.x - p1.x;
     int dy = p2.y - p1.y;
     int ux = ((dx > 0) << 1) - 1;//x的增量方向，取或-1
     int uy = ((dy > 0) << 1) - 1;//y的增量方向，取或-1
     int x = p1.x, y = p1.y, eps;//eps为累加误差

     eps = 0;dx = abs(dx); dy = abs(dy); 
     if (dx > dy) 
     {
         for (x = p1.x; x != p2.x+ux; x += ux)
         {
              pV.push_back(CPoint(x,y));
              eps += dy;
              if ((eps << 1) >= dx)
              {
                   y += uy; eps -= dx;
              }
         }
     }
     else
     {
         for (y = p1.y; y != p2.y+uy; y += uy)
         {
             pV.push_back(CPoint(x,y));
              eps += dx;
              if ((eps << 1) >= dy)
              {
                   x += ux; eps -= dy;
              }
         }
     }           
}


//绘制 障碍物 
void drawmap::DrawObstacles(CDC *pdc,CPoint p1,CPoint p2,COLORREF color){
	CPen pen;
	
	CPoint pt=p2-p1;
	int r=sqrt(pt.x*pt.x+pt.y*pt.y);

	pen.CreatePen(PS_SOLID,1,color);
	pdc->SelectStockObject(NULL_BRUSH);
	pdc->SelectObject(pen);
	pdc->MoveTo(p1);
	pdc->Ellipse(p1.x-r,p1.y-r,p1.x+r,p1.y+r);

}

//绘制一个路口的标志
void drawmap::DrawNodeMark(CDC *pdc,CPoint p,unsigned int r,COLORREF color,int  indexID){
	CPen pen;
	
	int rr=10;
	CRect textRect(p.x+rr,p.y-5*rr,p.x+10*rr,p.y-rr);

	pen.CreatePen(PS_SOLID,1,color);
	pdc->SelectStockObject(NULL_BRUSH);
	pdc->SelectObject(pen);
	pdc->MoveTo(p);
	pdc->Ellipse(p.x-r,p.y-r,p.x+r,p.y+r);

	CString outStr;
	outStr.Format(L"路口:%d",indexID+1);
	pdc->DrawTextEx(outStr,&textRect,DT_LEFT,NULL);
}

//绘制 道路标记
void drawmap::DrawRoadMark(CDC *pDC,const vector<CPoint> &pV,int id){
	unsigned midIndex=pV.size()/2;
	CPoint p=pV[midIndex];
	CPen pen;
	int r=10;
	CRect textRect(p.x+r,p.y-5*r,p.x+10*r,p.y-r);

	CString outStr;
	outStr.Format(L"道路:%d",id); //ID是显示ID
	pDC->DrawTextEx(outStr,&textRect,DT_LEFT,NULL);
}

//根据记录重绘
void drawmap::DrawByRecord(CImage *ptrImage,const vector<DRAW_RECORD>  &vRecord,COLORREF color){
	if(vRecord.empty())
		return ;

	HDC hdc=ptrImage->GetDC();
	CDC *pDC=CDC::FromHandle(hdc);
	
	unsigned int i,j;
	for (i = 0; i < vRecord.size(); i++){
		switch (vRecord[i].type)
		{
		//line 
		case 0:{
				DrawLine(pDC, vRecord[i].drawPoints.front(),vRecord[i].drawPoints.back(),color);
				break;
			   }
		//bezier
		case 1:{
				for(j=0;j<vRecord[i].drawPoints.size();j++)
					pDC->SetPixel(vRecord[i].drawPoints[j],color);
				break;
			   }
		//points
		case 2:{
				for(j=0;j<vRecord[i].drawPoints.size();j++)
					pDC->SetPixel(vRecord[i].drawPoints[j],color);
				break;
			   }
		//node
		case 3:{
					CPoint m_nodeP=vRecord[i].drawPoints[0];
					CPoint point=vRecord[i].drawPoints[1];
					double r=(point.x-m_nodeP.x)*(point.x-m_nodeP.x)+(point.y-m_nodeP.y)*(point.y-m_nodeP.y);
					r=sqrt(r);
					drawmap::DrawNodeMark(pDC,m_nodeP,int(r),color,vRecord[i].id);
					break;
			   }
	   //road 道路
		case 4:{
					drawmap::DrawRoadMark(pDC,vRecord[i].drawPoints,vRecord[i].id);
					break;
			   }
		case 5:{
					drawmap::DrawObstacles(pDC,
											vRecord[i].drawPoints[0],
											vRecord[i].drawPoints[1],
											RGB(255,255,0));
					break;	
			   }
		default:
			break;
		}
	}
	ptrImage->ReleaseDC();
}




//
void drawmap::DrawBezier(CDC *pdc,CPoint points[],unsigned int count,COLORREF color){
	CPen pen;
	pen.CreatePen(PS_SOLID,1,color);
	pdc->SelectObject(pen);
	pdc->PolyBezier(points,count);
}

//检测视窗是否 超过图片 如果超过则设置到边缘
void drawmap::CheckViewInImage(CRect &view,int imageW,int imageH,int viewW,int viewH){
		//检查是否越界
		if(view.right>=imageW){ //右越界
			view.left=imageW-viewW;
			view.right=imageW-1;
		}
		if(view.left<0){        //左越界
			view.left=0;
			view.right=viewW-1;
		}
		if(view.bottom>=imageH){ //下越界
			view.bottom=imageH-1;
			view.top=imageH-viewH;
		}
		if(view.top<0){       //上越界
			view.top=0;
			view.bottom=viewH-1;
		}
}

//重置图像设置为黑色背景
void drawmap::ResetImage(CImage *ptrImage,CImage *ptrImage2,CRect rect){
	HDC hdc=ptrImage->GetDC();
	ptrImage2->Draw(hdc,rect,rect);
	ptrImage->ReleaseDC();
}

//返回红色的数目
void drawmap::getPointsByImage(CImage *ptrImage,CRect rect,vector<CPoint> &points){
	int x,y;

	byte * pRealData=(byte*)ptrImage->GetBits();  
	int pit=ptrImage->GetPitch();  
	int bitCount=ptrImage->GetBPP()/8;  

	for(y=rect.top;y<rect.bottom;y++) //BGR
		for(x=rect.left;x<rect.right;x++){
			if(*(pRealData + pit*y + x*bitCount+2)==255){
				points.push_back(CPoint(x,y));
			//	*(pRealData + pit*y + x*bitCount)=255;
			}
		} 
	//ptrImage->Save(L"D:\\image.bmp");
}


//打印结构体
CString drawmap::PrintRecord(DRAW_RECORD record){
	CString str;
	switch (record.type)
	{
		//line
		case 0:{
				int c=record.drawPoints.size();
				if(c==0)
					break;
				str.Format(L"Line|p1:(%d,%d)p2:(%d,%d)",record.drawPoints[0].x,record.drawPoints[0].y,
				record.drawPoints[c-1].x,record.drawPoints[c-1].y);
			 break;
			   }
		//曲线
		case 1:{
				str.Format(L"Bezier|p1:(%d,%d)p4:(%d,%d)",record.drawPoints[0].x,record.drawPoints[0].y,
				record.drawPoints[3].x,record.drawPoints[3].y);
				break;
			   }
		//points
		case 2:{
				int c=record.drawPoints.size();
				if(c==0)
					break;
				str.Format(L"Points|p1:(%d,%d)p%d:(%d,%d)",record.drawPoints[0].x,record.drawPoints[0].y,c,
				record.drawPoints[c-1].x,record.drawPoints[c-1].y);
				break;
			   }
		case 3:{
				str.Format(L"路口:%d",record.id+1);
					break;
			   }
		case 4:{
				str.Format(L"道路:%d",record.id);
				break;
			   }
		case 5:{
				str.Format(L"障碍物|p:(%d,%d)",record.drawPoints[0].x,record.drawPoints[0].y);
				break;
			   }
	default:
		break;
	}
	return str;
}