#ifndef MSQL_MAP_H
#define MSQL_MAP_H

#include "MySqlCommand.h"
#include "MysqlConnector.h"
#include "MAP_BASIC_data.h"
#include "MapDrawControl.h"


//��ѯ��䳤�� �����ݳ��ȶ���
#define  SQL_S_LEN 512  //�̾�  
#define  SQL_M_LEN 1024  //�еȳ���
#define  SQL_L_LEN 102400 //�������

//�ַ������鳤��
#define STRING_S_LEN 255  //�� 255B
#define STRING_M_LEN 1024 //�еȳ��� 1KB
#define STRING_L_LEN 102400 //��󳤶� 10KB

//���ݿ�״̬
typedef struct DB_STATE{
	int mapid;  //��ͼID
	char smallImagePath[STRING_S_LEN];
}DB_STATE;

/////////// ���´���Ϊmodel�� 

//��Ӧ tb_win_calibration ��
typedef struct MODEL_WINCALIBRATION{
	int mapID;                //map ID
	double scaleX;           //������
	double scaleY;            
	COMPUTE_GPS buildGPS[3];//�궨��
}MODEL_WINCALIBRATION;

//���� tb_win_mapnames ��
typedef struct MODEL_WINMAPNAME{
	int ID;
	char name[STRING_S_LEN];
	char imagepath[STRING_S_LEN];
	char smallImagepath[STRING_S_LEN];
}MODEL_WINMAPNAME;

//��Ӧ�� tb_win_road ��
typedef struct MODEL_WINROAD{
	CREATE_MAP_ROAD mapRoad;
}MODEL_WINROAD;

//��Ӧ�� tb_win_node ��
typedef struct MODEL_WINNODE{
	CREATE_MAP_NODE mapNode;
}MODEL_WINNODE;


#endif