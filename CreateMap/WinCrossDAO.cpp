#include "stdafx.h"
#include "WinCrossDAO.h"
#include"WinPointsDAO.h"

WinCrossDAO*  WinCrossDAO::getInstance(){
	if(!s_winCrossDAO){
		s_winCrossDAO=new (std::nothrow)WinCrossDAO(); 
	}
	return s_winCrossDAO;
}

bool WinCrossDAO::init(MYSQL* con){
	_conn=con;
	return true;
}

bool WinCrossDAO::insertEntity(const  CREATE_MAP_CROSS &model,unsigned int mapID){
	int res=-1;
	char sql[SQL_S_LEN];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------插入点对序列--------------
	auto winpoints=WinPointsDAO::getInstance();
	winpoints->init(_conn);
	unsigned int id=winpoints->insertEntity(model.points);
	if(id==-1)
		return false;

	//Step 2 -----------插入其他属性--------------
	sprintf(sql,"INSERT INTO tb_win_cross(mapID,idstart,idend,pointsID) VALUES(%d,%d,%d,%d)",
		mapID,model.idstart,model.idend,id);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}


void WinCrossDAO::getEntitiesByMapID(unsigned int mapID,vector<CREATE_MAP_CROSS> &models){
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	int pointsID=-1;
	CREATE_MAP_CROSS tCross;
	auto winpoints=WinPointsDAO::getInstance();

	winpoints->init(_conn);
	memset(sql,0,sizeof(sql));

	//Step 1 -----------------获取该地图的所有路口信息-----------
	sprintf(sql,"SELECT * FROM tb_win_cross WHERE mapID=%d",mapID);
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //
		sscanf(row[1],"%d",&tCross.idstart);
		sscanf(row[2],"%d",&tCross.idend);
		sscanf(row[3],"%d",&pointsID);
		winpoints->getPointsByID(pointsID,tCross.points);
		models.push_back(tCross);
	}
	mysql_free_result(res_set);
}

//根据主键删除数据
bool WinCrossDAO::deleteEntityByKey(unsigned int mapID,int idstart,int idend){
	int res=-1;
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	int id;
	auto winpoints=WinPointsDAO::getInstance();

	winpoints->init(_conn);
	memset(sql,0,sizeof(sql));

	//Step 1 -----------------查询从表点集ID-----------
	sprintf(sql,"SELECT FROM tb_win_cross WHERE mapID=%d AND idstart=%d AND idend=%d",mapID,idstart,idend);
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //
		sscanf(row[0],"%d",&id);
	}
	mysql_free_result(res_set);

	//Step 2 -----------删除从表数据--------------
	//if(!winpoints->deletePintsByID(id))
	//	return false;
	winpoints->deletePintsByID(id);

	//Step 1 -----------根据主键删除主表数据--------------
	memset(sql,0,sizeof(sql));
	sprintf(sql,"DELETE FROM tb_win_cross WHERE mapID=%d AND idstart=%d AND idend=%d",mapID,idstart,idend);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}

//删除该地图的所有路口信息
bool WinCrossDAO::deleteEntitiesByMapID(unsigned int mapID){
	int res=-1;
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	vector<int> vPointsIDs; //点集ID
	int id; 
	auto winpoints=WinPointsDAO::getInstance();

	winpoints->init(_conn);
	memset(sql,0,sizeof(sql));

	//Step 1 -----------------查询所有从表点集ID-----------
	sprintf(sql,"SELECT pointsID FROM tb_win_cross WHERE mapID=%d",mapID);
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ 
		sscanf(row[0],"%d",&id);
		vPointsIDs.push_back(id);
	}
	mysql_free_result(res_set);

	//Step 2 -----------删除从表数据--------------
	if(!winpoints->deleteNPointsByIDs(vPointsIDs))
		return false;

	//Step 3 -----------根据主键删除数据--------------
	memset(sql,0,sizeof(sql));
	sprintf(sql,"DELETE FROM tb_win_cross WHERE mapID=%d",mapID);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}

