#include "stdafx.h"
#include "BlockImage.h"
#include <fstream>
#include <sstream>

void BlockImage::init(CRect viewRect){
	_viewRect=viewRect;
	_imageLT=NULL;//����ͼƬ
	_imageRT=NULL;//����ͼƬ
	_imageLB=NULL;//����ͼƬ
	_imageRB=NULL;//����ͼƬ
	_imageFu=NULL;//ƴ��ͼƬ
	_imageFuBack=new(std::nothrow) CImage();//ƴ��ͼƬ����
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
		sline>>x; //������ʱstring

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
	//Step 1 -------------��ȡ�����������ͼƬ����------------
	code1=getImageNum(p1);
	code2=getImageNum(p2);

	//Step 2 -----------���ݵ�������ж��ں�����--------------
	_lastFuseType=_curFuseType;
	if(code1==code2){  //��������ͬ
		_curFuseType=FU_NONE;
		return ;
	}
	if(code1+1==code2){//��������������
		_curFuseType=FU_LEFTRIGHT;
		return ;
	}
	if(code1+_blockWNum==code2){//��������������
		_curFuseType=FU_UPDOWN;
		return;
	}
	if(code1+_blockWNum+1==code2){//�����ŶԽ�������
		_curFuseType=FU_UDLR;
		return;
	}

	//û���������
}

void BlockImage::fuseNone(unsigned int code){
	//Step 1 -----------�������Ƿ�Ϊ��--------------
	CString imagefilename;
	if(_imageFu==NULL)
		_imageFu=new(std::nothrow) CImage();
	if(_imageFuBack==NULL)
		_imageFuBack=new(std::nothrow) CImage();
	assert(_imageFuBack!=NULL);

	//Step 2 -----------����ͼƬ�ļ�--------------
	_imageFu->Destroy();//�����ϴ洢�Ĳ�ͬλͼ
	imagefilename.Format(L"%s//%s(%d).png",_imagesDir,
		_imageName,
		code); //ƴ���ļ�����·��
	_imageFu->Load(imagefilename); //ֱ����Ϊ�ں�ͼƬ
	_imageFuBack->Load(imagefilename);

	return;
}


void BlockImage::getFuseLRRect(CRect &rectL,CRect &rectR,CRect &rectFL,CRect &rectFR){
	CPoint p1,p2; //�������þ��� p1Ϊ�������ϵ�,p2Ϊ�������µ�

	//Step 1 ------------��ͼ��ȡ����-------------
	p1.SetPoint(_blockW-_viewRect.Width(),0);
	p2.SetPoint(_blockW-1,_blockH-1);
	rectL.SetRect(p1,p2);

	//Step 2 -----------��ͼ��ȡ����,ƴ��ͼ���߽�ȡ����--------------
	p1.SetPoint(0,0);
	p2.SetPoint(_viewRect.Width()-1,_blockH-1);
	rectR.SetRect(p1,p2);
	rectFL.SetRect(p1,p2);

	//Step 3 -----------ƴ��ͼ�Ұ�߽�ȡ����--------------
	p1.SetPoint(_viewRect.Width(),0);
	p2.SetPoint(_viewRect.Width()*2-1,_blockH-1);
	rectFR.SetRect(p1,p2);
}

void BlockImage::fuseLR(unsigned int codeL,unsigned int codeR){
	CString imagefilename;
	HDC hdcFU=NULL;
	CRect rectL,rectR,rectFL,rectFR; //���ҿ�ͼ�Ľ�ȡ���� ƴ��ͼ���ҽ�ȡ����

	//Step 1 -----------�������Ƿ�Ϊ��,������ֵ--------------
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

	//Step 2 -----------�ͷ���һ�μ��ص�ͼƬ��Դ--------------
	_imageLT->Destroy();
	_imageRT->Destroy();
	_imageFu->Destroy();
	_imageFuBack->Destroy();

	//Step 3 -----------������ͼ����ͼ--------------
	//������ͼ
	imagefilename.Format(L"%s//%s(%d).png",_imagesDir,
		_imageName,
		codeL);
	_imageLT->Load(imagefilename);

	//������ͼ
	imagefilename.Format(L"%s//%s(%d).png",_imagesDir,
		_imageName,
		codeR);
	_imageRT->Load(imagefilename);

	//Step 4 -----------ƴ������ͼ--------------
	_imageFu->Create(_viewRect.Width()*2,_imageLT->GetHeight(),24);//����ƴ��ͼƬ��С
	_imageFuBack->Create(_viewRect.Width()*2,_imageLT->GetHeight(),24);//����ƴ��ͼƬ��С
	getFuseLRRect(rectL,rectR,rectFL,rectFR);//��ȡ��ȡ����
	
	//����ƴ��ͼ
	hdcFU=_imageFu->GetDC();
	_imageLT->Draw(hdcFU,rectFL,rectL);	//����
	_imageRT->Draw(hdcFU,rectFR,rectR);	//�Ұ��	
	_imageFu->ReleaseDC();

	//���Ʊ���ͼ
	hdcFU=_imageFuBack->GetDC();
	_imageLT->Draw(hdcFU,rectFL,rectL);	//����
	_imageRT->Draw(hdcFU,rectFR,rectR);	//�Ұ��	
	_imageFu->ReleaseDC();

	return;
}

CImage* BlockImage::getImage(CPoint p){
	CPoint endPoint;
	unsigned int code1,code2;//�������ڿ�ͼ�еı��
	endPoint.SetPoint(p.x+_viewRect.Width()-1,p.y+_viewRect.Height()-1);

	//Step 1 -----------��ȡƴ������--------------
	_lastPoint=p;//��¼��������
	this->computeTwoPointsType(p,endPoint,code1,code2);

	//Step 2 -----------����ƴ�����ͷ���ͼ��--------------
	if(_imageFu!=NULL&&_lastFuseType==_curFuseType) //״̬δ��,���Ѿ���������ֱ�ӷ�����һ�εĽ��
		return _imageFu;

	switch(_curFuseType){
		//����Ҫƴ��
		case FU_NONE:	{   
							this->fuseNone(code1);//code2==code1
							_lastFuseType=FU_NONE;
							break;
						 }
		//ƴ���������ڵ�ͼ
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