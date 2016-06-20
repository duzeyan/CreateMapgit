#ifndef BLOCKIMAGE_H
#define BLOCKIMAGE_H 

#include "stdafx.h"

//�ֿ����� ƴ��ͼƬ

//�����ļ�(*.BIMG)�ĸ�ʽ�涨����(Ϊ�����ļ�)%��Ϊ�ı�ע��
//
//gwidth:X  %ͼƬ���
//gheight:Y %ͼƬ�߶�
//blockWNum:XX %��ȷֿ����
//blockHNum:YY %�߶ȷֿ����
//imagesDir:Z %ͼƬ��Ŀ¼,Ϊ����·��
//imageName: %ͼƬ��ͳһǰ׺
//%��ǰ׺Ϊ"ImageNJUST",�������ļ���Ϊ"ImageNJUST_3.png"
//%����3Ϊ���
//%��ŷ�ʽ(������������ͼ����Ϊ�Ŀ�)
//	|---------|--------|
//	|     1   |		2  |
//	|         |		   |
//	|---------|--------|
//	|     3   |	    4  |
//	|         |		   |
//	|---------|--------|
//%ͼƬ����
//

//ͼƬƴ������
enum NJUST_MAP_IMAGEFUSETYPE{
	FU_NONE=0X00, //����Ҫƴ��
	FU_UPDOWN,//����ƴ��
	FU_LEFTRIGHT, //����ƴ��
	FU_UDLR     //��������ƴ��
};

const unsigned int INVALID_IMAGE_NUM=1000*1000; //ͼƬ�ֿ�������1000*1000 ��
#define TEXT_LINE_STR 1024

class BlockImage{
public:
	//��ʼ��
	void init(CRect viewRect);
	 ~BlockImage();
	void release(); //����ڴ�
private:
	unsigned int _gheight; //ȫͼ�߶�
	unsigned int _gWidth;  //ȫͼ���
	unsigned int _blockWNum;//��ȷֿ���� 
	unsigned int _blockHNum;//�߶ȷֿ����
	unsigned int _blockW;//��ͼ���
	unsigned int _blockH;//��ͼ�߶�
	CString _imagesDir;
	CString _imageName;

	//�ڴ������ͬʱ��������ͼƬ 
	CImage *_imageLT;//����ͼƬ
	CImage *_imageRT;//����ͼƬ
	CImage *_imageLB;//����ͼƬ
	CImage *_imageRB;//����ͼƬ
	CImage *_imageFu;//ƴ��ͼƬ
	CImage *_imageFuBack;//ƴ��ͼƬ����
	CString _imagepathLT; //����ͼƬ·��,�����ػ�ʱ����
	CString _imagepathRT;
	CString _imagepathLB;
	CString _imagepathRB;
	CRect  _viewRect;//�Ӵ���С���Ӵ�����С��ͼ���С 
	CPoint _lastPoint; //�����õ��Ӵ����Ͻ�����
	CPoint _gFuLTPoint; //��ȫ��������,ƴ��ͼƬ�����Ͻ�����
	NJUST_MAP_IMAGEFUSETYPE _curFuseType; //��ǰƴ������
	NJUST_MAP_IMAGEFUSETYPE _lastFuseType;//���һ��ƴ������
	unsigned int _PointsNum[4];//����,����,����,����ͼƬ�ı��.  _fuseType��ͬ���������仯
	bool _isStateChange; // ��¼_curFuseType�Ƿ��˸ı�״̬
public :
	//��ȡ�����ļ� strΪȫ·��
	void readConfig(const char* str);

	//д�����ļ� strΪȫ·��
	void writeConfig(const char* str);

	//���ݵ�ǰ�Ӵ����Ͻ��ڴ�ͼ����ϵ�е������ȡͼƬ
	CImage *getImage(CPoint p);

	//ֱ����FuseImage
	CImage *getImage();

	//��ȡ��ǰƴ��ͼ���Ӵ�����
	CRect getFUImageRect();

	//��ȡ����ͼƬ(δ�����ƹ�)
	CImage *getImageBackUp();

	//ȫ�ֵ�ת��Ϊ��ǰƴ��ͼƬ�ϵĵ�(����ת��)
	CPoint gPTolP(CPoint p);


private:
	//��ʼ��ͼƬ����
	void malloc();

	//��ȡp������ͼƬ�ı��,
	//1.�������ⲿȷ��û�г���ͼ��Χ
	//2.����Ǵ�1��ʼ
	unsigned int getImageNum(CPoint p);

	//����������λ�ù�ϵ _fuseType�ᱻ�ı�,�������������ڿ�ͼƬ���
	void computeTwoPointsType(CPoint p1,CPoint p2,unsigned int &code1,unsigned int &code2);

	//��ȡFU_LEFTRIGHT ���������Ҫ��ƴ�Ӿ���
	void getFuseLRRect(CRect &rectL,CRect &rectR,CRect &rectFL,CRect &rectFR);

	//��ȡFU_UPDOWN ���������Ҫ��ƴ�Ӿ���
	void getFuseUDRect(CRect &rectU,CRect &rectD,CRect &rectFU,CRect &rectFD);

	//��ȡFU_UDLR ���������Ҫ��ƴ�Ӿ��� ����ע����1,2,3,4��˳�����������
	void getFuseUDLRRect(CRect rect[4],CRect rectF[4]);

	//ƴ����FU_NONE����µ�ͼƬ
	void fuseNone(unsigned int code);

	//ƴ����FU_LEFTRIGHT����µ�ͼƬ
	void fuseLR(unsigned int codeL,unsigned int codeR);

	//ƴ����FU_UPDOWN����µ�ͼƬ
	void fuseUD(unsigned int codeU,unsigned int codeD);

	//ƴ����FU_UDLR����µ�ͼƬ
	void fuseUDLR(unsigned int codeLT,unsigned int codeRD);


public:
	inline int getgWidth(){
		return _gWidth;
	}

	inline int getgHeight(){
		return _gheight;
	}

	inline bool isStateChange(){
		return _isStateChange;
	}
};

#endif

