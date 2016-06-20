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

//插入新的cache编号,填写
//1.若有空位直接填写
//2.若没有空位,优先替换不在被使用的
//3.不满足以上条件,则从0开始替换
void ImageCacheManage::insertCache(unsigned int code){
	unsigned int i;
	if(searchCache(code)!=0)
		return;

	//Step 1 -----------在空位中插入--------------
	for(i=0;i<IMAGE_CACHE_NUM;i++){
		if(_isEmpty[i]){
			_isEmpty[i]=false;
			_imageCode[i]=code;
			return;
		}
	}

	_imageCode[0]=code;
}