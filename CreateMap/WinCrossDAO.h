#ifndef WINCROSSDAO_H
#define WINCROSSDAO_H

#include "map_mysql_data.h"

//对 tb_win_cross 操作
class WinCrossDAO{
public:
	static WinCrossDAO* getInstance();
	bool init(MYSQL* );

private:
	MYSQL* _conn;

public :
	bool insertEntity(const  CREATE_MAP_CROSS &model,unsigned int mapID);

	void getEntitiesByMapID(unsigned int mapID,vector<CREATE_MAP_CROSS> &model);

	//根据主键删除数据
	bool deleteEntityByKey(unsigned int mapID,int idstart,int idend);

	//删除该地图的所有路口信息
	bool deleteEntitiesByMapID(unsigned int mapID);
private: 
};

static WinCrossDAO* s_winCrossDAO=nullptr; 

#endif 