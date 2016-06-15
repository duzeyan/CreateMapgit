#ifndef WINRECORD_H
#define WINRECORD_H

#include "map_mysql_data.h"

//对 tb_win_drawrecord 操作
class WinDrawRecordDAO{
public:
	static WinDrawRecordDAO* getInstance();
	bool init(MYSQL* );

private:
	MYSQL* _conn;

public :
	//根据mapID获取该地图的所有绘画记录
	void getEntitiesByMapID(unsigned int mapID,vector<DRAW_RECORD> &models);

	//插入一条数据
	bool insertEntity(const  DRAW_RECORD &model,unsigned int mapID);

	//批量插入数据
	bool insertEntities(const vector<DRAW_RECORD> &vModels,unsigned int mapID);

	//根据主键删除数据
	//bool deleteEntityByKey(unsigned int mapID,int idstart,int type);

	//根据地图ID删除所有信息
	bool deleteAllByMapID(unsigned int mapID);


private: 

};

static WinDrawRecordDAO* s_WinDrawRecord=nullptr; 

#endif 