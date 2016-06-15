#ifndef WINMAPNAMEDAO_H
#define WINMAPNAMEDAO_H

#include "map_mysql_data.h"  
#include "MAP_BASIC_data.h"

class WinMapNameDAO{
public: static WinMapNameDAO* getInstance();
		bool init(MYSQL* con);
		void release();
private:
	  MYSQL* _conn;
public :


	//************************************
	// 描述：  获取winmapname的所有信息
	// 返回值: void
	// 参数:   vector<MODEL_WINMAPNAME> vModels [out] 返回结果
	//************************************
	void getAllEntities(vector<MODEL_WINMAPNAME> &vModels);

	//获取ID查询地图基本信息(名,图片路径) model在填充前需要初始化
	void getEntityByID(unsigned int mapID,MODEL_WINMAPNAME& model);

	
	//************************************
	// 描述： 在winmapname中插入数据,插入成功在填入ID
	// 返回值: bool   true为成功
	// 参数:   MODEL_WINMAPNAME model 
	//************************************
	bool insertEntity(MODEL_WINMAPNAME &model);

	//根据地图ID删除所有信息
	bool deleteEntityByMapID(unsigned int mapID);
};

static WinMapNameDAO* s_winmapnamedao=nullptr;


#endif