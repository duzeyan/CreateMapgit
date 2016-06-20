#include "stdafx.h"
#include "ImageCacheManage.h"

ImageCacheManage::ImageCacheManage(){
	memset(_isUsing,0,sizeof(_isUsing));
	memset(_isEmpty,0,sizeof(_isEmpty));
	memset(_imageCode,0,sizeof(_imageCode));
}

unsigned int ImageCacheManage::searchCache(unsigned int code){
	unsigned int i;
	for(i=0;i<IMAGE_CACHE_NUM;i++){
		if(!_isEmpty[i]&&code==_imageCode[i])
			return i;
	}
	return 0;
}

//�����µ�cache���,��д
//1.���п�λֱ����д
//2.��û�п�λ,�����滻���ڱ�ʹ�õ�
//3.��������������,���0��ʼ�滻
void ImageCacheManage::insertCache(unsigned int code){
	unsigned int i;
	if(searchCache(code)!=0)
		return;

	//Step 1 -----------�ڿ�λ�в���--------------
	for(i=0;i<IMAGE_CACHE_NUM;i++){
		if(_isEmpty[i]){
			_isEmpty[i]=false;
			_imageCode[i]=code;
			return;
		}
	}

	_imageCode[0]=code;
}