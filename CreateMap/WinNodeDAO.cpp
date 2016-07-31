#include "stdafx.h"
#include "WinNodeDAO.h"
#include"WinPointsDAO.h"

WinNodeDAO*  WinNodeDAO::getInstance(){
	if(!s_winNodeDAO){
		s_winNodeDAO=new (std::nothrow)WinNodeDAO(); 
	}
	return s_winNodeDAO;
}

bool WinNodeDAO::init(MYSQL* con){
	_conn=con;
	return true;
}

void WinNodeDAO::getInsertSql(char *sql,const MODEL_WINNODE &model,unsigned int mapID){
	string strSql="INSERT INTO tb_win_node";
	strSql=strSql+"(mapID,"
		+"x,"
		+"y,"
		+"idself,"
		+"neigh,"
		+"neighNodeID1,"
		+"neighNodeID2,"
		+"neighNodeID3,"
		+"neighNodeID4,"
		+"neighLineID1,"
		+"neighLineID2,"
		+"neighLineID3,"
		+"neighLineID4,"
		+"HLD,"
		+"HLDkind,"
		+"lukou,"
		+"zebra,"
		+"gpsx,"
		+"gpsy,"
		+"earthx,"
		+"earthy) VALUES("
		+"%d,%d,%d,%d,%d," //5
		+"%d,%d,%d,%d,%d,%d,%d,%d," //8
		+"%d,%d,%d,%d,%lf,%lf,%d,%d)"; //8

	sprintf(sql,strSql.c_str(),
		mapID,
		model.mapNode.position.x,
		model.mapNode.position.y,
		model.mapNode.node.idself,
		model.mapNode.node.neigh,
		model.mapNode.node.NeighNoteID[0],
		model.mapNode.node.NeighNoteID[1],
		model.mapNode.node.NeighNoteID[2],
		model.mapNode.node.NeighNoteID[3],
		model.mapNode.node.NeighLineID[0],
		model.mapNode.node.NeighLineID[1],
		model.mapNode.node.NeighLineID[2],
		model.mapNode.node.NeighLineID[3],
		model.mapNode.node.HLD,
		model.mapNode.node.HLDkind,
		model.mapNode.node.lukou,
		model.mapNode.node.zebra,
		model.mapNode.node.gpsx,
		model.mapNode.node.gpsy,
		model.mapNode.node.earthx,
		model.mapNode.node.earthy);
}

bool WinNodeDAO::insertEntity(const MODEL_WINNODE &model,unsigned int mapID){
	int res=-1;
	char sql[SQL_M_LEN*5];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------插入节点属性--------------
	getInsertSql(sql,model,mapID);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}

bool WinNodeDAO::deleteEntityByKey(unsigned int mapID,int idself){
	int res=-1;
	char sql[SQL_S_LEN];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------根据主键删除数据--------------
	sprintf(sql,"DELETE FROM tb_win_node WHERE mapID=%d AND idself=%d ",mapID,idself);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}


//根据地图ID删除所有信息
bool WinNodeDAO::deleteAllByMapID(unsigned int mapID){
	int res=-1;
	char sql[SQL_S_LEN];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------根据主键删除数据--------------
	sprintf(sql,"DELETE FROM tb_win_node WHERE mapID=%d",mapID);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}

void WinNodeDAO::getEntitiesByMapID(unsigned int mapID,vector<CREATE_MAP_NODE> &models){
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	CREATE_MAP_NODE tNode;
	auto winpoints=WinPointsDAO::getInstance();

	winpoints->init(_conn);
	memset(sql,0,sizeof(sql));

	//Step 1 -----------------获取该地图的所有路口信息-----------
	sprintf(sql,"SELECT * FROM tb_win_node WHERE mapID=%d",mapID);
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //
		row2Entity(row,tNode);
		models.push_back(tNode);
	}
	mysql_free_result(res_set);
}

void WinNodeDAO::row2Entity(const MYSQL_ROW &row,CREATE_MAP_NODE &model){
	sscanf(row[1],"%d",&model.position.x);
	sscanf(row[2],"%d",&model.position.y);
	sscanf(row[3],"%d",&model.node.idself);
	sscanf(row[4],"%d",&model.node.neigh);
	sscanf(row[5],"%d",&model.node.NeighNoteID[0]);
	sscanf(row[6],"%d",&model.node.NeighNoteID[1]);
	sscanf(row[7],"%d",&model.node.NeighNoteID[2]);
	sscanf(row[8],"%d",&model.node.NeighNoteID[3]);
	sscanf(row[9],"%d",&model.node.NeighLineID[0]);
	sscanf(row[10],"%d",&model.node.NeighLineID[1]);
	sscanf(row[11],"%d",&model.node.NeighLineID[2]);
	sscanf(row[12],"%d",&model.node.NeighLineID[3]);
	sscanf(row[13],"%d",&model.node.HLD);
	sscanf(row[14],"%d",&model.node.HLDkind);
	sscanf(row[15],"%d",&model.node.lukou);
	sscanf(row[16],"%d",&model.node.zebra);
	sscanf(row[17],"%lf",&model.node.gpsx);
	sscanf(row[18],"%lf",&model.node.gpsy);
	sscanf(row[19],"%d",&model.node.earthx);
	sscanf(row[20],"%d",&model.node.earthy);
}

