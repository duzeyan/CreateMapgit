#ifndef  IMAGE_CACHE_MANAGE_H
#define IMAGE_CACHE_MANAGE_H

const unsigned int IMAGE_CACHE_NUM=4;
class ImageCacheManage{
public:
	ImageCacheManage();
private:
	bool _isUsing[IMAGE_CACHE_NUM];//1 2 3 4��ͼ�Ƿ���ʹ��
	bool _isEmpty[IMAGE_CACHE_NUM];//1 2 3 4��ͼ�Ƿ����
	unsigned int _imageCode[IMAGE_CACHE_NUM];//�Ŀ�ƴ��ͼ1 2 3 4��ͼ�ı��
public:
	//��ѯ�������Ƿ�������Ҫ�ı�� û�з���0,��ѯ����������
	unsigned int searchCache(unsigned int code);

	//�����µ�cache���,��д
	//1.���п�λֱ����д
	//2.��û�п�λ,�����滻���ڱ�ʹ�õ�
	//3.��������������,���0��ʼ�滻
	void insertCache(unsigned int code);
};


#endif
