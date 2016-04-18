#ifndef MAP_DRAW_CONTROL_H
#define MAP_DRAW_CONTROL_H

#include"stdafx.h"
#include<iostream>
#include<vector>
#include "MAP__BASIC_data.h"

using namespace std;
//��������ϻ�ͼ

//���ڻ��Ƶ�����
enum CONTROL_CASE{
	Case_None,
	//Case_Moveing,
	Case_Line,
	Case_BEZIER,
	Case_Points,
	Case_MarkNode
	//..
};


//����������
typedef struct CONTROL_BEZIER{
	unsigned short index;		//��ǰ�ڼ�����
	CPoint Points[4];
}CONTROL_BEZIER;

//�����㼯
typedef struct  CONTORL_POINTS{
	vector<CPoint> points;		// �������points
}CONTROL_POINTS;

//��ͼ����
typedef struct DRAW_RECORD{
	int type;  //0:line 1:bezier 2:points 3:node
	int id;    //�ڵ���·��ID
	vector<CPoint> drawPoints;//���ڻ�ͼ�ĵ�
}DRAW_RECORD;

//��ͼϵͳ�е�·��
typedef struct CREATE_MAP_NODE{
	CPoint position; //ͼ������
	MAP_NODE node; //·����Ϣ
   
	
}CREATE_MAP_NODE;


//��ͼϵͳ�еĵ�·
typedef struct  CREATE_MAP_ROAD{
	CPoint position; //ͼ������
	vector<CPoint> pInLine;//��·�ϵ����ص� ������
	MAP_ROAD road; //��·��Ϣ


}CREATE_MAP_ROAD;


#define  MYLOG(msg) cout<<msg<<endl
#endif

