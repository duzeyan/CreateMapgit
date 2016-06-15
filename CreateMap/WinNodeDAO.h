#ifndef WINNODEDAO_H
#define WINNODEDAO_H

#include "map_mysql_data.h"

//�� tb_win_node ����
class WinNodeDAO{
public:
	static WinNodeDAO* getInstance();
	bool init(MYSQL* );

private:
	MYSQL* _conn;

public :
	//����mapID��ȡ�õ�ͼ�����нڵ���Ϣ
	void getEntitiesByMapID(unsigned int mapID,vector<CREATE_MAP_NODE> &models);


	//����һ������
	bool insertEntity(const MODEL_WINNODE &model,unsigned int mapID);

	//��������ɾ������
	bool deleteEntityByKey(unsigned int mapID,int idself);

	//���ݵ�ͼIDɾ��������Ϣ
	bool deleteAllByMapID(unsigned int mapID);

private: 
	//************************************
	// ������  ƴ�Ӳ��빦�ܵ�sql���
	// ����ֵ: void
	// ����:   char * sql [out]ƴ�ӽ�� ������ǰ��Ҫ��ʼ����0
	// ����:   const MODEL_WINNODE & model �ڵ�����
	// ����:   unsigned int mapID  ��ͼID
	//************************************
	void getInsertSql(char *sql,const MODEL_WINNODE &model,unsigned int mapID);

	//��ѯ��������ת��Ϊ�ṹ��������
	void row2Entity(const MYSQL_ROW &row,CREATE_MAP_NODE &model);

	
};

static WinNodeDAO* s_winNodeDAO=nullptr; 

#endif 