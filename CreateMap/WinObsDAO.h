#ifndef WINOBSDAO_H
#define WINOBSDAO_H

#include "stdafx.h"
#include "map_mysql_data.h"

// 对 tb_win_obs 表操作
class WinObsDAO{
public:
	static WinObsDAO* getInstance();
	bool init(MYSQL* );

private:
	MYSQL* _conn;

public :
	//根据mapID获取该地图的所有节点信息
	void getEntitiesByMapID(unsigned int mapID,vector<CREATE_MAP_OBSTACLE> &models);

	//插入数据
	bool  insertEntity(const CREATE_MAP_OBSTACLE &model,unsigned int mapID);

	//根据关键字删除数据
	bool deleteEntityByKey(unsigned int mapID,int id);

	//根据地图ID删除所有信息
	bool deleteAllByMapID(unsigned int mapID);
};

static WinObsDAO* s_winobsdao;
#endif