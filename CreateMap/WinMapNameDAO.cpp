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


	//Step 1 -----------��ѯ--------------
	memset(sql,0,sizeof(sql));
	sprintf(sql,"SELECT * FROM tb_win_mapname ");
	mysql_query(_conn, sql);

	//Step 2 -----------��ȡ����--------------
	res_set = mysql_store_result(_conn);    //���ɽ����
	if(res_set == NULL){
		TRACE(mysql_error(_conn));
		return ;
	}

	//Step 3 -----------��������--------------
	while ((row = mysql_fetch_row(res_set)) != NULL){ //���������
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

	//Step 1 -----------��ѯ--------------
	memset(sql,0,sizeof(sql));
	sprintf(sql,"SELECT * FROM tb_win_mapname where ID=%d ",mapID);
	mysql_query(_conn, sql);

	//Step 2 -----------��ȡ����--------------
	res_set = mysql_store_result(_conn);    //���ɽ����
	if(res_set == NULL){
		TRACE(mysql_error(_conn));
		return ;
	}

	//Step 3 -----------��������--------------
	while ((row = mysql_fetch_row(res_set)) != NULL){ //���������
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

	//Step 1 -----------��������--------------
	sprintf(sql,"INSERT INTO tb_win_mapname(name,imagepath) VALUES('%s','%s')",
		model.name,model.imagepath);
	res=mysql_query(_conn,sql);

	//Step 2 -----------��ȡ�ղ����������ID--------------
	if(res!=0){
		TRACE(mysql_error(_conn));
		return false;
	}
	sprintf(sql,"SELECT max(ID) FROM tb_win_mapname");
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //�˴�Ӧ��ֻ��һ��ѭ��
		sscanf(row[0],"%d",&id);
	}
	model.ID=id;

	mysql_free_result(res_set);
	return true;
}

//���ݵ�ͼIDɾ��������Ϣ
bool WinMapNameDAO::deleteEntityByMapID(unsigned int mapID){
	int res=-1;
	char sql[SQL_S_LEN];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------��������ɾ������--------------
	sprintf(sql,"DELETE FROM tb_win_mapname WHERE mapID=%d",mapID);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}