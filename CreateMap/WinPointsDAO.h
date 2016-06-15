#ifndef WINPOINTSDAO_H
#define WINPOINTSDAO_H

#include "stdafx.h"
#include "map_mysql_data.h"

// 对 tb_win_points 表操作
class WinPointsDAO{
public:
	static WinPointsDAO* getInstance();
	bool init(MYSQL* );

private:
	MYSQL* _conn;

public :
	//插入有序点集 返回ID 失败返回-1
	unsigned int insertEntity(const vector<CPoint> &models);

	//根据ID删除数据 
	bool deletePintsByID(int ID);

	//批量删除点集
	bool deleteNPointsByIDs(const vector<int> &vIDs);

	//根据ID获取点集 [out]vPoints 会先对vPoints调用clear()
	void getPointsByID(int ID,vector<CPoint> &vPoints);
};

static WinPointsDAO* s_winpointsdao;
#endif