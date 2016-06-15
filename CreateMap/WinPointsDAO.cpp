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

	//Step 1 -----------获取最大ID,程序中维护ID自增操作--------------
	sprintf(sql,"SELECT max(ID) FROM tb_win_points");
	res=mysql_query(_conn,sql);
	if(res!=0){
		return id;
	}
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //此处应该只有一次循环
		sscanf(row[0],"%d",&id);
	}
	mysql_free_result(res_set);
	id++;

	//Step 2 -----------插入点对序列--------------
	mysql_autocommit(_conn, 0);   //为了提高效率设置为手动提交
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

	//Step 3 -----------检查数据是否都成功插入--------------
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

//根据ID删除数据 
bool WinPointsDAO::deletePintsByID(int ID){
	int res=-1;
	char sql[SQL_S_LEN];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------根据主键删除数据--------------
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
	
	//Step 1 -----------批量删除数据--------------
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

	//Step 2 -----------判断回滚或者提交--------------
	if(isAllRight){
		mysql_commit(_conn); //提交
	}else{
		mysql_rollback(_conn);//回滚
		return false;
	}
	return true;
}

//根据ID获取点集
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

	//Step 1 -----------获取最大ID,程序中维护ID自增操作--------------
	sprintf(sql,"SELECT * FROM tb_win_points WHERE ID=%d ORDER BY autoID",ID);
	res=mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //此处应该只有一次循环
		sscanf(row[2],"%d",&point.x);
		sscanf(row[3],"%d",&point.y);
		vPoints.push_back(point);
	}
	mysql_free_result(res_set);
}