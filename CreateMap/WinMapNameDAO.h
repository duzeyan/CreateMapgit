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
	// ������  ��ȡwinmapname��������Ϣ
	// ����ֵ: void
	// ����:   vector<MODEL_WINMAPNAME> vModels [out] ���ؽ��
	//************************************
	void getAllEntities(vector<MODEL_WINMAPNAME> &vModels);

	//��ȡID��ѯ��ͼ������Ϣ(��,ͼƬ·��) model�����ǰ��Ҫ��ʼ��
	void getEntityByID(unsigned int mapID,MODEL_WINMAPNAME& model);

	
	//************************************
	// ������ ��winmapname�в�������,����ɹ�������ID
	// ����ֵ: bool   trueΪ�ɹ�
	// ����:   MODEL_WINMAPNAME model 
	//************************************
	bool insertEntity(MODEL_WINMAPNAME &model);

	//���ݵ�ͼIDɾ��������Ϣ
	bool deleteEntityByMapID(unsigned int mapID);
};

static WinMapNameDAO* s_winmapnamedao=nullptr;


#endif