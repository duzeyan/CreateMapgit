#ifndef NJUST_MAP_H
#define NJUST_MAP_H


#include"MapDrawControl.h"
#include <algorithm>
using std::vector;

#define  MAP_DASH_NUM 10    //ֱ�����߲ȵ���



//���ݻ�ͼ����ĵ��Ľ�� ��һ������
class NJUSTMap{
public:
	vector<CREATE_MAP_ROAD> roads;   //��·����
	vector<CREATE_MAP_NODE> nodes;   //�ڵ�����
	vector<CREATE_MAP_CROSS> crosses;//·������

	COMPUTE_GPS buildGPS[3];        //ͼ������3��� ͼƬ�����GPS
	double scaleX;					//X������GPS�ֲ�����
	double scaleY;				    //Y������GPS�ֲ�����
	vector<CREATE_MAP_OBSTACLE> obstacles;    //�ϰ���

public:
	NJUSTMap();

	//************************************
	// ������:   init
	// ������������ʼ��,��Ҫ����ͼƬ���ɵ������Ͷ�Ӧ��GPS
	// ����: 	 COMPUTE_GPS buildGPS[2]
	// ��������: void
	// ���ڣ�	 2016/04/18
	//************************************
	void init();

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
	// ����: 	 vector<DRAW_RECORD> mergeV :ѡ�еĻ滭��¼
	// ����: 	 CPoint line2ID			    :��·�������˵�
	// ��������: bool                       :�ɹ�����true
	// ���ڣ�	 2016/04/15
	//************************************
	bool merge2Line(const vector<DRAW_RECORD> &mergeV,MAP_ROAD package);


	//************************************
	// ������:   merge2Cross
	// ������������ѡ�еĻ滭��¼���кϲ�,�õ�·����Ϣ
	// ����: 	 const vector<DRAW_RECORD> & mergeV
	// ����: 	 CPoint line2ID            :·�ڵ�����ID
	// ��������: bool
	// ���ڣ�	 2016/04/27
	//************************************
	bool merge2Cross(const vector<DRAW_RECORD> &mergeV,CPoint line2ID);

	//************************************
	// ������:   printLine
	// ������������ӡһ����·��Ϣ
	// ����: 	 unsigned int index: ����
	// ��������: CString
	// ���ڣ�	 2016/04/15
	//************************************
	CString printRoad(unsigned int index)const;


	//************************************
	// ������:   printCross
	// ������������ӡһ��·����Ϣ
	// ����: 	 unsigned int index: ����
	// ��������: CString
	// ���ڣ�	 2016/04/27
	//************************************
	CString printCross(unsigned int index)const;

	//************************************
	// ������:   printLine
	// ������������ӡ���µ�·��Ϣ
	// ����: 	 unsigned int index
	// ��������: CString
	// ���ڣ�	 2016/04/15
	//************************************
	CString printRoadBack()const;

	//����IDɾ���ϰ���
	void removeObstaclesByID(int id);

	//���ض�Ŀ¼�±���������ļ�
	bool writeRoad(CString path);

	//��ָ��Ŀ¼�±����ı��ļ�
	bool writeRoadTxt(CString path);

	//����ת��ΪGPS
	void pixel2GPS(COMPUTE_GPS &p2g);

	//����ת��ΪGPS
	void GPS2pexel(COMPUTE_GPS &p2g);

	//����Ƿ��Ѿ����úò���
	bool CheckIsCali();

	//����߶�
	void computeScale();
	
	//************************************
	// ������:   serial
	// ������������ָ���ļ��־û���
	// ����: 	 CFile
	// ��������: void
	// ���ڣ�	 2016/04/29
	//************************************
	//[�ļ��ṹ]
	//... //������������
	//road��С (uint)
	//road[i]�ĵ�Ը��� (uint)
	//road[i]��n����� (CPoint)
	//road[i]��ͼ������ (CPoint)
	//road[i]��MAP_ROAD��Ϣ (MAP_ROAD)
	//nodes��crosses����
	//buildGPS scaleX scaleY
	//..//������������
	//***********************************
	void serial(CFile&);

	//�����л���
	void enserial(CFile& file);
};

#endif