
#ifndef _NJUST_MAP_APP_H_
#define _NJUST_MAP_APP_H_


#include<vector>
#include<string.h>
#include<string>
#include<algorithm>
#include<errno.h>
#include<cmath>
#include<limits>
#include<stdio.h>

#include "NJUST_ALV_BYD_H/ALV_BYD.h"
#include"MAP_BASIC_data.h"
#include"MapFileStream.h"
#include"MapTools.h"


///已知BUG
///1.起始点如果靠近终点，导致车启动时定位到终点，在第一次进行pathplaning会出现越界错误
///2.不能在第一条路上就要求重规划，会导致段错误(由wholePath引起)

//模块运行模式
enum NJUST_MAP_WORK_MODEL
{
	MAP_WORK_NORMAL = 0x00,                  //行驶模式
	MAP_WORK_BACKDRAW ,           			 //倒车
	MAP_WORK_REPLAN  ,                       //重规划
	MAP_WORK_SHEARRIGHT,                     //搜索规划路径中的合理位置
	MAP_WORK_TASK							 //任务点模式，找点
};


// Map模块主类（业务和算法）
class MapApp{
private:
	MapFileStream* _mapFile;			   //文件类
	NJUST_MAP_BUILD_MAP _map;			   //自建地图信息
	vector<MAP_TASK_NODE_ZZ> _mapTaskNode;	   //任务文件中给出的路径，转化成自建地图中的【路口编号序列】
	NJUST_PLAN_PATH _planPath;			   //规划的路径 编号
	vector<int> _historyPath;			   //所有走过的规划路径记录 编号
	vector<MAP_DOUBLE_POINT> _GPSList;     //当前道路或者路口的GPS序列 单位是分
	int _frameNum;					       //MAP自统计帧数(pl,mo接受）
	FILE *_pRecord;					       //文件指针(状态文件的)
	int _nCurToMapCmdID;                   //发给PL的命令号
public:  
	
	//************************************
	// 描述：构造函数
	// 返回值: 无
	// 参数:   const char * loadpath 地图信息目录
	//************************************
	MapApp();
	~MapApp();

	
	//************************************
	// 描述：模拟运行MAP模块,生成结果存在特定文件中
	// 返回值: void
	//************************************
	vector<MAP_DOUBLE_POINT> simulate( );

	//************************************
	// 描述：将结果存在特定文件中
	// 返回值: void
	//************************************
	void simOutResult(const vector<MAP_DOUBLE_POINT> &list);

	//************************************
	// 描述：模拟运行MAP模块,生成结果存在特定文件中
	// 返回值: void
	//************************************
	void simMakeGPS(double lng,double lat,vector<MAP_DOUBLE_POINT> &GPSList);
	
	//************************************
	// 描述：初始化参数 
	// 返回值: void
	// 参数:   const char * loadpath  地图信息目录
	//************************************
	void initalize(const char* loadpath);

	
	//************************************
	// 描述：规划路径(选取自建地图中的节点)比如编号为1,4的节点 
	//		 则规划结构如:[1],1,[2],2,[3],3,[4] 
	//       以上[]中为路口编号,剩下的为道路编号
	//       结果存在_planPath中
	// 返回值: void
	// 参数:   int s 出发处节点的编号
	// 参数:   int e 终点处节点编号
	//************************************
	void pathPlaning(int s,int e);


	
	//************************************
	// 描述：  用Dijkstra算法找出自建地图中s路口到e路口的最短路径
	// 返回值: void
	// 参数:   int s 开始路口的编号
	// 参数:   int e 终点路口的编号
	// 参数:   vector<int> & [out]规划出的节点序列,内容为编号
	//************************************
	void dijkstra(int s,int e,vector<int>&);

	
	//************************************
	// 描述： 在车运行时,通过当前经纬度计算出车在自建地图中的位置
	// 返回值: int  ID   定位到的ID值 优先返回节点ID,其次为道路ID
	// 参数:   double lng 经度，单位度
	// 参数:   double lat 维度，单位度
	//************************************
	int location(double lng,double lat);

	
	//************************************
	// 描述： 判断经纬度是否存在某条道路主方向(南北或东西)中
	// 返回值: bool   true表示符合
	// 参数:   double lng 经度,单位度
	// 参数:   double lat 维度,单位度
	// 参数:   int lineID 道路ID
	//************************************
	bool isInLine(double lng,double lat,int lineID);

	//
	//************************************
	// 描述：根据当前GPS值,返回GPS序列中的索引
	// 返回值: int index   _GPSList GPS序列索引
	// 参数:   double lng 经度,单位度
	// 参数:   double lat 维度,单位度
	//************************************
	int locationGPS(double lng,double lat);

	
	//************************************
	// 描述： 计算路口的转弯方向
	// 返回值: void
	// 参数:   NJUST_MAP_INFO_NODE & node
	// 参数:   int curIndex  当前节点ID
	// 参数:   int lastIndex 最近节点ID
	// 参数:   int nextIndex 下一个要路过节点ID
	//************************************
	void getDirection(NJUST_MAP_INFO_NODE &node,int curIndex,int lastIndex,int nextIndex);

	
	//************************************
	// 描述：  检查location是否合理,错误则输出到日志
	// 返回值: bool		是否合理
	// 参数:   int ID   location计算出的ID
	//************************************
	bool checkLoaction(int ID);


	//************************************
	// 描述：  释放持有内存
	// 返回值: void
	//************************************
	void release(); 
};

#endif



