#include "stdafx.h"
#include "WinCalibration.h"




WinCailbrationDAO*  WinCailbrationDAO::getInstance(){
	if(!s_Wincailbrationdao){
		s_Wincailbrationdao=new (std::nothrow)WinCailbrationDAO(); 
		if(!s_Wincailbrationdao)
			return nullptr;
	}
	return s_Wincailbrationdao;
}

bool WinCailbrationDAO::init(MYSQL* con){
	_conn=con;
	return true;
}

void WinCailbrationDAO::release(){
	if(s_Wincailbrationdao)
		delete s_Wincailbrationdao;
}

int WinCailbrationDAO::insertBuildGPS(COMPUTE_GPS cgps){
	int id=-1;
	int res=-1;
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	memset(sql,0,sizeof(sql));

	//Step 1 -----------��������--------------
	sprintf(sql,"INSERT INTO tb_win_gpsbuild(lng,lat,x,y) VALUES(%lf,%lf,%d,%d)",
		cgps.lng,cgps.lat,cgps.x,cgps.y);
	res=mysql_query(_conn,sql);

	//Step 2 -----------��ȡ�ղ����������ID--------------
	if(res!=0){
		return id;
	}
	sprintf(sql,"SELECT max(id) FROM tb_win_gpsbuild");
	mysql_query(_conn,sql);
	res_set = mysql_store_result(_conn);
	while ((row = mysql_fetch_row(res_set)) != NULL){ //�˴�Ӧ��ֻ��һ��ѭ��
		sscanf(row[0],"%d",&id);
	}
	mysql_free_result(res_set);
	return id;
}

bool WinCailbrationDAO::insertEntity(MODEL_WINCALIBRATION calibationEntity){
	int res=-1,i;
	char sql[SQL_S_LEN];
	int IDs[3];

	memset(sql,0,sizeof(sql));

	//Step 1 -----------����ӱ�����--------------
	for(i=0;i<3;i++){
		IDs[i]=insertBuildGPS(calibationEntity.buildGPS[i]);
	}

	sprintf(sql,"INSERT INTO tb_win_calibration(mapID,scaleX,scaleY,buildGPSID1,buildGPSID2,buildGPSID3) VALUES(%d,%e,%e,%d,%d,%d)",
		calibationEntity.mapID,
		calibationEntity.scaleX,
		calibationEntity.scaleY,
		IDs[0],IDs[1],IDs[2]
	);
	res=mysql_query(_conn, sql);

	//Step 2 -----------����calibration����-------------
	if(res!=0){
		return false;
	}
	return true;
}

 COMPUTE_GPS WinCailbrationDAO::getBuildGPSByID(int id){
	 COMPUTE_GPS cgps;
	 MYSQL_RES *res_set;
	 MYSQL_ROW row;
	 int i,buildGPSID=-1;
	 char sql[SQL_S_LEN];

	 
	 memset(sql,0,sizeof(sql));
	 sprintf(sql,"SELECT * FROM tb_win_gpsbuild WHERE id=%d",
		 id);

	 //Step 1 -----------��ѯ--------------
	 mysql_query(_conn, sql);

	 //Step 2 -----------��ȡ����--------------
	 res_set = mysql_store_result(_conn);    //���ɽ����
	 if(res_set == NULL){
		 TRACE(mysql_error(_conn));
		 return cgps;
	 }

	 //Step 3 -----------��������--------------
	 if(mysql_num_rows(res_set)!=1){
		 TRACE("�����������������ȷ��Ӧ��Ϊ1");
	 }
	 while ((row = mysql_fetch_row(res_set)) != NULL){ //�˴�Ӧ��ֻ��һ��ѭ��
		 sscanf(row[1],"%lf",&cgps.lng);
		 sscanf(row[2],"%lf",&cgps.lat);
		 sscanf(row[3],"%d",&cgps.x);
		 sscanf(row[4],"%d",&cgps.y);
	 }
	 mysql_free_result(res_set);
	 return cgps;
 }

MODEL_WINCALIBRATION WinCailbrationDAO::getEntityByMapID(int id){
	MODEL_WINCALIBRATION calibration;
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	int i,buildGPSID[3];
	char sql[SQL_S_LEN];

	calibration.mapID=-1;
	memset(sql,0,sizeof(sql));
	sprintf(sql,"SELECT * FROM tb_win_calibration WHERE mapID=%d",
		id);

	//Step 1 -----------��ѯ--------------
	mysql_query(_conn, sql);

	//Step 2 -----------��ȡ����--------------
	res_set = mysql_store_result(_conn);    //���ɽ����
	if(res_set == NULL){
		TRACE(mysql_error(_conn));
		return calibration;
	}

	//Step 3 -----------��������--------------
	if(mysql_num_rows(res_set)!=1){
		 TRACE("�����������������ȷ��Ӧ��Ϊ1");
	}
	while ((row = mysql_fetch_row(res_set)) != NULL){ //�˴�Ӧ��ֻ��һ��ѭ��
//		for(i=0;i<mysql_num_fields(res_set);i++){
		sscanf(row[0],"%d",&calibration.mapID);
		sscanf(row[1],"%lf",&calibration.scaleX);
		sscanf(row[2],"%lf",&calibration.scaleY);
		sscanf(row[3],"%d",&buildGPSID[0]);  //�궨λ�õ�ID
		sscanf(row[4],"%d",&buildGPSID[1]);
		sscanf(row[5],"%d",&buildGPSID[2]);
	}
	mysql_free_result(res_set);

	//Step 4 -----------��ѯ�궨�����ӱ�--------------
	for(i=0;i<3;i++){
		calibration.buildGPS[i]=getBuildGPSByID(buildGPSID[i]);
	}
	return calibration;
}


bool WinCailbrationDAO::deleteEntityByKey(unsigned int mapID){
	int res=-1,i;
	char sql[SQL_S_LEN];
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	int ids[3];
	memset(sql,0,sizeof(sql));

	//Step 1 -----------����������ѯ����BuildGPS--------------
	sprintf(sql,"SELECT * FROM tb_win_calibration WHERE mapID=%d",mapID);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}
	res_set = mysql_store_result(_conn);    //���ɽ����
	while ((row = mysql_fetch_row(res_set)) != NULL){ //�˴�Ӧ��ֻ��һ��ѭ��
		sscanf(row[3],"%d",&ids[0]);
		sscanf(row[4],"%d",&ids[1]);
		sscanf(row[5],"%d",&ids[2]);
	}
	mysql_free_result(res_set);

	//Step 2 -----------ɾ��BuildGPS--------------
	mysql_autocommit(_conn,0);
	for( i=0;i<3;i++){
		memset(sql,0,sizeof(sql));
		sprintf(sql,"DELETE FROM tb_win_gpsbuild WHERE id=%d",ids[i]);
		mysql_query(_conn,sql);
	}
	mysql_commit(_conn);
	mysql_autocommit(_conn,1);

	//Step 3 -----------ɾ������--------------
	memset(sql,0,sizeof(sql));
	sprintf(sql,"DELETE FROM tb_win_calibration WHERE mapID=%d",mapID);
	res=mysql_query(_conn,sql);
	if(res!=0){
		return false;
	}

	return true;
}
