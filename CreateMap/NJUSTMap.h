#ifndef NJUST_MAP_H
#define NJUST_MAP_H


#include"MapDrawControl.h"
#include <algorithm>
using std::vector;

#define  MAP_DASH_NUM 10    //直线曲线踩点间隔



//根据绘图程序的到的结果 进一步处理
class NJUSTMap{
public:
	vector<CREATE_MAP_ROAD> roads;   //道路序列
	vector<CREATE_MAP_NODE> nodes;   //节点序列
	vector<CREATE_MAP_CROSS> crosses;//路口序列

	COMPUTE_GPS buildGPS[3];        //图中任意3点的 图片坐标和GPS
	double scaleX;					//X方向上GPS分布比例
	double scaleY;				    //Y方向上GPS分布比例
	vector<CREATE_MAP_OBSTACLE> obstacles;    //障碍物

public:
	NJUSTMap();

	//************************************
	// 函数名:   init
	// 函数描述：初始化,需要给出图片若干点的坐标和对应的GPS
	// 参数: 	 COMPUTE_GPS buildGPS[2]
	// 返回类型: void
	// 日期：	 2016/04/18
	//************************************
	void init();

	//************************************
	// 函数名:   deleteEleByID
	// 函数描述：根据类型和ID删除元素
	// 参数: 	 bool isNode :true为路口 false为道路
	// 参数: 	 int id      :路口或者道路的ID
	// 返回类型: bool        :成功返回true
	// 日期：	 2016/04/13
	//************************************
	void deleteEleByID(bool isNode,int id);

	//************************************
	// 函数名:   merge2Line
	// 函数描述：对选中的绘画记录进行合并,得到道路信息
	// 参数: 	 vector<DRAW_RECORD> mergeV :选中的绘画记录
	// 参数: 	 CPoint line2ID			    :道路的两个端点
	// 返回类型: bool                       :成功返回true
	// 日期：	 2016/04/15
	//************************************
	bool merge2Line(const vector<DRAW_RECORD> &mergeV,MAP_ROAD package);


	//************************************
	// 函数名:   merge2Cross
	// 函数描述：对选中的绘画记录进行合并,得到路口信息
	// 参数: 	 const vector<DRAW_RECORD> & mergeV
	// 参数: 	 CPoint line2ID            :路口的两端ID
	// 返回类型: bool
	// 日期：	 2016/04/27
	//************************************
	bool merge2Cross(const vector<DRAW_RECORD> &mergeV,CPoint line2ID);

	//************************************
	// 函数名:   printLine
	// 函数描述：打印一条道路信息
	// 参数: 	 unsigned int index: 索引
	// 返回类型: CString
	// 日期：	 2016/04/15
	//************************************
	CString printRoad(unsigned int index)const;


	//************************************
	// 函数名:   printCross
	// 函数描述：打印一个路口信息
	// 参数: 	 unsigned int index: 索引
	// 返回类型: CString
	// 日期：	 2016/04/27
	//************************************
	CString printCross(unsigned int index)const;

	//************************************
	// 函数名:   printLine
	// 函数描述：打印最新道路信息
	// 参数: 	 unsigned int index
	// 返回类型: CString
	// 日期：	 2016/04/15
	//************************************
	CString printRoadBack()const;

	//根据ID删除障碍物
	void removeObstaclesByID(int id);

	//在特定目录下保存二进制文件
	bool writeRoad(CString path);

	//在指定目录下保存文本文件
	bool writeRoadTxt(CString path);

	//像素转化为GPS
	void pixel2GPS(COMPUTE_GPS &p2g);

	//像素转化为GPS
	void GPS2pexel(COMPUTE_GPS &p2g);

	//检查是否已经设置好参数
	bool CheckIsCali();

	//计算尺度
	void computeScale();
	
	//************************************
	// 函数名:   serial
	// 函数描述：在指定文件持久化类
	// 参数: 	 CFile
	// 返回类型: void
	// 日期：	 2016/04/29
	//************************************
	//[文件结构]
	//... //其他部分内容
	//road大小 (uint)
	//road[i]的点对个数 (uint)
	//road[i]的n个点对 (CPoint)
	//road[i]的图中坐标 (CPoint)
	//road[i]的MAP_ROAD信息 (MAP_ROAD)
	//nodes和crosses类似
	//buildGPS scaleX scaleY
	//..//其他部分内容
	//***********************************
	void serial(CFile&);

	//反序列化类
	void enserial(CFile& file);
};

#endif