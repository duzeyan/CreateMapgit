#ifndef WINRECORD_H
#define WINRECORD_H

#include "map_mysql_data.h"

//�� tb_win_drawrecord ����
class WinDrawRecordDAO{
public:
	static WinDrawRecordDAO* getInstance();
	bool init(MYSQL* );

private:
	MYSQL* _conn;

public :
	//����mapID��ȡ�õ�ͼ�����л滭��¼
	void getEntitiesByMapID(unsigned int mapID,vector<DRAW_RECORD> &models);

	//����һ������
	bool insertEntity(const  DRAW_RECORD &model,unsigned int mapID);

	//������������
	bool insertEntities(const vector<DRAW_RECORD> &vModels,unsigned int mapID);

	//��������ɾ������
	//bool deleteEntityByKey(unsigned int mapID,int idstart,int type);

	//���ݵ�ͼIDɾ��������Ϣ
	bool deleteAllByMapID(unsigned int mapID);


private: 

};

static WinDrawRecordDAO* s_WinDrawRecord=nullptr; 

#endif 