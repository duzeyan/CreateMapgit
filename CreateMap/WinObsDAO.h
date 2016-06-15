#ifndef WINOBSDAO_H
#define WINOBSDAO_H

#include "stdafx.h"
#include "map_mysql_data.h"

// �� tb_win_obs �����
class WinObsDAO{
public:
	static WinObsDAO* getInstance();
	bool init(MYSQL* );

private:
	MYSQL* _conn;

public :
	//����mapID��ȡ�õ�ͼ�����нڵ���Ϣ
	void getEntitiesByMapID(unsigned int mapID,vector<CREATE_MAP_OBSTACLE> &models);

	//��������
	bool  insertEntity(const CREATE_MAP_OBSTACLE &model,unsigned int mapID);

	//���ݹؼ���ɾ������
	bool deleteEntityByKey(unsigned int mapID,int id);

	//���ݵ�ͼIDɾ��������Ϣ
	bool deleteAllByMapID(unsigned int mapID);
};

static WinObsDAO* s_winobsdao;
#endif