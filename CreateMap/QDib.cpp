#include"stdafx.h"
#include"QDib.h"

//构造
QDib::QDib(){
	m_palDIB=NULL;
	m_hDIB=NULL;
}


//析构
QDib::~QDib(){
	DeleteDIB();
}

//加载位图
BOOL QDib::Load(LPCTSTR ipszFileName){
	CFile file;
	HDIB hDIB;
	CFileException fe;

	//open file
	if(!file.Open(ipszFileName,CFile::modeRead|CFile::shareDenyWrite,&fe)){
		AfxMessageBox(fe.m_cause);
		return FALSE;
	}

	//read file
	try{
		hDIB=ReadDIBFile(file);
	}catch(CFileException eLoad){
		file.Abort();
		return FALSE;
	}

	DeleteDIB();
	m_hDIB=hDIB;
	m_palDIB=new CPalette;
	//no palette
	if(CreateDIBPalette(m_hDIB,m_palDIB)==NULL){
		delete m_palDIB;
		m_palDIB=NULL;
	}
	return TRUE;
}

//
BOOL QDib::LoadFromResource(UINT nID){
	HINSTANCE hResInst=AfxGetResourceHandle();
	HRSRC hFindRes;
	HDIB hDIB;
	LPSTR pDIB;
	LPSTR pRes;
	HGLOBAL hRes;

	hFindRes=FindResource(hResInst,MAKEINTRESOURCE(nID),RT_BITMAP);
	if(hFindRes==NULL){
		return FALSE;
	}
	hRes=LoadResource(hResInst,hFindRes); //载入位图资源
	if(hRes==NULL)
		return FALSE;
	DWORD dwSize=SizeofResource(hResInst,hFindRes);
	hDIB=(HDIB)GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,dwSize);
	if(hDIB==NULL)
		return FALSE;
	pDIB=(LPSTR)GlobalLock((HGLOBAL)hDIB);
	pRes=(LPSTR)LockResource(hRes);
	memcpy(pDIB,pRes,dwSize);
	GlobalUnlock((HGLOBAL)hDIB);
	DeleteDIB();

	m_hDIB=hDIB;
	m_palDIB=new CPalette;
	if(CreateDIBPalette(m_hDIB,m_palDIB)==NULL){
		delete m_palDIB;
		m_palDIB=NULL;
	}
	return TRUE;
}

CPalette * QDib::GetPalette(){
	return m_palDIB;
}

BOOL QDib::Show(CDC *pDC,int x,int y,int cx,int cy){
	if(m_hDIB==NULL)
		return FALSE;

	CRect rDIB,rDest;
	rDest.left=x;
	rDest.top=y;
	if(cx==0||cy==0){
		cx=Width();
		cy=Height();
	}
	rDest.right=rDest.left+cx;
	rDest.bottom=rDest.top+cy;
	rDIB.left=rDIB.top=0;
	rDIB.right=Width();
	rDIB.bottom=Height();
	return PaintDIB(pDC->GetSafeHdc(),&rDest,m_hDIB,&rDIB,m_palDIB);
}

int QDib::Width(){
	if(m_hDIB==NULL)
		return 0;
	LPSTR lpDIB=(LPSTR)GlobalLock((HGLOBAL)m_hDIB);
	int cxDIB=(int)DIBWidth(lpDIB);
	GlobalUnlock((HGLOBAL)m_hDIB);
	return cxDIB;
}

int QDib::Height(){
	if(m_hDIB==NULL)
		return 0;
	LPSTR lpDIB=(LPSTR)GlobalLock((HGLOBAL)m_hDIB);
	int cyDIB=(int)DIBHeight(lpDIB);
	GlobalUnlock((HGLOBAL)m_hDIB);
	return cyDIB;
}

void QDib::DeleteDIB(){
	if(m_hDIB!=NULL)
		GlobalFree((HGLOBAL)m_hDIB);
	if(m_palDIB!=NULL)
		delete m_palDIB;
}