#include"stdafx.h"
#include "WinDrawRecordDAO.h"
#include "WinPointsDAO.h"

WinDrawRecordDAO* WinDrawRecordDAO::getInstance(){
	if(!s_WinDrawRecord){
		s_WinDrawRecord=new (std::nothrow)WinDrawRecordDAO(); 
		if(!s_WinDrawRecord)
			return nullptr;
	}
	return s_WinDrawRecord;
}

bool WinDrawRecordDAO::init(MYSQL *con){
	_conn=con;
	return true;
}

bool WinDrawRecordDAO::insertEntity(const  DRAW_RECORD &model,unsigned int mapID){
	int res=-1;
	char sql[SQL_S_LEN];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------插入点对序列--------------
	auto winpoints=WinPointsDAO::getInstance();
	winpoints->init(_conn);
	unsigned int id=winpoints->insertEntity(model.drawPoints);
	if(id==-1)
		return false;

	//Step 2 -----------插入其他属性--------------
	memset(sql,0,sizeof(sql));
	sprintf(sql,"INSERT INTO tb_win_drawrecord(mapID,id,type,pointsID) ",
				mapID,model.id,model.type,id);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}

//批量插入数据
bool WinDrawRecordDAO::insertEntities(const vector<DRAW_RECORD> &vModels,unsigned int mapID){
	int res=-1;
	char sql[SQL_S_LEN];
	bool isAllRight=true;
	memset(sql,0,sizeof(sql));
	unsigned int id=-1;
	auto winpoints=WinPointsDAO::getInstance();
	winpoints->init(_conn);

	mysql_autocommit(_conn,0);
	for(const auto &model:vModels){
		//Step 1 -----------插入点对序列--------------
		id=-1;
		id=winpoints->insertEntity(model.drawPoints);
		if(id==-1){
			isAllRight=false;
			break;
		}

		//Step 2 -----------插入其他属性--------------
		memset(sql,0,sizeof(sql));
		sprintf(sql,"INSERT INTO tb_win_drawrecord(mapID,id,type,pointsID) VALUES(%d,%d,%d,%d)",
			mapID,model.id,model.type,id);
		res=mysql_query(_conn,sql);
		if(res!=0){
			isAllRight=false;
			break;
		}
	}
	if(isAllRight){
		mysql_commit(_conn);  //整体提交
	}else{
		mysql_rollback(_conn); //回滚
	}
	mysql_autocommit(_conn,1);
	return true;
}

//根据地图ID删除所有信息
bool WinDrawRecordDAO::deleteAllByMapID(unsigned int mapID){
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
	sprintf(sql,"SELECT pointsID FROM tb_win_drawrecord WHERE mapID=%d",mapID);
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //
		sscanf(row[0],"%d",&id);
		vPointsIDs.push_back(id);
	}
	mysql_free_result(res_set);

	//Step 2 -----------删除从表数据--------------
	//if(!winpoints->deleteNPointsByIDs(vPointsIDs))
	//	return false;
	winpoints->deleteNPointsByIDs(vPointsIDs);

	//Step 3 -----------根据主键删除数据--------------
	memset(sql,0,sizeof(sql));
	sprintf(sql,"DELETE FROM tb_win_drawrecord WHERE mapID=%d",mapID);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}

//根据mapID获取该地图的所有绘画记录
void WinDrawRecordDAO::getEntitiesByMapID(unsigned int mapID,vector<DRAW_RECORD> &models){
	models.clear();
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	int pointsID=-1;
	DRAW_RECORD tRecord;
	auto winpoints=WinPointsDAO::getInstance();

	winpoints->init(_conn);
	memset(sql,0,sizeof(sql));

	//Step 1 -----------------获取该地图的所有路口信息-----------
	sprintf(sql,"SELECT * FROM tb_win_drawrecord WHERE mapID=%d",mapID);
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //
		sscanf(row[2],"%d",&tRecord.id);
		sscanf(row[3],"%d",&tRecord.type);
		sscanf(row[4],"%d",&pointsID);
		winpoints->getPointsByID(pointsID,tRecord.drawPoints);
		models.push_back(tRecord);
	}
	mysql_free_result(res_set);
}