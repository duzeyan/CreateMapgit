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
	Case_None,       //默认操作
	Case_Line,       //画直线
	Case_BEZIER,     //画曲线
	Case_Points,      //画孤立点
	Case_MarkNode,     //标注路口
	Case_getP1,         //标定点1
	Case_getP2,        //标定点2
	Case_getP3,		   //标定点3
	Case_Deviation,     //计算误差
	Case_MarkObstacle  //标记障碍物
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
	int type;  //0:line 1:bezier 2:points 3:node 4:road 5:障碍物
	int id;    //节点或道路的ID
	vector<CPoint> drawPoints;//用于绘图的点
	bool operator==(const int& type) // 操作运算符重载 比较类型相等
    {
        return this->type==type;
    }
}DRAW_RECORD;

//绘图系统中的路口
typedef struct CREATE_MAP_NODE{
	CPoint position; //图中坐标
	MAP_NODE node; //路口信息
}CREATE_MAP_NODE;

//路口信息
typedef struct CREATE_MAP_CROSS{
	int idstart;			//路口进入ID
	int idend;				//路口出ID
   vector<CPoint> points;   //图中的路口坐标序列
}CREATE_MAP_CROSS;


//绘图系统中的道路
typedef struct  CREATE_MAP_ROAD{
	CPoint position; //图中坐标
	vector<CPoint> pInLine;//道路上的像素点 非连续
	MAP_ROAD road; //道路信息


}CREATE_MAP_ROAD;


#define  MYLOG(msg) cout<<msg<<endl
#endif

