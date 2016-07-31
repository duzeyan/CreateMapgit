#ifndef MSQL_MAP_H
#define MSQL_MAP_H

#include "MySqlCommand.h"
#include "MysqlConnector.h"
#include "MAP_BASIC_data.h"
#include "MapDrawControl.h"


//查询语句长度 视数据长度而定
#define  SQL_S_LEN 512  //短句  
#define  SQL_M_LEN 1024  //中等长度
#define  SQL_L_LEN 102400 //大块数据

//字符串数组长度
#define STRING_S_LEN 255  //短 255B
#define STRING_M_LEN 1024 //中等长度 1KB
#define STRING_L_LEN 102400 //最大长度 10KB

//数据库状态
typedef struct DB_STATE{
	int mapid;  //地图ID
	char smallImagePath[STRING_S_LEN];
}DB_STATE;

/////////// 以下代码为model层 

//对应 tb_win_calibration 表
typedef struct MODEL_WINCALIBRATION{
	int mapID;                //map ID
	double scaleX;           //比例尺
	double scaleY;            
	COMPUTE_GPS buildGPS[3];//标定点
}MODEL_WINCALIBRATION;

//对于 tb_win_mapnames 表
typedef struct MODEL_WINMAPNAME{
	int ID;
	char name[STRING_S_LEN];
	char imagepath[STRING_S_LEN];
	char smallImagepath[STRING_S_LEN];
}MODEL_WINMAPNAME;

//对应于 tb_win_road 表
typedef struct MODEL_WINROAD{
	CREATE_MAP_ROAD mapRoad;
}MODEL_WINROAD;

//对应于 tb_win_node 表
typedef struct MODEL_WINNODE{
	CREATE_MAP_NODE mapNode;
}MODEL_WINNODE;


#endif