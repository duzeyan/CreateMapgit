#include"stdafx.h"
#include "WinMapNameDAO.h"


#define  IMAGE_PATH_LEN 255
WinMapNameDAO* WinMapNameDAO::getInstance(){
	if(!s_winmapnamedao){
		s_winmapnamedao=new (std::nothrow)WinMapNameDAO(); 
		if(!s_winmapnamedao)
			return nullptr;
	}
	return s_winmapnamedao;
}

bool WinMapNameDAO::init(MYSQL *con){
	_conn=con;
	return true;
}

void WinMapNameDAO::getAllEntities(vector<MODEL_WINMAPNAME> &vModels){
	MODEL_WINMAPNAME winmapname;
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	int i;
	char sql[SQL_S_LEN];


	//Step 1 -----------查询--------------
	memset(sql,0,sizeof(sql));
	sprintf(sql,"SELECT * FROM tb_win_mapname ");
	mysql_query(_conn, sql);

	//Step 2 -----------获取数据--------------
	res_set = mysql_store_result(_conn);    //生成结果集
	if(res_set == NULL){
		TRACE(mysql_error(_conn));
		return ;
	}

	//Step 3 -----------解析数据--------------
	while ((row = mysql_fetch_row(res_set)) != NULL){ //遍历结果集
		sscanf(row[0],"%d",&winmapname.ID);
		strcpy(winmapname.name,row[1]);
		strcpy(winmapname.imagepath,row[2]);
		strcpy(winmapname.smallImagepath,row[3]);
		vModels.push_back(winmapname);
	}
	mysql_free_result(res_set);
	return;
}

void WinMapNameDAO::getEntityByID(unsigned int mapID,MODEL_WINMAPNAME &model){
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	char sql[SQL_S_LEN];
	char imagePath[IMAGE_PATH_LEN];
	memset(imagePath,0,sizeof(imagePath));

	//Step 1 -----------查询--------------
	memset(sql,0,sizeof(sql));
	sprintf(sql,"SELECT * FROM tb_win_mapname where ID=%d ",mapID);
	mysql_query(_conn, sql);

	//Step 2 -----------获取数据--------------
	res_set = mysql_store_result(_conn);    //生成结果集
	if(res_set == NULL){
		TRACE(mysql_error(_conn));
		return ;
	}

	//Step 3 -----------解析数据--------------
	while ((row = mysql_fetch_row(res_set)) != NULL){ //遍历结果集
		strcpy(model.name,row[1]);
		strcpy(model.imagepath,row[2]);
		strcpy(model.smallImagepath,row[3]);
	}
	mysql_free_result(res_set);
	return ;
}

bool WinMapNameDAO::insertEntity(MODEL_WINMAPNAME &model){
	int id=-1;
	int res=-1;
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	memset(sql,0,sizeof(sql));

	//Step 1 -----------插入数据--------------
	sprintf(sql,"INSERT INTO tb_win_mapname(name,imagepath) VALUES('%s','%s')",
		model.name,model.imagepath);
	res=mysql_query(_conn,sql);

	//Step 2 -----------获取刚插入数据项的ID--------------
	if(res!=0){
		TRACE(mysql_error(_conn));
		return false;
	}
	sprintf(sql,"SELECT max(ID) FROM tb_win_mapname");
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //此处应该只有一次循环
		sscanf(row[0],"%d",&id);
	}
	model.ID=id;

	mysql_free_result(res_set);
	return true;
}

//根据地图ID删除所有信息
bool WinMapNameDAO::deleteEntityByMapID(unsigned int mapID){
	int res=-1;
	char sql[SQL_S_LEN];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------根据主键删除数据--------------
	sprintf(sql,"DELETE FROM tb_win_mapname WHERE mapID=%d",mapID);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}