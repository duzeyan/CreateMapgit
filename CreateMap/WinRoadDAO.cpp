#include "stdafx.h"
#include "WinRoadDAO.h"
#include"WinPointsDAO.h"

WinRoadDAO*  WinRoadDAO::getInstance(){
	if(!s_winRoadDAO){
		s_winRoadDAO=new (std::nothrow)WinRoadDAO(); 
	}
	return s_winRoadDAO;
}

bool WinRoadDAO::init(MYSQL* con){
	_conn=con;
	return true;
}

void WinRoadDAO::getInsertSql(char *sql,const MODEL_WINROAD &model,unsigned int mapID,unsigned int pid){
	
	string strSql="INSERT INTO tb_win_road";
	strSql=strSql+"(mapID,"
		+"x,"
		+"y,"
		+"pInLineID,"
		+"idself,"
		+"idstart,"
		+"idend,"
		+"k,"
		+"b,"
		+"c,"
		+"roadkind,"
		+"width,"
		+"length,"
		+"maluyazi,"
		+"hyazi,"
		+"hulan,"
		+"hhunlan,"
		+"xingdaoxiannum,"
		+"leftxingdaoxian,"
		+"middlexingdaoxian,"
		+"rightxingdaoxian,"
		+"chedaonum,"
		+"leftdaolubianjie,"
		+"rightdaolubianjie,"
		+"idealspeed,"
		+"structure,"
		+"GPSDataFrom) VALUES("
		+"%d,%d,%d,%d,%d,%d,%d," //7
		+"%lf,%lf,%lf,%d,%lf,%lf,%d," //7
		+"%lf,%d,%lf,%d,%d,%d,%d," //7
		+"%d,%d,%d,%d," //4
		+"%d,%d)"; //参数

	sprintf(sql,strSql.c_str(),
		mapID,
		model.mapRoad.position.x,
		model.mapRoad.position.y,
		pid,
		model.mapRoad.road.idself,
		model.mapRoad.road.idstart,
		model.mapRoad.road.idend,
		model.mapRoad.road.k,
		model.mapRoad.road.b,
		model.mapRoad.road.c,
		model.mapRoad.road.roadkind,
		model.mapRoad.road.wedth,
		model.mapRoad.road.length,
		model.mapRoad.road.maluyazi,
		model.mapRoad.road.hyazi,
		model.mapRoad.road.hulan,
		model.mapRoad.road.hhulan,
		model.mapRoad.road.xingdaoxiannum,
		model.mapRoad.road.leftxingdaoxian,
		model.mapRoad.road.middlexingdaoxian,
		model.mapRoad.road.rightxingdaoxian,
		model.mapRoad.road.chedaonum,
		model.mapRoad.road.leftdaolubianjie,
		model.mapRoad.road.rightdaolubianjie,
		model.mapRoad.road.idealspeed,
		model.mapRoad.road.structure,
		model.mapRoad.road.GPSDataFrom);
}

bool WinRoadDAO::insertEntity(const MODEL_WINROAD &model,unsigned int mapID){
	int res=-1;
	char sql[SQL_M_LEN*5];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------插入点对序列--------------
	auto winpoints=WinPointsDAO::getInstance();
	winpoints->init(_conn);
	unsigned int id=winpoints->insertEntity(model.mapRoad.pInLine);
	if(id==-1)
		return false;

	//Step 2 -----------插入其他属性--------------
	memset(sql,0,sizeof(sql));
	getInsertSql(sql,model,mapID,id);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}

bool WinRoadDAO::deleteEntityByKey(unsigned int mapID,int idself){
	int res=-1;
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	int id;
	auto winpoints=WinPointsDAO::getInstance();

	winpoints->init(_conn);
	memset(sql,0,sizeof(sql));

	//Step 1 -----------------查询所有从表点集ID-----------
	sprintf(sql,"SELECT pInLineID FROM tb_win_road WHERE mapID=%d AND idself=%d",mapID,idself);
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //
		sscanf(row[0],"%d",&id);
	}
	mysql_free_result(res_set);

	//Step 2 -----------删除从表数据--------------
	if(!winpoints->deletePintsByID(id))
		return false;
	
	//Step 2 -----------根据主键删除数据--------------
	memset(sql,0,sizeof(sql));
	sprintf(sql,"DELETE FROM tb_win_road WHERE mapID=%d AND idself=%d ",mapID,idself);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}

//根据地图ID删除所有信息
bool WinRoadDAO::deleteAllByMapID(unsigned int mapID){
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
	sprintf(sql,"SELECT pInLineID FROM tb_win_road WHERE mapID=%d",mapID);
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
	sprintf(sql,"DELETE FROM tb_win_road WHERE mapID=%d",mapID);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}

//根据mapID获取该地图的所有节点信息
void WinRoadDAO::getEntitiesByMapID(unsigned int mapID,vector<CREATE_MAP_ROAD> &models){
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	int pointsID;
	CREATE_MAP_ROAD tRoad;
	auto winpoints=WinPointsDAO::getInstance();
	winpoints->init(_conn);

	memset(sql,0,sizeof(sql));

	//Step 1 -----------------获取该地图的所有路口信息-----------
	sprintf(sql,"SELECT * FROM tb_win_road WHERE mapID=%d",mapID);
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //
		sscanf(row[3],"%d",&pointsID);
		winpoints->getPointsByID(pointsID,tRoad.pInLine);
		row2Entity(row,tRoad);
		models.push_back(tRoad);
	}
	mysql_free_result(res_set);
}

//查询的数据项转化为结构体中数据
void WinRoadDAO::row2Entity(const MYSQL_ROW &row,CREATE_MAP_ROAD &model){
	sscanf(row[1],"%d",&model.position.x);
	sscanf(row[2],"%d",&model.position.y);
	//3 点集
	sscanf(row[4],"%d",&model.road.idself);
	sscanf(row[5],"%d",&model.road.idstart);
	sscanf(row[6],"%d",&model.road.idend);
	sscanf(row[7],"%lf",&model.road.k);
	sscanf(row[8],"%lf",&model.road.b);
	sscanf(row[9],"%lf",&model.road.c);
	sscanf(row[10],"%d",&model.road.roadkind);
	sscanf(row[11],"%lf",&model.road.wedth);
	sscanf(row[12],"%lf",&model.road.length);
	sscanf(row[13],"%d",&model.road.maluyazi);
	sscanf(row[14],"%lf",&model.road.hyazi);
	sscanf(row[15],"%d",&model.road.hulan);
	sscanf(row[16],"%lf",&model.road.hhulan);
	sscanf(row[17],"%d",&model.road.xingdaoxiannum);
	sscanf(row[18],"%d",&model.road.leftxingdaoxian);
	sscanf(row[19],"%d",&model.road.middlexingdaoxian);
	sscanf(row[20],"%d",&model.road.rightxingdaoxian);
	sscanf(row[21],"%d",&model.road.chedaonum);
	sscanf(row[22],"%d",&model.road.leftdaolubianjie);
	sscanf(row[23],"%d",&model.road.rightdaolubianjie);
	sscanf(row[24],"%d",&model.road.idealspeed);
	sscanf(row[25],"%d",&model.road.structure);
	sscanf(row[26],"%d",&model.road.GPSDataFrom);
}