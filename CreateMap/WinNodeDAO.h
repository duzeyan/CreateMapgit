#ifndef WINNODEDAO_H
#define WINNODEDAO_H

#include "map_mysql_data.h"

//对 tb_win_node 操作
class WinNodeDAO{
public:
	static WinNodeDAO* getInstance();
	bool init(MYSQL* );

private:
	MYSQL* _conn;

public :
	//根据mapID获取该地图的所有节点信息
	void getEntitiesByMapID(unsigned int mapID,vector<CREATE_MAP_NODE> &models);


	//插入一条数据
	bool insertEntity(const MODEL_WINNODE &model,unsigned int mapID);

	//根据主键删除数据
	bool deleteEntityByKey(unsigned int mapID,int idself);

	//根据地图ID删除所有信息
	bool deleteAllByMapID(unsigned int mapID);

private: 
	//************************************
	// 描述：  拼接插入功能的sql语句
	// 返回值: void
	// 参数:   char * sql [out]拼接结果 在输入前需要初始化置0
	// 参数:   const MODEL_WINNODE & model 节点属性
	// 参数:   unsigned int mapID  地图ID
	//************************************
	void getInsertSql(char *sql,const MODEL_WINNODE &model,unsigned int mapID);

	//查询的数据项转化为结构体中数据
	void row2Entity(const MYSQL_ROW &row,CREATE_MAP_NODE &model);

	
};

static WinNodeDAO* s_winNodeDAO=nullptr; 

#endif 