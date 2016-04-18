#ifndef NJUST_MAP_H
#define NJUST_MAP_H


#include"MapDrawControl.h"
#include <algorithm>
using std::vector;

#define  MAP_DASH_NUM 10    //ֱ�����߲ȵ���



//���ݻ�ͼ����ĵ��Ľ�� ��һ������
class NJUSTMap{
public:
	vector<CREATE_MAP_ROAD> roads; //��·����
	vector<CREATE_MAP_NODE> nodes; //·������
	COMPUTE_GPS buildGPS[2];         //���Ϻ����µ�ͼƬ�����GPS
	double scaleX;  //X������GPS�ֲ�����
	double scaleY;  //Y������GPS�ֲ�����
	

public:
	
	//************************************
	// ������:   init
	// ������������ʼ��,��Ҫ����ͼƬ���ɵ������Ͷ�Ӧ��GPS
	// ����: 	 COMPUTE_GPS buildGPS[2]
	// ��������: void
	// ���ڣ�	 2016/04/18
	//************************************
	void init(COMPUTE_GPS buildGPS[2]);

	//************************************
// ������:   deleteEleByID
// �����������������ͺ�IDɾ��Ԫ��
// ����: 	 bool isNode :trueΪ·�� falseΪ��·
// ����: 	 int id      :·�ڻ��ߵ�·��ID
// ��������: bool        :�ɹ�����true
// ���ڣ�	 2016/04/13
//************************************
	void deleteEleByID(bool isNode,int id);


	//************************************
	// ������:   merge2Line
	// ������������ѡ�еĻ滭��¼���кϲ�,�õ���·��Ϣ
	// ����: 	 vector<DRAW_RECORD> mergeV : ѡ�еĻ滭��¼
	// ����: 	 CPoint line2ID			    :��·�������˵�
	// ��������: bool                       :�ɹ�����true
	// ���ڣ�	 2016/04/15
	//************************************
	bool merge2Line(const vector<DRAW_RECORD> &mergeV,CPoint line2ID);

	//************************************
	// ������:   printLine
	// ������������ӡһ����·��Ϣ
	// ����: 	 unsigned int index
	// ��������: CString
	// ���ڣ�	 2016/04/15
	//************************************
	CString printRoad(unsigned int index)const;

	//************************************
	// ������:   printLine
	// ������������ӡ���µ�·��Ϣ
	// ����: 	 unsigned int index
	// ��������: CString
	// ���ڣ�	 2016/04/15
	//************************************
	CString printRoadBack()const;

	//���ض�Ŀ¼�±����ļ�
	bool writeRoad(CString path);

	void pixel2GPS(COMPUTE_GPS &p2g);

	
};

#endif