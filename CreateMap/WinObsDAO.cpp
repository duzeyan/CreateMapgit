#include "stdafx.h"
#include "WinObsDAO.h"


WinObsDAO*  WinObsDAO::getInstance(){
	if(!s_winobsdao){
		s_winobsdao=new (std::nothrow)WinObsDAO(); 
	}
	return s_winobsdao;
}

bool WinObsDAO::init(MYSQL* con){
	_conn=con;
	return true;
}

bool WinObsDAO::insertEntity(const CREATE_MAP_OBSTACLE &model,unsigned int mapID){
	int res=-1;
	char sql[SQL_S_LEN];
	memset(sql,0,sizeof(sql));


	//Step 1 -----------ƴ��sql���--------------
	sprintf(sql,"INSERT INTO tb_win_obs(mapID,id,x,y,rPix) VALUES(%d,%d,%d,%d,%lf) ",
		     mapID,model.id,model.x,model.y,model.rPix);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}

bool WinObsDAO::deleteEntityByKey(unsigned int mapID,int id){
	int res=-1;
	char sql[SQL_S_LEN];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------��������ɾ������--------------
	sprintf(sql,"DELETE FROM tb_win_obs WHERE mapID=%d AND id=%d ",mapID,id);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}

//���ݵ�ͼIDɾ��������Ϣ
bool WinObsDAO::deleteAllByMapID(unsigned int mapID){
	int res=-1;
	char sql[SQL_S_LEN];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------��������ɾ������--------------
	sprintf(sql,"DELETE FROM tb_win_obs WHERE mapID=%d",mapID);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}


//����mapID��ȡ�õ�ͼ�����нڵ���Ϣ
void WinObsDAO::getEntitiesByMapID(unsigned int mapID,vector<CREATE_MAP_OBSTACLE> &models){
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	CREATE_MAP_OBSTACLE tObs;

	memset(sql,0,sizeof(sql));

	//Step 1 -----------------��ȡ�õ�ͼ�������ϰ�����Ϣ-----------
	sprintf(sql,"SELECT * FROM tb_win_obs WHERE mapID=%d",mapID);
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //
		sscanf(row[1],"%d",&tObs.id);
		sscanf(row[2],"%d",&tObs.x);
		sscanf(row[3],"%d",&tObs.y);
		sscanf(row[4],"%lf",&tObs.rPix);
		models.push_back(tObs);
	}
	mysql_free_result(res_set);
}

