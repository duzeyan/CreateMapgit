#ifndef WINCROSSDAO_H
#define WINCROSSDAO_H

#include "map_mysql_data.h"

//�� tb_win_cross ����
class WinCrossDAO{
public:
	static WinCrossDAO* getInstance();
	bool init(MYSQL* );

private:
	MYSQL* _conn;

public :
	bool insertEntity(const  CREATE_MAP_CROSS &model,unsigned int mapID);

	void getEntitiesByMapID(unsigned int mapID,vector<CREATE_MAP_CROSS> &model);

	//��������ɾ������
	bool deleteEntityByKey(unsigned int mapID,int idstart,int idend);

	//ɾ���õ�ͼ������·����Ϣ
	bool deleteEntitiesByMapID(unsigned int mapID);
private: 
};

static WinCrossDAO* s_winCrossDAO=nullptr; 

#endif 