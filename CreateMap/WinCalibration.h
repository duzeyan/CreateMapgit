#ifndef WINCAILBRATION_H
#define WINCAILBRATION_H

#include "map_mysql_data.h"  
#include "MAP_BASIC_data.h"


//单例模式


class  WinCailbrationDAO{
public :static WinCailbrationDAO* getInstance(); //获取类实例
		bool init(MYSQL* con); //初始化
		void release();
private:
	    MYSQL* _conn;
public :
		
		
		///数据库操作///

	    
		//************************************
		// 描述： 根据ID查询表项,若查询失败则返回值中的mapID为-1
	    //        若返回值中的mapID和参数id一样侧表示查询成功
		// 返回值: MODEL_WINCALIBRATION
		// 参数:   int id 地图ID
		//************************************
		MODEL_WINCALIBRATION getEntityByMapID(int id);

		//************************************
		// 描述：  向表中插入一条MODEL_WINCALIBRATION数据
		// 返回值: bool true为插入成功
		// 参数:   MODEL_WINCALIBRATION calibationEntity
		//************************************
		bool insertEntity(MODEL_WINCALIBRATION calibationEntity);

		//删除
		bool deleteEntityByKey(unsigned int mapID);





private:
	   //************************************
	   // 描述： 通过id查询获取 tb_win_buildgps 表的数据
	   // 返回值: COMPUTE_GPS
	   // 参数:   int id
	   //************************************
	   COMPUTE_GPS getBuildGPSByID(int id);


	   //************************************
	   // 描述：在 tb_win_buildgps 表中插入数据
	   // 返回值: int  自增ID
	   // 参数:   COMPUTE_GPS cgps
	   //************************************
	   int insertBuildGPS(COMPUTE_GPS cgps);
};

static WinCailbrationDAO *s_Wincailbrationdao=nullptr;
#endif