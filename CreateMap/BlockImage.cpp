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
		delete[] _imageLT;//����ͼƬ
	if(_imageRT!=NULL)
		delete[] _imageRT;//����ͼƬ
	if(_imageLB!=NULL)
		delete[] _imageLB;//����ͼƬ
	if(_imageRB!=NULL)
		delete[] _imageRB;//����ͼƬ
	if(_imageFu!=NULL)
		delete[] _imageFu;//ƴ��ͼƬ
	if(_imageFuBack!=NULL)
		delete[] _imageFuBack;//ƴ��ͼƬ����
	_imageLT=_imageRT=_imageLB=_imageRB=_imageFu=_imageFuBack=NULL;
}

//TODO 7.1ǰ����XML����
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
		sline>>x; //������ʱstring
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

//д�����ļ� strΪȫ·��
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



void BlockImage::getFuseLRRect(CRect &rectL,CRect &rectR,CRect &rectFL,CRect &rectFR){
	CPoint p1,p2; //�������þ��� p1Ϊ�������ϵ�,p2Ϊ�������µ�
	//ע��:CImage::Draw���еľ��β���,����ʱ������ƾ��ε��ұߺ��±�
	//��������ѡȡ�ľ������ұߺ�������һ������

	//Step 1 ------------��ͼ��ȡ����-------------
	p1.SetPoint(_blockW-_viewRect.Width(),0);
	p2.SetPoint(_blockW,_blockH);
	rectL.SetRect(p1,p2);

	//Step 2 -----------��ͼ��ȡ����,ƴ��ͼ���߽�ȡ����--------------
	p1.SetPoint(0,0);
	p2.SetPoint(_viewRect.Width(),_blockH);
	rectR.SetRect(p1,p2);
	rectFL.SetRect(p1,p2);

	//Step 3 -----------ƴ��ͼ�Ұ�߽�ȡ����--------------
	p1.SetPoint(_viewRect.Width(),0);
	p2.SetPoint(_viewRect.Width()*2,_blockH);
	rectFR.SetRect(p1,p2);
}

//��ȡFU_UPDOWN ���������Ҫ��ƴ�Ӿ���
void BlockImage::getFuseUDRect(CRect &rectU,CRect &rectD,CRect &rectFU,CRect &rectFD){
	CPoint p1,p2; //�������þ��� p1Ϊ�������ϵ�,p2Ϊ�������µ�

	//Step 1 ------------��ͼ��ȡ����-------------
	p1.SetPoint(0,_blockH-_viewRect.Height());
	p2.SetPoint(_blockW,_blockH);
	rectU.SetRect(p1,p2);

	//Step 2 -----------��ͼ��ȡ����,ƴ��ͼ���ϰ��--------------
	p1.SetPoint(0,0);
	p2.SetPoint(_blockW,_viewRect.Height());
	rectD.SetRect(p1,p2);
	rectFU.SetRect(p1,p2);

	//Step 3 -----------ƴ��ͼ�°�߽�ȡ����--------------
	p1.SetPoint(0,_viewRect.Height());
	p2.SetPoint(_blockW,_viewRect.Height()*2);
	rectFD.SetRect(p1,p2);
}

void BlockImage::getFuseUDLRRect(CRect rect[4],CRect rectF[4]){
	CPoint p1,p2; //�������þ��� p1Ϊ�������ϵ�,p2Ϊ�������µ�

	//Step 1 ------------LT��ȡ����-------------
	p1.SetPoint(_blockW-_viewRect.Width(),_blockH-_viewRect.Height());
	p2.SetPoint(_blockW,_blockH);
	rect[0].SetRect(p1,p2);

	//Step 2 ------------RT��ȡ����-------------
	p1.SetPoint(0,_blockH-_viewRect.Height());
	p2.SetPoint(_viewRect.Width(),_blockH);
	rect[1].SetRect(p1,p2);

	//Step 3 ------------LB��ȡ����-------------
	p1.SetPoint(_blockW-_viewRect.Width(),0);
	p2.SetPoint(_blockW,_viewRect.Height());
	rect[2].SetRect(p1,p2);

	//Step 4 ------------RB��ȡ����,LTƴ��ͼ��ȡ����-------------
	p1.SetPoint(0,0);
	p2.SetPoint(_viewRect.Width(),_viewRect.Height());
	rect[3].SetRect(p1,p2);
	rectF[0].SetRect(p1,p2);

	//Step 5 ------------RTƴ��ͼ��ȡ����-------------
	p1.SetPoint(_viewRect.Width(),0);
	p2.SetPoint(_viewRect.Width()*2,_viewRect.Height());
	rectF[1].SetRect(p1,p2);

	//Step 6 ------------LBƴ��ͼ��ȡ����-------------
	p1.SetPoint(0,_viewRect.Height());
	p2.SetPoint(_viewRect.Width(),_viewRect.Height()*2);
	rectF[2].SetRect(p1,p2);

	//Step 7 ------------RBƴ��ͼ��ȡ����-------------
	p1.SetPoint(_viewRect.Width(),_viewRect.Height());
	p2.SetPoint(_viewRect.Width()*2,_viewRect.Height()*2);
	rectF[3].SetRect(p1,p2);
	return;
}


