#ifndef DRAWMAPMARK_H
#define DRAWMAPMARK_H


#include"stdafx.h"
#include"MapDrawControl.h"

namespace drawmap{

	//��Ŀ���� ����ֱ�� p1�׶� p2β��
	void DrawLine(CDC *pdc,CPoint p1,CPoint p2,COLORREF color);

	//��Ŀ���� ��bresenhamֱ�߻淨��ͼ
	void DrawLineBresenham(CDC *pdc,CPoint p1,CPoint p2,COLORREF color);

	//��¼breshenham ���Ƶĵ�����
	void LogLineBresenham(CPoint p1,CPoint p2,vector<CPoint> &pV);

	//���ݼ�¼�ػ�
	void DrawByRecord(CImage *ptrImage,const vector<DRAW_RECORD>  &vRecord,COLORREF color);

	//����һ��·�ڵı�־ ���ĺͰ뾶 IDΪ·�ڸ����ID
	void DrawNodeMark(CDC *pdc,CPoint p1,unsigned int r,COLORREF color,int ID);


	//����һ����¼�ػ�
	//void DrawOneRecord(CImage *ptrImage,const DRAW_RECORD &recoed);

	//������
	void DrawBezier(CDC *pdc,CPoint points[],unsigned int len,COLORREF color);

	//�����Ƿ��� ������
	//bool CheckPInRect(CPoint p,CRect rect);  ptInRect

	//����Ӵ��Ƿ� ����ͼƬ ������������õ���Ե
	void CheckViewInImage(CRect &view,int imageW,int imageH,int viewW,int viewH);

	//����ͼ������Ϊ��ɫ����
	void ResetImage(CImage *ptrImage,CImage *ptrImage2,CRect rect);

	//���غ�ɫ����Ŀ
	void getPointsByImage(CImage *ptrImage,CRect rect,vector<CPoint> &points);

	//����б���Ŀ
	CString PrintRecord(DRAW_RECORD record);

	

}


#endif