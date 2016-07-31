#ifndef WINROADDAO_H
#define WINROADDAO_H

#include "map_mysql_data.h"

//对 tb_win_road 操作
class WinRoadDAO{
public:
	static WinRoadDAO* getInstance();
	bool init(MYSQL* );

private:
	MYSQL* _conn;

public :
	bool insertEntity(const MODEL_WINROAD &model,unsigned int mapID);

	bool deleteEntityByKey(unsigned int mapID,int idself);

	//根据地图ID删除所有信息
	bool deleteAllByMapID(unsigned int mapID);

	//根据mapID获取该地图的所有节点信息
	void getEntitiesByMapID(unsigned int mapID,vector<CREATE_MAP_ROAD> &models);
private: 
	//************************************
	// 描述：  拼接插入功能的sql语句
	// 返回值: void
	// 参数:   char * sql [out]拼接结果 在输入前需要初始化置0
	// 参数:   const MODEL_WINROAD & model 道路属性
	// 参数:   unsigned int mapID  地图ID
	// 参数:   unsigned int pid    点集ID 
	// 参数:   unsigned int GPSid  车采GPS的ID 
	// 参数:   unsigned int GPSidNeg  逆序车采GPS的ID 
	//************************************
	void getInsertSql(char *sql,const MODEL_WINROAD &model,unsigned int mapID,unsigned int pid,int GPSid,int GPSidNeg);

	//查询的数据项转化为结构体中数据
	void row2Entity(const MYSQL_ROW &row,CREATE_MAP_ROAD &model);
};

static WinRoadDAO* s_winRoadDAO=nullptr; 

#endif 