void BlockImage::fuseNone(unsigned int code){
	CString imagefilename;
	unsigned int indexH=(code-1)/_blockWNum; //������ͼ�е��к� ��0��ʼ
	unsigned int indexW=(code-1)%_blockWNum; //������ͼ�е��к� ��0��ʼ

	//Step 1 -----------�������Ƿ�Ϊ��--------------

	if(_imageFu==NULL)
		_imageFu=new(std::nothrow) CImage();
	if(_imageFuBack==NULL)
		_imageFuBack=new(std::nothrow) CImage();
	assert(_imageFu!=NULL);
	assert(_imageFuBack!=NULL);

	//Step 2 -----------����ͼƬ�ļ�--------------
	_imageFu->Destroy();//�����ϴ洢�Ĳ�ͬλͼ
	_imageFuBack->Destroy();

	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		code); //ƴ���ļ�����·��
	_imageFu->Load(imagefilename); //ֱ����Ϊ�ں�ͼƬ
	_imageFuBack->Load(imagefilename);
	//ƴ��ͼ���Ͻ���ȫ��ͼ�е�����
	_gFuLTPoint.SetPoint(indexW*_blockW,indexH*_blockH);
	return;
}

void BlockImage::fuseLR(unsigned int codeL,unsigned int codeR){
	CString imagefilename;
	HDC hdcFU=NULL;
	CRect rectL,rectR,rectFL,rectFR; //���ҿ�ͼ�Ľ�ȡ���� ƴ��ͼ���ҽ�ȡ����
	unsigned int indexH=(codeL-1)/_blockWNum; //������ͼ�е��к� ��0��ʼ
	unsigned int indexW=(codeL-1)%_blockWNum; //������ͼ�е��к� ��0��ʼ
	int FUWidth=0;


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
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeL);
	_imageLT->Load(imagefilename);

	//������ͼ
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
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
	_imageFuBack->ReleaseDC();

	//Step 5 -----------ƴ��ͼ���Ͻ���ȫ��ͼ�е�����--------------
	FUWidth=indexW*_blockW+(_blockW-_viewRect.Width());
	_gFuLTPoint.SetPoint(FUWidth,indexH*_blockH);
	return;
}

void BlockImage::fuseUD(unsigned int codeU,unsigned int codeD){
	CString imagefilename;
	HDC hdcFU=NULL;
	CRect rectU,rectD,rectFU,rectFD; //���¿�ͼ�Ľ�ȡ���� ƴ��ͼ���½�ȡ����
	unsigned int indexH=(codeU-1)/_blockWNum; //���ڼ���ƴ��ͼ���Ͻ�����
	unsigned int indexW=(codeU-1)%_blockWNum; 
	int FUHeight=0;

	//Step 1 -----------�������Ƿ�Ϊ��,������ֵ--------------
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

	//Step 2 -----------�ͷ���һ�μ��ص�ͼƬ��Դ--------------
	_imageLT->Destroy();
	_imageLB->Destroy();
	_imageFu->Destroy();
	_imageFuBack->Destroy();

	//Step 3 -----------������ͼ����ͼ--------------
	//������ͼ
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeU);
	_imageLT->Load(imagefilename);

	//������ͼ
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeD);
	_imageLB->Load(imagefilename);

	//Step 4 -----------ƴ������ͼ--------------
	_imageFu->Create(_imageLT->GetWidth(),_viewRect.Height()*2,24);//����ƴ��ͼƬ��С
	_imageFuBack->Create(_imageLT->GetWidth(),_viewRect.Height()*2,24);//����ƴ��ͼƬ��С
	getFuseUDRect(rectU,rectD,rectFU,rectFD);//��ȡ��ȡ����

	//����ƴ��ͼ
	hdcFU=_imageFu->GetDC();
	_imageLT->Draw(hdcFU,rectFU,rectU);	//�ϰ��
	_imageLB->Draw(hdcFU,rectFD,rectD);	//�°��	
	_imageFu->ReleaseDC();

	//���Ʊ���ͼ
	hdcFU=_imageFuBack->GetDC();
	_imageLT->Draw(hdcFU,rectFU,rectU);	//�ϰ��
	_imageLB->Draw(hdcFU,rectFD,rectD);	//�°��	
	_imageFuBack->ReleaseDC();

	//Step 5 -----------ƴ��ͼ���Ͻ���ȫ��ͼ�е�����--------------
	FUHeight=indexH*_blockH+(_blockH-_viewRect.Height());
	_gFuLTPoint.SetPoint(indexW*_blockW,FUHeight);
	return;
}

