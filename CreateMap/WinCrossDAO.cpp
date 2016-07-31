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

	//Step 1 -----------插入绘图点对序列--------------
	auto winpoints=WinPointsDAO::getInstance();
	winpoints->init(_conn);
	unsigned int idp=winpoints->insertEntity(model.points);
	if(idp==-1)
		return false;

	//Step 2 -----------插入GPS序列--------------
	int idGPS,idGPSNeg;
	idGPSNeg=idGPS=-1;
	if(model.GPSDataFrom==NJUST_MAP_GPS_FROM_CAR){//车采
		if(!model.realGPS.empty())
			idGPS=winpoints->insertEntity(model.realGPS);//插入正序列车采数据
		if(!model.realGPSNeg.empty())
			idGPSNeg=winpoints->insertEntity(model.realGPSNeg);//插入负序列车采数据
	}

	//Step 3 -----------插入其他属性--------------
	sprintf(sql,"INSERT INTO tb_win_cross(mapID,idstart,idend,pointsID,GPSDataFrom,GPSListID,GPSListIDNeg) VALUES(%d,%d,%d,%d,%d,%d,%d)",
		mapID,model.idstart,model.idend,idp,model.GPSDataFrom,idGPS,idGPSNeg);
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
		sscanf(row[4],"%d",&tCross.GPSDataFrom);
		winpoints->getPointsByID(pointsID,tCross.points);
		if(tCross.GPSDataFrom==NJUST_MAP_GPS_FROM_CAR){ //车采类型,则读取数据
			sscanf(row[5],"%d",&pointsID);
			winpoints->getGPSByID(pointsID,tCross.realGPS);
			sscanf(row[6],"%d",&pointsID);
			winpoints->getGPSByID(pointsID,tCross.realGPSNeg);
		}
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
	int id;//点集ID
	int GPSid,GPSidNeg;//GPS集合ID
	NJUST_MAP_GPS_SOURCE GPSDataFrom;
	auto winpoints=WinPointsDAO::getInstance();

	winpoints->init(_conn);
	memset(sql,0,sizeof(sql));

	//Step 1 -----------------查询从表点集ID-----------
	sprintf(sql,"SELECT * FROM tb_win_cross WHERE mapID=%d AND idstart=%d AND idend=%d",mapID,idstart,idend);
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //
		sscanf(row[3],"%d",&id);
		sscanf(row[4],"%d",&GPSDataFrom);
		sscanf(row[5],"%d",&GPSid);
		sscanf(row[6],"%d",&GPSidNeg);
	}
	mysql_free_result(res_set);

	//Step 2 -----------删除从表数据--------------
	//if(!winpoints->deletePintsByID(id))
	//	return false;
	winpoints->deletePintsByID(id);//删除点集
	if(GPSid!=-1){
		winpoints->deleteGPSByID(GPSid);//删正序GPS
	}
	if(GPSidNeg>0){
		winpoints->deleteGPSByID(GPSidNeg);//删逆序GPS
	}

	//Step 3 -----------根据主键删除主表数据--------------
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
	vector<int> vGPSsIDs,vGPSsIDsNeg; //正序逆序 数据
	int id,GPSid,GPSidNeg; 
	auto winpoints=WinPointsDAO::getInstance();

	winpoints->init(_conn);
	memset(sql,0,sizeof(sql));

	//Step 1 -----------------查询所有从表点集ID-----------
	sprintf(sql,"SELECT pointsID,GPSListID,GPSListIDNeg FROM tb_win_cross WHERE mapID=%d",mapID);
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ 
		sscanf(row[0],"%d",&id);
		sscanf(row[1],"%d",&GPSid);
		sscanf(row[2],"%d",&GPSidNeg);
		vPointsIDs.push_back(id);
		vGPSsIDs.push_back(GPSid);
		vGPSsIDsNeg.push_back(GPSidNeg);
	}
	mysql_free_result(res_set);

	//Step 2 -----------删除从表数据--------------
	if(!winpoints->deleteNPointsByIDs(vPointsIDs))
		return false;
	for(auto theid : vGPSsIDs){
		if(theid!=-1){
			winpoints->deleteGPSByID(theid);
		}
	}
	//逆
	for(auto theid : vGPSsIDsNeg){
		if(theid>0){ //0 -1都为无效ID
			winpoints->deleteGPSByID(theid);
		}
	}

	//Step 3 -----------根据主键删除数据--------------
	memset(sql,0,sizeof(sql));
	sprintf(sql,"DELETE FROM tb_win_cross WHERE mapID=%d",mapID);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}

