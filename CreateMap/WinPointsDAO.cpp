#include "stdafx.h"
#include "WinPointsDAO.h"




WinPointsDAO*  WinPointsDAO::getInstance(){
	if(!s_winpointsdao){
		s_winpointsdao=new (std::nothrow)WinPointsDAO(); 
	}
	return s_winpointsdao;
}

bool WinPointsDAO::init(MYSQL* con){
	_conn=con;
	return true;
}

unsigned int WinPointsDAO::insertEntity(const vector<CPoint> &models){
	int id=-1;
	int res=-1;
	bool isAllRight=true;
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	memset(sql,0,sizeof(sql));

	//Step 1 -----------��ȡ���ID,������ά��ID��������--------------
	sprintf(sql,"SELECT max(ID) FROM tb_win_points");
	res=mysql_query(_conn,sql);
	if(res!=0){
		return id;
	}
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //�˴�Ӧ��ֻ��һ��ѭ��
		sscanf(row[0],"%d",&id);
	}
	mysql_free_result(res_set);
	id++;

	//Step 2 -----------����������--------------
	mysql_autocommit(_conn, 0);   //Ϊ�����Ч������Ϊ�ֶ��ύ
	for(auto &point:models){
		memset(sql,0,sizeof(sql));
		sprintf(sql,"INSERT INTO tb_win_points(ID,x,y) VALUES(%d,%d,%d)",
			id,point.x,point.y);
		res=mysql_query(_conn,sql);
		if(res!=0){
			isAllRight=false;
			break;
		}
	}

	//Step 3 -----------��������Ƿ񶼳ɹ�����--------------
	if (isAllRight)
	{
		mysql_commit(_conn); 
	}else{
		mysql_rollback(_conn);
		id=-1;
	}
	mysql_autocommit(_conn, 1); 

	return id;
}

//����IDɾ������ 
bool WinPointsDAO::deletePintsByID(int ID){
	int res=-1;
	char sql[SQL_S_LEN];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------��������ɾ������--------------
	sprintf(sql,"DELETE FROM tb_win_points WHERE ID=%d",ID);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	return true;
}
bool WinPointsDAO::deleteNPointsByIDs(const vector<int> &vIDs){
	int res=-1;
	bool isAllRight=true;
	char sql[SQL_S_LEN];
	
	//Step 1 -----------����ɾ������--------------
	mysql_autocommit(_conn,0);
	for(auto &ID:vIDs){
		memset(sql,0,sizeof(sql));
		sprintf(sql,"DELETE FROM tb_win_points WHERE ID=%d",ID);
		res=mysql_query(_conn,sql);
		if(res!=0){
			isAllRight=false;
			break;
		}
	}

	//Step 2 -----------�жϻع������ύ--------------
	if(isAllRight){
		mysql_commit(_conn); //�ύ
	}else{
		mysql_rollback(_conn);//�ع�
		return false;
	}
	return true;
}

//����ID��ȡ�㼯
void WinPointsDAO::getPointsByID(int ID,vector<CPoint> &vPoints){
	int id=-1;
	int res=-1;
	bool isAllRight=true;
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	CPoint point;

	vPoints.clear();
	memset(sql,0,sizeof(sql));

	//Step 1 -----------��ȡ���ID,������ά��ID��������--------------
	sprintf(sql,"SELECT * FROM tb_win_points WHERE ID=%d ORDER BY autoID",ID);
	res=mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //�˴�Ӧ��ֻ��һ��ѭ��
		sscanf(row[2],"%d",&point.x);
		sscanf(row[3],"%d",&point.y);
		vPoints.push_back(point);
	}
	mysql_free_result(res_set);
}