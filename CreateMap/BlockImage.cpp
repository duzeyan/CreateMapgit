#include "stdafx.h"
#include "BlockImage.h"
#include <fstream>
#include <sstream>
#include "ToolsUtil.h"

const unsigned int BLOCK_IMAGE_PATH_MAX=255; 


void BlockImage::init(CRect viewRect){
	malloc();
	_viewRect=viewRect;
	_lastPoint.SetPoint(0,0);
	_curFuseType=FU_NONE;
	_lastFuseType=FU_NONE;
	_isStateChange=false;
	_isFirst=true;
	_reDraw=false;
	memset(_PointsNum,0,sizeof(_PointsNum));
}

void BlockImage::malloc(){
	_imageLT=new(std::nothrow) CImage();
	_imageLB=new(std::nothrow) CImage();
	_imageRT=new(std::nothrow) CImage();
	_imageRB=new(std::nothrow) CImage();
	_imageFu=new(std::nothrow) CImage();
	_imageFuBack=new(std::nothrow) CImage();

	assert(_imageLT!=NULL);
	assert(_imageLB!=NULL);
	assert(_imageRT!=NULL);
	assert(_imageRB!=NULL);
	assert(_imageFu!=NULL);
	assert(_imageFuBack!=NULL);
}

BlockImage::~BlockImage(){
	release();
	return ;
}
void BlockImage::release(){
	if(_imageLT!=NULL)
		delete[] _imageLT;//左上图片
	if(_imageRT!=NULL)
		delete[] _imageRT;//右上图片
	if(_imageLB!=NULL)
		delete[] _imageLB;//左下图片
	if(_imageRB!=NULL)
		delete[] _imageRB;//右下图片
	if(_imageFu!=NULL)
		delete[] _imageFu;//拼接图片
	if(_imageFuBack!=NULL)
		delete[] _imageFuBack;//拼接图片备份
	_imageLT=_imageRT=_imageLB=_imageRB=_imageFu=_imageFuBack=NULL;
}

