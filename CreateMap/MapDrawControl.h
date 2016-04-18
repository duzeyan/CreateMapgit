#ifndef MAP_DRAW_CONTROL_H
#define MAP_DRAW_CONTROL_H

#include"stdafx.h"
#include<iostream>
#include<vector>
#include "MAP__BASIC_data.h"

using namespace std;
//鼠标操作配合绘图

//正在绘制的内容
enum CONTROL_CASE{
	Case_None,
	//Case_Moveing,
	Case_Line,
	Case_BEZIER,
	Case_Points,
	Case_MarkNode
	//..
};


//贝塞尔曲线
typedef struct CONTROL_BEZIER{
	unsigned short index;		//当前第几个点
	CPoint Points[4];
}CONTROL_BEZIER;

//孤立点集
typedef struct  CONTORL_POINTS{
	vector<CPoint> points;		// 鼠标点击的points
}CONTROL_POINTS;

//绘图操作
typedef struct DRAW_RECORD{
	int type;  //0:line 1:bezier 2:points 3:node
	int id;    //节点或道路的ID
	vector<CPoint> drawPoints;//用于绘图的点
}DRAW_RECORD;

//绘图系统中的路口
typedef struct CREATE_MAP_NODE{
	CPoint position; //图中坐标
	MAP_NODE node; //路口信息
   
	
}CREATE_MAP_NODE;


//绘图系统中的道路
typedef struct  CREATE_MAP_ROAD{
	CPoint position; //图中坐标
	vector<CPoint> pInLine;//道路上的像素点 非连续
	MAP_ROAD road; //道路信息


}CREATE_MAP_ROAD;


#define  MYLOG(msg) cout<<msg<<endl
#endif

