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
	Case_None,       //Ĭ�ϲ���
	Case_Line,       //��ֱ��
	Case_BEZIER,     //������
	Case_Points,      //��������
	Case_MarkNode,     //��ע·��
	Case_getP1,         //�궨��1
	Case_getP2,        //�궨��2
	Case_getP3,		   //�궨��3
	Case_Deviation,     //�������
	Case_MarkObstacle  //����ϰ���
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
	int type;  //0:line 1:bezier 2:points 3:node 4:road 5:�ϰ���
	int id;    //�ڵ���·��ID
	vector<CPoint> drawPoints;//���ڻ�ͼ�ĵ�
	bool operator==(const int& type) // ������������� �Ƚ��������
    {
        return this->type==type;
    }
}DRAW_RECORD;

//��ͼϵͳ�е�·��
typedef struct CREATE_MAP_NODE{
	CPoint position; //ͼ������
	MAP_NODE node; //·����Ϣ
}CREATE_MAP_NODE;

//·����Ϣ
typedef struct CREATE_MAP_CROSS{
	int idstart;			//·�ڽ���ID
	int idend;				//·�ڳ�ID
   vector<CPoint> points;   //ͼ�е�·����������
}CREATE_MAP_CROSS;


//��ͼϵͳ�еĵ�·
typedef struct  CREATE_MAP_ROAD{
	CPoint position; //ͼ������
	vector<CPoint> pInLine;//��·�ϵ����ص� ������
	MAP_ROAD road; //��·��Ϣ


}CREATE_MAP_ROAD;


#define  MYLOG(msg) cout<<msg<<endl
#endif

