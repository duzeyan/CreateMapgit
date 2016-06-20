#ifndef BLOCKIMAGE_H
#define BLOCKIMAGE_H 

#include "stdafx.h"

//分块载入 拼接图片

//配置文件(*.BIMG)的格式规定如下(为本文文件)%后为文本注释
//
//gwidth:X  %图片宽度
//gheight:Y %图片高度
//blockWNum:XX %宽度分块个数
//blockHNum:YY %高度分块个数
//imagesDir:Z %图片集目录,为绝对路径
//imageName: %图片集统一前缀
//%如前缀为"ImageNJUST",则完整文件名为"ImageNJUST_3.png"
//%其中3为编号
//%标号方式(此例中整个大图被分为四块)
//	|---------|--------|
//	|     1   |		2  |
//	|         |		   |
//	|---------|--------|
//	|     3   |	    4  |
//	|         |		   |
//	|---------|--------|
//%图片划分
//

//图片拼接类型
enum NJUST_MAP_IMAGEFUSETYPE{
	FU_NONE=0X00, //不需要拼接
	FU_UPDOWN,//上下拼接
	FU_LEFTRIGHT, //左右拼接
	FU_UDLR     //上下左右拼接
};

const unsigned int INVALID_IMAGE_NUM=1000*1000; //图片分块上限是1000*1000 块
#define TEXT_LINE_STR 1024

class BlockImage{
public:
	//初始化
	void init(CRect viewRect);
	 ~BlockImage();
	void release(); //清除内存
private:
	unsigned int _gheight; //全图高度
	unsigned int _gWidth;  //全图宽度
	unsigned int _blockWNum;//宽度分块个数 
	unsigned int _blockHNum;//高度分块个数
	unsigned int _blockW;//块图宽度
	unsigned int _blockH;//快图高度
	CString _imagesDir;
	CString _imageName;

	//内存中最多同时存在四张图片 
	CImage *_imageLT;//左上图片
	CImage *_imageRT;//右上图片
	CImage *_imageLB;//左下图片
	CImage *_imageRB;//右下图片
	CImage *_imageFu;//拼接图片
	CImage *_imageFuBack;//拼接图片备份
	CString _imagepathLT; //左上图片路径,用于重绘时读入
	CString _imagepathRT;
	CString _imagepathLB;
	CString _imagepathRB;
	CRect  _viewRect;//视窗大小，视窗必须小于图块大小 
	CPoint _lastPoint; //最近获得的视窗左上角坐标
	CPoint _gFuLTPoint; //在全局坐标中,拼接图片的左上角坐标
	NJUST_MAP_IMAGEFUSETYPE _curFuseType; //当前拼接类型
	NJUST_MAP_IMAGEFUSETYPE _lastFuseType;//最近一次拼接类型
	unsigned int _PointsNum[4];//左上,右上,左下,右下图片的编号.  _fuseType不同语义有所变化
	bool _isStateChange; // 记录_curFuseType是否了改变状态
public :
	//读取配置文件 str为全路径
	void readConfig(const char* str);

	//写配置文件 str为全路径
	void writeConfig(const char* str);

	//根据当前视窗左上角在大图坐标系中的坐标获取图片
	CImage *getImage(CPoint p);

	//直接拿FuseImage
	CImage *getImage();

	//获取当前拼接图的视窗矩阵
	CRect getFUImageRect();

	//获取备份图片(未被绘制过)
	CImage *getImageBackUp();

	//全局点转化为当前拼接图片上的点(坐标转换)
	CPoint gPTolP(CPoint p);


private:
	//初始化图片对象
	void malloc();

	//获取p点所在图片的编号,
	//1.输入由外部确定没有超过图像范围
	//2.编号是从1开始
	unsigned int getImageNum(CPoint p);

	//计算两个点位置关系 _fuseType会被改变,返回两个点所在块图片编号
	void computeTwoPointsType(CPoint p1,CPoint p2,unsigned int &code1,unsigned int &code2);

	//获取FU_LEFTRIGHT 情况下所需要的拼接矩形
	void getFuseLRRect(CRect &rectL,CRect &rectR,CRect &rectFL,CRect &rectFR);

	//获取FU_UPDOWN 情况下所需要的拼接矩形
	void getFuseUDRect(CRect &rectU,CRect &rectD,CRect &rectFU,CRect &rectFD);

	//获取FU_UDLR 情况下所需要的拼接矩形 按照注释中1,2,3,4的顺序来构造矩形
	void getFuseUDLRRect(CRect rect[4],CRect rectF[4]);

	//拼接在FU_NONE情况下的图片
	void fuseNone(unsigned int code);

	//拼接在FU_LEFTRIGHT情况下的图片
	void fuseLR(unsigned int codeL,unsigned int codeR);

	//拼接在FU_UPDOWN情况下的图片
	void fuseUD(unsigned int codeU,unsigned int codeD);

	//拼接在FU_UDLR情况下的图片
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

