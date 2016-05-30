#include"stdafx.h"
#include"DrawMapMark.h"


//��Ŀ���� ����ֱ�� p1�׶� p2β��
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
     int ux = ((dx > 0) << 1) - 1;//x����������ȡ��-1
     int uy = ((dy > 0) << 1) - 1;//y����������ȡ��-1
     int x = p1.x, y = p1.y, eps;//epsΪ�ۼ����

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

//��¼breshenham ���Ƶĵ�����
void drawmap::LogLineBresenham(CPoint p1,CPoint p2,vector<CPoint> &pV){
	pV.clear();

	 int dx = p2.x - p1.x;
     int dy = p2.y - p1.y;
     int ux = ((dx > 0) << 1) - 1;//x����������ȡ��-1
     int uy = ((dy > 0) << 1) - 1;//y����������ȡ��-1
     int x = p1.x, y = p1.y, eps;//epsΪ�ۼ����

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


//���� �ϰ��� 
void drawmap::DrawObstacles(CDC *pdc,CPoint p1,CPoint p2,COLORREF color,int id){
	CPen pen;
	
	CPoint pt=p2-p1;
	int r=sqrt(pt.x*pt.x+pt.y*pt.y);

	pen.CreatePen(PS_SOLID,1,color);
	pdc->SelectStockObject(NULL_BRUSH);
	pdc->SelectObject(pen);
	pdc->MoveTo(p1);
	pdc->Ellipse(p1.x-r,p1.y-r,p1.x+r,p1.y+r);

	int rr=10;
	CRect textRect(p1.x+rr,p1.y-5*rr,p1.x+10*rr,p1.y-rr);

	CString outStr;
	outStr.Format(L"�ϰ���:%d",id); //ID����ʾID
	pdc->DrawTextEx(outStr,&textRect,DT_LEFT,NULL);

}

//����һ��·�ڵı�־
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
	outStr.Format(L"·��:%d",indexID+1);
	pdc->DrawTextEx(outStr,&textRect,DT_LEFT,NULL);
}

//���� ��·���
void drawmap::DrawRoadMark(CDC *pDC,const vector<CPoint> &pV,int id){
	unsigned midIndex=pV.size()/2;
	CPoint p=pV[midIndex];
	CPen pen;
	int r=10;
	CRect textRect(p.x+r,p.y-5*r,p.x+10*r,p.y-r);

	CString outStr;
	outStr.Format(L"��·:%d",id); //ID����ʾID
	pDC->DrawTextEx(outStr,&textRect,DT_LEFT,NULL);
}

//���ݼ�¼�ػ�
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
	   //road ��·
		case 4:{
					drawmap::DrawRoadMark(pDC,vRecord[i].drawPoints,vRecord[i].id);
					break;
			   }
		case 5:{
					drawmap::DrawObstacles(pDC,
											vRecord[i].drawPoints[0],
											vRecord[i].drawPoints[1],
											RGB(255,255,0),vRecord[i].id);
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

//�Զ��屴�������� ��ǰ֧���ĵ� lenΪ4
void drawmap::DrawMyBezier(CDC *pdc,CPoint points[],unsigned int len,COLORREF color){
	float dt; 
	CPoint tp=points[0]-points[3];//������Ҫ��ʾ��gps���� ��������
	int numberOfPoints=tp.x+tp.y;
	numberOfPoints*=2;
	dt = 1.0 / ( numberOfPoints - 1 ); 
	CPoint oldPoint(0,0); //�����ظ�
	for(int i = 0; i < numberOfPoints; i++) { //��ͬ�ĵ㲻���� ����¼
		CPoint drawPoint = PointOnCubicBezier( points, i*dt ); 
		if(oldPoint!=drawPoint){
			pdc->SetPixel(drawPoint,color);
			oldPoint=drawPoint;
		}
	}
}

//��¼�������������ɵĵ� Ŀǰ�汾������볤��Ϊ��
void drawmap::LogLineBresenham(CPoint points[],vector<CPoint> &pV){
	float dt; 
	CPoint tp=points[0]-points[3];//������Ҫ��ʾ��gps���� ��������
	int numberOfPoints=tp.x+tp.y;
	numberOfPoints*=2;
	dt = 1.0 / ( numberOfPoints - 1 ); 
	CPoint oldPoint(0,0); //�����ظ�
	for(int i = 0; i < numberOfPoints; i++) { //��ͬ�ĵ㲻���� ����¼
		CPoint drawPoint = PointOnCubicBezier( points, i*dt ); 
		if(oldPoint!=drawPoint){
			pV.push_back(drawPoint);
			oldPoint=drawPoint;
		}
	}
}

//��ȡ��������
CPoint drawmap::PointOnCubicBezier( CPoint cp[4], float t )
	{ 
		float ax, bx, cx; 
		float ay, by, cy; 
		float tSquared, tCubed;
		CPoint result; 
		/* �������ʽϵ�� */ 
		cx = 3.0 * (cp[1].x - cp[0].x); 
		bx = 3.0 * (cp[2].x - cp[1].x) - cx; 
		ax = cp[3].x - cp[0].x - cx - bx; 
		cy = 3.0 * (cp[1].y - cp[0].y); 
		by = 3.0 * (cp[2].y - cp[1].y) - cy; 
		ay = cp[3].y - cp[0].y - cy - by; 
		/* ����tλ�õĵ�ֵ */ 
		tSquared = t * t; 
		tCubed = tSquared * t; 
		result.x = (ax * tCubed) + (bx * tSquared) + (cx * t) + cp[0].x; 
		result.y = (ay * tCubed) + (by * tSquared) + (cy * t) + cp[0].y; 
		return result; 
	} 



//����Ӵ��Ƿ� ����ͼƬ ������������õ���Ե
void drawmap::CheckViewInImage(CRect &view,int imageW,int imageH,int viewW,int viewH){
		//����Ƿ�Խ��
		if(view.right>=imageW){ //��Խ��
			view.left=imageW-viewW;
			view.right=imageW-1;
		}
		if(view.left<0){        //��Խ��
			view.left=0;
			view.right=viewW-1;
		}
		if(view.bottom>=imageH){ //��Խ��
			view.bottom=imageH-1;
			view.top=imageH-viewH;
		}
		if(view.top<0){       //��Խ��
			view.top=0;
			view.bottom=viewH-1;
		}
}

//����ͼ������Ϊ��ɫ����
void drawmap::ResetImage(CImage *ptrImage,CImage *ptrImage2,CRect rect){
	HDC hdc=ptrImage->GetDC();
	ptrImage2->Draw(hdc,rect,rect);
	ptrImage->ReleaseDC();
}

//���غ�ɫ����Ŀ
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


//��ӡ�ṹ��
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
		//����
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
				str.Format(L"·��:%d",record.id+1);
					break;
			   }
		case 4:{
				str.Format(L"��·:%d",record.id);
				break;
			   }
		case 5:{
			str.Format(L"�ϰ���|ID:%d",record.id);
				break;
			   }
	default:
		break;
	}
	return str;
}