//TODO 7.1前换成XML读存
void BlockImage::readConfig(const char * filename){
	std::ifstream fin(filename, std::ios::in);
	char line[TEXT_LINE_STR];
	std::string x = "";
	int it[4];
	CString str[2];
	int i=0,k=0;

	while(fin.getline(line, sizeof(line)))
	{
		//fin.getline(line, sizeof(line));
		std::stringstream sline(line);
		sline>>x; //存入临时string
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

//写配置文件 str为全路径
void BlockImage::writeConfig(const char* filename){
	std::ofstream fout(filename, std::ios::out);
	char tstr[BLOCK_IMAGE_PATH_MAX];
	char line[TEXT_LINE_STR];
	
	sprintf(line,"gwidth %d\n",_gWidth);
	fout<<line;
	sprintf(line,"gheight %d\n",_gheight);
	fout<<line;
	sprintf(line,"blockWNum %d\n",_blockWNum);
	fout<<line;
	sprintf(line,"blockHNum %d\n",_blockHNum);
	fout<<line;
	ToolsUtil::WtoA(tstr,BLOCK_IMAGE_PATH_MAX,&_imagesDir);
	sprintf(line,"imagesDir %s\n",tstr);
	fout<<line;
	ToolsUtil::WtoA(tstr,BLOCK_IMAGE_PATH_MAX,&_imageName);
	sprintf(line,"imageName %s\n",tstr);
	fout<<line;

	fout.flush();
	fout.close();
	
}


unsigned int BlockImage::getImageNum(CPoint p){
	unsigned int xIndex,yIndex;
	xIndex=p.x/_blockW;
	yIndex=p.y/_blockH;
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



void BlockImage::getFuseLRRect(CRect &rectL,CRect &rectR,CRect &rectFL,CRect &rectFR){
	CPoint p1,p2; //用于设置矩阵 p1为矩阵左上点,p2为矩阵右下点
	//注意:CImage::Draw的中的矩形参数,绘制时不会绘制矩形的右边和下边
	//所以这里选取的矩形在右边和下面多加一个像素

	//Step 1 ------------左图截取矩形-------------
	p1.SetPoint(_blockW-_viewRect.Width(),0);
	p2.SetPoint(_blockW,_blockH);
	rectL.SetRect(p1,p2);

	//Step 2 -----------右图截取矩形,拼接图左半边截取矩形--------------
	p1.SetPoint(0,0);
	p2.SetPoint(_viewRect.Width(),_blockH);
	rectR.SetRect(p1,p2);
	rectFL.SetRect(p1,p2);

	//Step 3 -----------拼接图右半边截取矩形--------------
	p1.SetPoint(_viewRect.Width(),0);
	p2.SetPoint(_viewRect.Width()*2,_blockH);
	rectFR.SetRect(p1,p2);
}

//获取FU_UPDOWN 情况下所需要的拼接矩形
void BlockImage::getFuseUDRect(CRect &rectU,CRect &rectD,CRect &rectFU,CRect &rectFD){
	CPoint p1,p2; //用于设置矩阵 p1为矩阵左上点,p2为矩阵右下点

	//Step 1 ------------上图截取矩形-------------
	p1.SetPoint(0,_blockH-_viewRect.Height());
	p2.SetPoint(_blockW,_blockH);
	rectU.SetRect(p1,p2);

	//Step 2 -----------下图截取矩形,拼接图的上半块--------------
	p1.SetPoint(0,0);
	p2.SetPoint(_blockW,_viewRect.Height());
	rectD.SetRect(p1,p2);
	rectFU.SetRect(p1,p2);

	//Step 3 -----------拼接图下半边截取矩形--------------
	p1.SetPoint(0,_viewRect.Height());
	p2.SetPoint(_blockW,_viewRect.Height()*2);
	rectFD.SetRect(p1,p2);
}

void BlockImage::getFuseUDLRRect(CRect rect[4],CRect rectF[4]){
	CPoint p1,p2; //用于设置矩阵 p1为矩阵左上点,p2为矩阵右下点

	//Step 1 ------------LT截取矩形-------------
	p1.SetPoint(_blockW-_viewRect.Width(),_blockH-_viewRect.Height());
	p2.SetPoint(_blockW,_blockH);
	rect[0].SetRect(p1,p2);

	//Step 2 ------------RT截取矩形-------------
	p1.SetPoint(0,_blockH-_viewRect.Height());
	p2.SetPoint(_viewRect.Width(),_blockH);
	rect[1].SetRect(p1,p2);

	//Step 3 ------------LB截取矩形-------------
	p1.SetPoint(_blockW-_viewRect.Width(),0);
	p2.SetPoint(_blockW,_viewRect.Height());
	rect[2].SetRect(p1,p2);

	//Step 4 ------------RB截取矩形,LT拼接图截取矩形-------------
	p1.SetPoint(0,0);
	p2.SetPoint(_viewRect.Width(),_viewRect.Height());
	rect[3].SetRect(p1,p2);
	rectF[0].SetRect(p1,p2);

	//Step 5 ------------RT拼接图截取矩形-------------
	p1.SetPoint(_viewRect.Width(),0);
	p2.SetPoint(_viewRect.Width()*2,_viewRect.Height());
	rectF[1].SetRect(p1,p2);

	//Step 6 ------------LB拼接图截取矩形-------------
	p1.SetPoint(0,_viewRect.Height());
	p2.SetPoint(_viewRect.Width(),_viewRect.Height()*2);
	rectF[2].SetRect(p1,p2);

	//Step 7 ------------RB拼接图截取矩形-------------
	p1.SetPoint(_viewRect.Width(),_viewRect.Height());
	p2.SetPoint(_viewRect.Width()*2,_viewRect.Height()*2);
	rectF[3].SetRect(p1,p2);
	return;
}


void BlockImage::fuseNone(unsigned int code){
	CString imagefilename;
	unsigned int indexH=(code-1)/_blockWNum; //块在整图中的行号 从0开始
	unsigned int indexW=(code-1)%_blockWNum; //块在整图中的列号 从0开始

	//Step 1 -----------检查对象是否为空--------------

	if(_imageFu==NULL)
		_imageFu=new(std::nothrow) CImage();
	if(_imageFuBack==NULL)
		_imageFuBack=new(std::nothrow) CImage();
	assert(_imageFu!=NULL);
	assert(_imageFuBack!=NULL);

	//Step 2 -----------加载图片文件--------------
	_imageFu->Destroy();//销毁上存储的不同位图
	_imageFuBack->Destroy();

	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		code); //拼接文件绝对路径
	_imageFu->Load(imagefilename); //直接作为融合图片
	_imageFuBack->Load(imagefilename);
	//拼接图左上角在全局图中的坐标
	_gFuLTPoint.SetPoint(indexW*_blockW,indexH*_blockH);
	return;
}

void BlockImage::fuseLR(unsigned int codeL,unsigned int codeR){
	CString imagefilename;
	HDC hdcFU=NULL;
	CRect rectL,rectR,rectFL,rectFR; //左右块图的截取矩形 拼接图左右截取矩形
	unsigned int indexH=(codeL-1)/_blockWNum; //块在整图中的行号 从0开始
	unsigned int indexW=(codeL-1)%_blockWNum; //块在整图中的列号 从0开始
	int FUWidth=0;


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
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeL);
	_imageLT->Load(imagefilename);

	//加载右图
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
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
	_imageFuBack->ReleaseDC();

	//Step 5 -----------拼接图左上角在全局图中的坐标--------------
	FUWidth=indexW*_blockW+(_blockW-_viewRect.Width());
	_gFuLTPoint.SetPoint(FUWidth,indexH*_blockH);
	return;
}

void BlockImage::fuseUD(unsigned int codeU,unsigned int codeD){
	CString imagefilename;
	HDC hdcFU=NULL;
	CRect rectU,rectD,rectFU,rectFD; //上下块图的截取矩形 拼接图上下截取矩形
	unsigned int indexH=(codeU-1)/_blockWNum; //用于计算拼接图左上角坐标
	unsigned int indexW=(codeU-1)%_blockWNum; 
	int FUHeight=0;

	//Step 1 -----------检查对象是否为空,若空则赋值--------------
	if(_imageLT==NULL)
		_imageLT=new(std::nothrow) CImage();
	if(_imageLB==NULL)
		_imageLB=new(std::nothrow) CImage();
	if(_imageFu==NULL)
		_imageFu=new(std::nothrow) CImage();
	if(_imageFuBack==NULL)
		_imageFuBack=new(std::nothrow) CImage();

	assert(_imageLT!=NULL);
	assert(_imageLB!=NULL);
	assert(_imageFu!=NULL);
	assert(_imageFuBack!=NULL);

	//Step 2 -----------释放上一次加载的图片资源--------------
	_imageLT->Destroy();
	_imageLB->Destroy();
	_imageFu->Destroy();
	_imageFuBack->Destroy();

	//Step 3 -----------加载左图和右图--------------
	//加载左图
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeU);
	_imageLT->Load(imagefilename);

	//加载右图
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeD);
	_imageLB->Load(imagefilename);

	//Step 4 -----------拼接左右图--------------
	_imageFu->Create(_imageLT->GetWidth(),_viewRect.Height()*2,24);//设置拼接图片大小
	_imageFuBack->Create(_imageLT->GetWidth(),_viewRect.Height()*2,24);//设置拼接图片大小
	getFuseUDRect(rectU,rectD,rectFU,rectFD);//获取截取矩形

	//绘制拼接图
	hdcFU=_imageFu->GetDC();
	_imageLT->Draw(hdcFU,rectFU,rectU);	//上半边
	_imageLB->Draw(hdcFU,rectFD,rectD);	//下半边	
	_imageFu->ReleaseDC();

	//绘制备份图
	hdcFU=_imageFuBack->GetDC();
	_imageLT->Draw(hdcFU,rectFU,rectU);	//上半边
	_imageLB->Draw(hdcFU,rectFD,rectD);	//下半边	
	_imageFuBack->ReleaseDC();

	//Step 5 -----------拼接图左上角在全局图中的坐标--------------
	FUHeight=indexH*_blockH+(_blockH-_viewRect.Height());
	_gFuLTPoint.SetPoint(indexW*_blockW,FUHeight);
	return;
}

