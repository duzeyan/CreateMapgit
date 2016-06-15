#ifndef WINPOINTSDAO_H
#define WINPOINTSDAO_H

#include "stdafx.h"
#include "map_mysql_data.h"

// �� tb_win_points �����
class WinPointsDAO{
public:
	static WinPointsDAO* getInstance();
	bool init(MYSQL* );

private:
	MYSQL* _conn;

public :
	//��������㼯 ����ID ʧ�ܷ���-1
	unsigned int insertEntity(const vector<CPoint> &models);

	//����IDɾ������ 
	bool deletePintsByID(int ID);

	//����ɾ���㼯
	bool deleteNPointsByIDs(const vector<int> &vIDs);

	//����ID��ȡ�㼯 [out]vPoints ���ȶ�vPoints����clear()
	void getPointsByID(int ID,vector<CPoint> &vPoints);
};

static WinPointsDAO* s_winpointsdao;
#endif