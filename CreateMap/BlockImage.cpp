#include "stdafx.h"
#include "BlockImage.h"
#include <fstream>
#include <sstream>

void BlockImage::init(CRect viewRect){
	_viewRect=viewRect;
	_imageLT=NULL;//左上图片
	_imageRT=NULL;//右上图片
	_imageLB=NULL;//左下图片
	_imageRB=NULL;//右下图片
	_imageFu=NULL;//拼接图片
	_imageFuBack=new(std::nothrow) CImage();//拼接图片备份
	_lastPoint.SetPoint(0,0);
	_curFuseType=FU_NONE;
	_lastFuseType=FU_NONE;
	_viewRect.SetRect(CPoint(0,0),CPoint(0,0));
	memset(_PointsNum,0,sizeof(_PointsNum));
}


void BlockImage::readConfig(CString filename){
	std::ifstream fin(filename, std::ios::in);
	char line[TEXT_LINE_STR];
	std::string x = "";
	int it[4];
	CString str[2];
	int i=0,k=0;

	while(fin.getline(line, sizeof(line)))
	{
		fin.getline(line, sizeof(line));
		std::stringstream sline(line);
		sline>>x; //存入临时string

		if(i<4){
			it[i]=atoi(x.c_str());
			i++;
		}else{
			str[k]=x.c_str();
			k++;
		}
	}
	fin.clear();
	fin.close();

	_gWidth=it[0];
	_gheight=it[1];
	_blockWNum=it[2];
	_blockHNum=it[3];
	_imagesDir=str[0];
	_imageName=str[1];
	_blockW=_gWidth/_blockWNum;
	_blockH=_gheight/_blockHNum;
	return ;
}

unsigned int BlockImage::getImageNum(CPoint p){
	unsigned int xIndex,yIndex;
	xIndex=p.x/_blockWNum;
	yIndex=p.y/_blockHNum;
	return yIndex*_blockWNum+xIndex+1;
}

void BlockImage::computeTwoPointsType(CPoint p1,CPoint p2,unsigned int &code1,unsigned int &code2){
	//Step 1 -------------获取两个点的所在图片索引------------
	code1=getImageNum(p1);
	code2=getImageNum(p2);

	//Step 2 -----------根据点的类型判断融合类型--------------
	_lastFuseType=_curFuseType;
	if(code1==code2){  //两点编号相同
		_curFuseType=FU_NONE;
		return ;
	}
	if(code1+1==code2){//两点编号左右相邻
		_curFuseType=FU_LEFTRIGHT;
		return ;
	}
	if(code1+_blockWNum==code2){//两点编号上下相邻
		_curFuseType=FU_UPDOWN;
		return;
	}
	if(code1+_blockWNum+1==code2){//两点编号对角线相邻
		_curFuseType=FU_UDLR;
		return;
	}

	//没有其他情况
}

void BlockImage::fuseNone(unsigned int code){
	//Step 1 -----------检查对象是否为空--------------
	CString imagefilename;
	if(_imageFu==NULL)
		_imageFu=new(std::nothrow) CImage();
	if(_imageFuBack==NULL)
		_imageFuBack=new(std::nothrow) CImage();
	assert(_imageFuBack!=NULL);

	//Step 2 -----------加载图片文件--------------
	_imageFu->Destroy();//销毁上存储的不同位图
	imagefilename.Format(L"%s//%s(%d).png",_imagesDir,
		_imageName,
		code); //拼接文件绝对路径
	_imageFu->Load(imagefilename); //直接作为融合图片
	_imageFuBack->Load(imagefilename);

	return;
}


void BlockImage::getFuseLRRect(CRect &rectL,CRect &rectR,CRect &rectFL,CRect &rectFR){
	CPoint p1,p2; //用于设置矩阵 p1为矩阵左上点,p2为矩阵右下点

	//Step 1 ------------左图截取矩形-------------
	p1.SetPoint(_blockW-_viewRect.Width(),0);
	p2.SetPoint(_blockW-1,_blockH-1);
	rectL.SetRect(p1,p2);

	//Step 2 -----------右图截取矩形,拼接图左半边截取矩形--------------
	p1.SetPoint(0,0);
	p2.SetPoint(_viewRect.Width()-1,_blockH-1);
	rectR.SetRect(p1,p2);
	rectFL.SetRect(p1,p2);

	//Step 3 -----------拼接图右半边截取矩形--------------
	p1.SetPoint(_viewRect.Width(),0);
	p2.SetPoint(_viewRect.Width()*2-1,_blockH-1);
	rectFR.SetRect(p1,p2);
}