void BlockImage::fuseUDLR(unsigned int codeLT,unsigned int codeRD){
	CString imagefilename;
	HDC hdcFU=NULL;
	CRect rect[4],rectF[4]; //上下块图的截取矩形 拼接图上下截取矩形
	unsigned int codeRT=codeLT+1;
	unsigned int codeLD=codeRD-1;
	unsigned int indexH=(codeLT-1)/_blockWNum; //用于计算拼接图左上角坐标
	unsigned int indexW=(codeLT-1)%_blockWNum; 
	int FUHeight,FUWidth;

	//Step 1 -----------检查对象是否为空,若空则赋值--------------
	

	//Step 2 -----------释放上一次加载的图片资源--------------
	_imageLT->Destroy();
	_imageLB->Destroy();
	_imageRB->Destroy();
	_imageRT->Destroy();
	_imageFu->Destroy();
	_imageFuBack->Destroy();

	//Step 3 -----------加载左图和右图--------------
	//加载1图
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeLT);
	_imageLT->Load(imagefilename);

	//加载2图
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeRT);
	_imageRT->Load(imagefilename);

	//加载3图
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeLD);
	_imageLB->Load(imagefilename);

	//加载4图
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeRD);
	_imageRB->Load(imagefilename);

	//Step 4 -----------拼接左右图--------------
	_imageFu->Create(_viewRect.Width()*2,_viewRect.Height()*2,24);//设置拼接图片大小
	_imageFuBack->Create(_viewRect.Width()*2,_viewRect.Height()*2,24);//设置拼接图片大小
	getFuseUDLRRect(rect,rectF);

	//绘制拼接图
	hdcFU=_imageFu->GetDC();
	_imageLT->Draw(hdcFU,rectF[0],rect[0]);	//1
	_imageRT->Draw(hdcFU,rectF[1],rect[1]);	//2
	_imageLB->Draw(hdcFU,rectF[2],rect[2]);	//3
	_imageRB->Draw(hdcFU,rectF[3],rect[3]);	//4
	_imageFu->ReleaseDC();

	//绘制备份图
	hdcFU=_imageFuBack->GetDC();
	_imageLT->Draw(hdcFU,rectF[0],rect[0]);	//1
	_imageRT->Draw(hdcFU,rectF[1],rect[1]);	//2
	_imageLB->Draw(hdcFU,rectF[2],rect[2]);	//3
	_imageRB->Draw(hdcFU,rectF[3],rect[3]);	//4	
	_imageFuBack->ReleaseDC();

	//Step 5 -----------拼接图左上角在全局图中的坐标--------------
	FUWidth=indexW*_blockW+(_blockW-_viewRect.Width());
	FUHeight=indexH*_blockH+(_blockH-_viewRect.Height());
	_gFuLTPoint.SetPoint(FUWidth,FUHeight);
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
	if(_isFirst||_reDraw){
		_isFirst=false;
	}else{
		if(_imageFu!=NULL&&(_lastFuseType==_curFuseType)){ //状态未变,且已经计算过结果直接返回上一次的结果
			_isStateChange=false;
			return _imageFu;
		}
	}

	_isStateChange=true;
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
		//拼接上下相邻的图
		case FU_UPDOWN:{
							this->fuseUD(code1,code2);
						    _lastFuseType=FU_UPDOWN;							
							break;
					   }
		//拼接四邻接图
		case FU_UDLR:{
							this->fuseUDLR(code1,code2);
							_lastFuseType=FU_UDLR;
							break;
					  }
	}
	return _imageFu;
}


CRect BlockImage::getFUImageRect(){
	 CPoint localPLT=_lastPoint-_gFuLTPoint;//视窗左上角
	 CPoint localPRD;
	 localPRD.SetPoint(localPLT.x+_viewRect.Width(),
						localPLT.y+_viewRect.Height());//视窗右下角
	 return CRect(localPLT,localPRD);
}

CPoint BlockImage::gPTolP(CPoint p){
	return p-_gFuLTPoint;
}

CImage* BlockImage::getImage(){
	return _imageFu;
}


CImage* BlockImage::getImageBackUp(){
	return _imageFuBack;
}