void BlockImage::fuseUDLR(unsigned int codeLT,unsigned int codeRD){
	CString imagefilename;
	HDC hdcFU=NULL;
	CRect rect[4],rectF[4]; //���¿�ͼ�Ľ�ȡ���� ƴ��ͼ���½�ȡ����
	unsigned int codeRT=codeLT+1;
	unsigned int codeLD=codeRD-1;
	unsigned int indexH=(codeLT-1)/_blockWNum; //���ڼ���ƴ��ͼ���Ͻ�����
	unsigned int indexW=(codeLT-1)%_blockWNum; 
	int FUHeight,FUWidth;

	//Step 1 -----------�������Ƿ�Ϊ��,������ֵ--------------
	

	//Step 2 -----------�ͷ���һ�μ��ص�ͼƬ��Դ--------------
	_imageLT->Destroy();
	_imageLB->Destroy();
	_imageRB->Destroy();
	_imageRT->Destroy();
	_imageFu->Destroy();
	_imageFuBack->Destroy();

	//Step 3 -----------������ͼ����ͼ--------------
	//����1ͼ
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeLT);
	_imageLT->Load(imagefilename);

	//����2ͼ
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeRT);
	_imageRT->Load(imagefilename);

	//����3ͼ
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeLD);
	_imageLB->Load(imagefilename);

	//����4ͼ
	imagefilename.Format(L"%s\\%s (%d).png",_imagesDir,
		_imageName,
		codeRD);
	_imageRB->Load(imagefilename);

	//Step 4 -----------ƴ������ͼ--------------
	_imageFu->Create(_viewRect.Width()*2,_viewRect.Height()*2,24);//����ƴ��ͼƬ��С
	_imageFuBack->Create(_viewRect.Width()*2,_viewRect.Height()*2,24);//����ƴ��ͼƬ��С
	getFuseUDLRRect(rect,rectF);

	//����ƴ��ͼ
	hdcFU=_imageFu->GetDC();
	_imageLT->Draw(hdcFU,rectF[0],rect[0]);	//1
	_imageRT->Draw(hdcFU,rectF[1],rect[1]);	//2
	_imageLB->Draw(hdcFU,rectF[2],rect[2]);	//3
	_imageRB->Draw(hdcFU,rectF[3],rect[3]);	//4
	_imageFu->ReleaseDC();

	//���Ʊ���ͼ
	hdcFU=_imageFuBack->GetDC();
	_imageLT->Draw(hdcFU,rectF[0],rect[0]);	//1
	_imageRT->Draw(hdcFU,rectF[1],rect[1]);	//2
	_imageLB->Draw(hdcFU,rectF[2],rect[2]);	//3
	_imageRB->Draw(hdcFU,rectF[3],rect[3]);	//4	
	_imageFuBack->ReleaseDC();

	//Step 5 -----------ƴ��ͼ���Ͻ���ȫ��ͼ�е�����--------------
	FUWidth=indexW*_blockW+(_blockW-_viewRect.Width());
	FUHeight=indexH*_blockH+(_blockH-_viewRect.Height());
	_gFuLTPoint.SetPoint(FUWidth,FUHeight);
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
	if(_isFirst||_reDraw){
		_isFirst=false;
	}else{
		if(_imageFu!=NULL&&(_lastFuseType==_curFuseType)){ //״̬δ��,���Ѿ���������ֱ�ӷ�����һ�εĽ��
			_isStateChange=false;
			return _imageFu;
		}
	}

	_isStateChange=true;
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
		//ƴ���������ڵ�ͼ
		case FU_UPDOWN:{
							this->fuseUD(code1,code2);
						    _lastFuseType=FU_UPDOWN;							
							break;
					   }
		//ƴ�����ڽ�ͼ
		case FU_UDLR:{
							this->fuseUDLR(code1,code2);
							_lastFuseType=FU_UDLR;
							break;
					  }
	}
	return _imageFu;
}


CRect BlockImage::getFUImageRect(){
	 CPoint localPLT=_lastPoint-_gFuLTPoint;//�Ӵ����Ͻ�
	 CPoint localPRD;
	 localPRD.SetPoint(localPLT.x+_viewRect.Width(),
						localPLT.y+_viewRect.Height());//�Ӵ����½�
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