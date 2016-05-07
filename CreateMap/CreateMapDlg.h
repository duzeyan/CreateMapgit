
// CreateMapDlg.h : ͷ�ļ�
//

#pragma once
 
#include"QDib.h"
#include "afxwin.h"
#include "NJUSTMap.h"
#include"DrawMapMark.h"
#include "CreateLineDlg.h"
#include"CreateCrossDlg.h"

#include<math.h>

// CCreateMapDlg �Ի���
class CCreateMapDlg : public CDialog
{
// ����
public:
	CCreateMapDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CREATEMAP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


private:
	QDib qDib;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool m_isMove;			//����Ƿ����ƶ�
	CPoint m_startPoint;   //���������
	CImage *m_loadImage;   //���»��Ƶ�ͼƬ
	CImage *m_backUpImage; //ԭʼͼ�񱸷�
	CImage *m_canvas;	   //���� Ϊ�˼�¼���Ƶ㼯 
	//CImage *m_canvas1;	   //���� Ϊ�˼�¼���Ƶ㼯 

	CRect m_srcRect;       //ԭͼ�ϵ��Ӵ� 
	CRect m_picRect;       //ͼƬ�ؼ���С
	bool m_isDrawLine;     //�Ƿ���뻭״̬
	CPoint m_lineP;			//���ߵ���ʼ����յ�
	CPoint m_nodeP;         //����·��ʱ����Ҫ�ĵ�
	CDC *m_pPicDC;		 //ͼƬ�ؼ�DC
	vector<DRAW_RECORD> m_records; //��ͼ������¼��
	NJUSTMap m_njustMap;     //���ʹ�õĵ�ͼ���˽ṹ(����ʹ洢)

	CONTROL_CASE m_nowCase;
	CONTROL_BEZIER control_bezier; //���Ʊ���������ʱ ��¼���״̬
	CONTORL_POINTS control_points;//���ƹ�����ʱ ��¼���״̬

	CreateLineDlg *m_lineDlg;       //��·�Ӵ���
	CreateCrossDlg *m_crossDlg;  //·���Ӵ���s
	CPoint m_Line2ID ;				//x yΪֱ�����ߵ�ID

	CString m_curMapName;      //��ǰ�򿪵�ͼ������

	//�ؼ����
	CListBox m_listRecord;  //�滭��¼�б�
	CListBox m_listMap;    //��ͼ�ṹ�б�
	CStatusBarCtrl* m_statusBar;  //״̬��
	CList<CRect, CRect> m_listRect;  //�ؼ���С����

	////////////////////////////////////////////////////////////////////////// �����������
	vector<MAP_DOUBLE_POINT> m_Show_GPSList; //��ȡ��GFS
	int m_Show_cur; //��ǰ����������λ��
	//////////////////////////////////////////////////////////////////////////

	CStatic m_picMain;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedButtonup();
	afx_msg void OnBnClickedButtonleft();
	afx_msg void OnBnClickedButtonright();
	afx_msg void OnBnClickedButtonupleft();
	afx_msg void OnBnClickedButton2();

	afx_msg void OnBnClickedButnbezier();



//ҵ���߼� ���ܺ���
private :
	//��ֱ�߻�ͼ�ľ������
	//point:��ǰ������(ȫ����)  rect:�ؼ�����
	void DlgDrawLine(CPoint point,CRect rect);

	//�����߻�ͼ�ľ������
	//point:��ǰ������(ȫ����)  rect:�ؼ�����
	void DlgDrawBezier(CPoint point ,CRect rect);

	//����ɢ���ͼ�ľ������
	//point:��ǰ������(ȫ����)  rect:�ؼ�����
	void DlgDrawPoints(CPoint point ,CRect rect);

	//�Խڵ��ͼ�ľ������
	//point:��ǰ������(ȫ����)  rect:�ؼ�����
	void DlgDrawMark(CPoint point,CRect rect);

	//��ʾ�����ػ�
	void DlgReDraw();

	//���л�����
	void serial(CFile &file);

	//�����л�
	void enserial(CFile &file);

	//����Ƿ������ͼ
	bool isLoad();
	//��ʼ��״̬��
	void initStatusBar();

	

public:
	afx_msg void OnBnClickedButnpoints();
	afx_msg void OnBnClickedButtondel();
	afx_msg void OnBnClickedButtonshow();
	afx_msg void OnBnClickedButnmarknode();
	afx_msg void OnBnClickedButtonmeg();
protected:
	afx_msg LRESULT OnMapSetline2id(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLbnSelchangeListrecord();
	
	afx_msg void OnBnClickedButtonshowroad();
	afx_msg void OnBnClickedButtonf5();
	afx_msg void OnBnClickedButtondeline();
	afx_msg void OnBnClickedButtondelineout();
	afx_msg void OnBnClickedCancel();
	//afx_msg void OnBnClickedButtonshowgps();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonmeg2();
protected:
	afx_msg LRESULT OnMapSetcross2id(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonsavemap();
	afx_msg void OnBnClickedButtonloadmap();
	afx_msg void OnMenuBuildMap();
	afx_msg void OnOpenMap();
	afx_msg void OnSaveMap();
	afx_msg void OnMenuShowGPS();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