void BlockImage::fuseLR(unsigned int codeL,unsigned int codeR){
	CString imagefilename;
	HDC hdcFU=NULL;
	CRect rectL,rectR,rectFL,rectFR; //左右块图的截取矩形 拼接图左右截取矩形

	//Step 1 -----------检查对象是否为空,若空则赋值--------------
	if(_imageLT==NULL)
		_imageLT=new(std::nothrow) CImage();
	if(_imageRT==NULL)
		_imageRT=new(std::nothrow) CImage();
	if(_imageFu==NULL)
		_imageFu=new(std::nothrow) CImage();
	if(_imageFuBack==NULL)
		_imageFuBack=new(std::nothrow) CImage();
	
	assert(_imageLT!=NULL);
	assert(_imageRT!=NULL);
	assert(_imageFu!=NULL);
	assert(_imageFuBack!=NULL);

	//Step 2 -----------释放上一次加载的图片资源--------------
	_imageLT->Destroy();
	_imageRT->Destroy();
	_imageFu->Destroy();
	_imageFuBack->Destroy();

	//Step 3 -----------加载左图和右图--------------
	//加载左图
	imagefilename.Format(L"%s//%s(%d).png",_imagesDir,
		_imageName,
		codeL);
	_imageLT->Load(imagefilename);

	//加载右图
	imagefilename.Format(L"%s//%s(%d).png",_imagesDir,
		_imageName,
		codeR);
	_imageRT->Load(imagefilename);

	//Step 4 -----------拼接左右图--------------
	_imageFu->Create(_viewRect.Width()*2,_imageLT->GetHeight(),24);//设置拼接图片大小
	_imageFuBack->Create(_viewRect.Width()*2,_imageLT->GetHeight(),24);//设置拼接图片大小
	getFuseLRRect(rectL,rectR,rectFL,rectFR);//获取截取矩形
	
	//绘制拼接图
	hdcFU=_imageFu->GetDC();
	_imageLT->Draw(hdcFU,rectFL,rectL);	//左半边
	_imageRT->Draw(hdcFU,rectFR,rectR);	//右半边	
	_imageFu->ReleaseDC();

	//绘制备份图
	hdcFU=_imageFuBack->GetDC();
	_imageLT->Draw(hdcFU,rectFL,rectL);	//左半边
	_imageRT->Draw(hdcFU,rectFR,rectR);	//右半边	
	_imageFu->ReleaseDC();

	return;
}

CImage* BlockImage::getImage(CPoint p){
	CPoint endPoint;
	unsigned int code1,code2;//两点所在块图中的编号
	endPoint.SetPoint(p.x+_viewRect.Width()-1,p.y+_viewRect.Height()-1);

	//Step 1 -----------获取拼接类型--------------
	_lastPoint=p;//记录最近请求点
	this->computeTwoPointsType(p,endPoint,code1,code2);

	//Step 2 -----------根据拼接类型返回图像--------------
	if(_imageFu!=NULL&&_lastFuseType==_curFuseType) //状态未变,且已经计算过结果直接返回上一次的结果
		return _imageFu;

	switch(_curFuseType){
		//不需要拼接
		case FU_NONE:	{   
							this->fuseNone(code1);//code2==code1
							_lastFuseType=FU_NONE;
							break;
						 }
		//拼接左右相邻的图
		case FU_LEFTRIGHT:{
							this->fuseLR(code1,code2);
							_lastFuseType=FU_LEFTRIGHT;
							break;
						  }
	}
	return _imageFu;
}

CImage* BlockImage::getImageBackUp(){
	return _imageFuBack;
}