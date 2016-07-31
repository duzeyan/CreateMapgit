#ifndef WINROADDAO_H
#define WINROADDAO_H

#include "map_mysql_data.h"

//�� tb_win_road ����
class WinRoadDAO{
public:
	static WinRoadDAO* getInstance();
	bool init(MYSQL* );

private:
	MYSQL* _conn;

public :
	bool insertEntity(const MODEL_WINROAD &model,unsigned int mapID);

	bool deleteEntityByKey(unsigned int mapID,int idself);

	//���ݵ�ͼIDɾ��������Ϣ
	bool deleteAllByMapID(unsigned int mapID);

	//����mapID��ȡ�õ�ͼ�����нڵ���Ϣ
	void getEntitiesByMapID(unsigned int mapID,vector<CREATE_MAP_ROAD> &models);
private: 
	//************************************
	// ������  ƴ�Ӳ��빦�ܵ�sql���
	// ����ֵ: void
	// ����:   char * sql [out]ƴ�ӽ�� ������ǰ��Ҫ��ʼ����0
	// ����:   const MODEL_WINROAD & model ��·����
	// ����:   unsigned int mapID  ��ͼID
	// ����:   unsigned int pid    �㼯ID 
	// ����:   unsigned int GPSid  ����GPS��ID 
	// ����:   unsigned int GPSidNeg  ���򳵲�GPS��ID 
	//************************************
	void getInsertSql(char *sql,const MODEL_WINROAD &model,unsigned int mapID,unsigned int pid,int GPSid,int GPSidNeg);

	//��ѯ��������ת��Ϊ�ṹ��������
	void row2Entity(const MYSQL_ROW &row,CREATE_MAP_ROAD &model);
};

static WinRoadDAO* s_winRoadDAO=nullptr; 

#endif 