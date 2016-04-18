#ifndef QDIB_H
#define QDIB_H

#include"stdafx.h"
#include"DIBAPI.H"
class QDib{

public:
	QDib();
	~QDib();
private:
	HDIB m_hDIB;
	CPalette *m_palDIB;

public:
	BOOL Load(LPCTSTR lpszFileName);
	BOOL LoadFromResource(UINT nID);
	CPalette *GetPalette();
	BOOL Show(CDC* pDC,int x,int y,int cx=0,int cy=0);
	int Width();
	int Height();
	void DeleteDIB();
};

#endif