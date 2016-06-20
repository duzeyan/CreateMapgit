#ifndef  IMAGE_CACHE_MANAGE_H
#define IMAGE_CACHE_MANAGE_H

const unsigned int IMAGE_CACHE_NUM=4;
class ImageCacheManage{
public:
	ImageCacheManage();
private:
	bool _isUsing[IMAGE_CACHE_NUM];//1 2 3 4子图是否在使用
	bool _isEmpty[IMAGE_CACHE_NUM];//1 2 3 4子图是否加载
	unsigned int _imageCode[IMAGE_CACHE_NUM];//四块拼接图1 2 3 4子图的编号
public:
	//查询缓存中是否有所需要的编号 没有返回0,查询到返回索引
	unsigned int searchCache(unsigned int code);

	//插入新的cache编号,填写
	//1.若有空位直接填写
	//2.若没有空位,优先替换不在被使用的
	//3.不满足以上条件,则从0开始替换
	void insertCache(unsigned int code);
};


#endif
