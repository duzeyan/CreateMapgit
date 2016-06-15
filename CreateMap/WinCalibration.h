#ifndef WINCAILBRATION_H
#define WINCAILBRATION_H

#include "map_mysql_data.h"  
#include "MAP_BASIC_data.h"


//����ģʽ


class  WinCailbrationDAO{
public :static WinCailbrationDAO* getInstance(); //��ȡ��ʵ��
		bool init(MYSQL* con); //��ʼ��
		void release();
private:
	    MYSQL* _conn;
public :
		
		
		///���ݿ����///

	    
		//************************************
		// ������ ����ID��ѯ����,����ѯʧ���򷵻�ֵ�е�mapIDΪ-1
	    //        ������ֵ�е�mapID�Ͳ���idһ�����ʾ��ѯ�ɹ�
		// ����ֵ: MODEL_WINCALIBRATION
		// ����:   int id ��ͼID
		//************************************
		MODEL_WINCALIBRATION getEntityByMapID(int id);

		//************************************
		// ������  ����в���һ��MODEL_WINCALIBRATION����
		// ����ֵ: bool trueΪ����ɹ�
		// ����:   MODEL_WINCALIBRATION calibationEntity
		//************************************
		bool insertEntity(MODEL_WINCALIBRATION calibationEntity);

		//ɾ��
		bool deleteEntityByKey(unsigned int mapID);





private:
	   //************************************
	   // ������ ͨ��id��ѯ��ȡ tb_win_buildgps �������
	   // ����ֵ: COMPUTE_GPS
	   // ����:   int id
	   //************************************
	   COMPUTE_GPS getBuildGPSByID(int id);


	   //************************************
	   // �������� tb_win_buildgps ���в�������
	   // ����ֵ: int  ����ID
	   // ����:   COMPUTE_GPS cgps
	   //************************************
	   int insertBuildGPS(COMPUTE_GPS cgps);
};

static WinCailbrationDAO *s_Wincailbrationdao=nullptr;
#endif