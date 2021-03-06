﻿
// CreateMapDlg.h : 头文件
//

#pragma once
 
#include"QDib.h"
#include "afxwin.h"
#include "NJUSTMap.h"
#include"DrawMapMark.h"
#include "CreateLineDlg.h"
#include"CreateCrossDlg.h"
#include "ModifyNode.h"
#include "SetNameDlg.h"
#include "CreateMapCommunication.h"
#include "resource.h"
#include<math.h>
#include"map_mysql_data.h"
#include "SelectMapDlg.h"
#include "BlockImage.h"
#include "ShowSmallDlg.h"

// CCreateMapDlg 对话框
class CCreateMapDlg : public CDialog
{
// 构造
public:
	CCreateMapDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CREATEMAP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


private:
	QDib qDib;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool m_isMove;			//鼠标是否在移动
	CPoint m_startPoint;   //左键启动点
	//CImage *m_loadImage;   //最新绘制的图片
	//CImage *m_backUpImage; //原始图像备份
	BlockImage m_blockImage;//分块读取图片
	//CImage *m_canvas1;	   //画布 为了记录绘制点集 
	bool m_RecordGPS;

	CRect m_viewRect;       //原图上的视窗 
	CRect m_picRect;       //图片控件大小
	bool m_isDrawLine;     //是否进入画状态
	CPoint m_lineP;			//画线的起始点和终点
	CPoint m_nodeP;         //绘制路口时候需要的点
	CPoint m_obP;           //绘制障碍物时候的标记点
	CDC *m_pPicDC;		 //图片控件DC
	vector<DRAW_RECORD> m_records; //绘图操作记录表
	NJUSTMap m_njustMap;     //最后使用的地图拓扑结构(计算和存储)

	CONTROL_CASE m_nowCase;         //鼠标点击当前状态
	CONTROL_BEZIER control_bezier; //绘制贝塞尔曲线时 记录鼠标状态
	CONTORL_POINTS control_points;//绘制孤立点时 记录鼠标状态

	CreateLineDlg *m_lineDlg;       //道路子窗口
	CreateCrossDlg *m_crossDlg;  //路口子窗口s
	ModifyNode *m_nodeDlg;         //修改节点属性的子窗口
	SelectMapDlg *m_dbDlg;          //选择地图子窗口
	SetNameDlg   *m_nameDlg;      //设置名字子窗口
	ShowSmallDlg *m_smallImgDlg;     //设置子对话框
	CPoint m_Line2ID ;				//x y为直线两边的ID

	CString m_curMapFullPath;      //当前地图的全路径

	//控件相关
	CListBox m_listRecord;  //绘画记录列表
	CListBox m_listMap;    //地图结构列表
	CStatusBarCtrl* m_statusBar;  //状态栏
	CList<CRect, CRect> m_listRect;  //控件大小调整

	CreateMapCommunication m_getMCInfo;  //GPS通信
	MAP_DOUBLE_POINT m_RealGPS;         //从惯导获取的GPS
	unsigned int m_clockGPS;			//控制接受频率
	bool m_isDrawCar;                   //控制绘制车体
	bool m_isHighLight;    

	//数据库
	MysqlConnector m_dbcon;             //数据库连接
    DB_STATE m_dbstate;                 //当前数据库状态

	//////采集GPS
	bool m_isStartGPS;//标志是否记录
	//实时车采,记录,剪切三种方法得到
	CPoint m_cutPoint;//默认(0,0),剪切的第一个点
	vector<MAP_DOUBLE_POINT> m_realGPSList;//用于赋给道路或路口的GPS序列
	vector<CPoint> m_pathKeep;//记录绘制车体,刷新动作取消
	////////////////////////////////////////////////////////////////////////// 显示
	vector<MAP_DOUBLE_POINT> m_Show_GPSList; //读取规划结果文件
	vector<MAP_TASK_NODE_ZZ> m_taskPoint;//原始任务路点文件
	vector<MAP_TASK_NODE_ZZ> m_taskShow;//用于在大图中显示任务文件结果
	int m_Show_cur; //当前读到的序列位置
	//////////////////////////////////////////////////////////////////////////

	CStatic m_picMain;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonup();
	afx_msg void OnBnClickedButtonleft();
	afx_msg void OnBnClickedButtonright();
	afx_msg void OnBnClickedButtonupleft();
	afx_msg void OnBnClickedButton2();

	afx_msg void OnBnClickedButnbezier();



//业务逻辑 功能函数
private :
	//对直线绘图的具体操作
	//point:当前点坐标(全窗体)  rect:控件坐标
	void DlgDrawLine(CPoint point,CRect rect);

	//对曲线绘图的具体操作
	//point:当前点坐标(全窗体)  rect:控件坐标
	void DlgDrawBezier(CPoint point ,CRect rect);

	//对离散点绘图的具体操作
	//point:当前点坐标(全窗体)  rect:控件坐标
	void DlgDrawPoints(CPoint point ,CRect rect);

	//对节点绘图的具体操作
	//point:当前点坐标(全窗体)  rect:控件坐标
	void DlgDrawMark(CPoint point,CRect rect);

	//绘制障碍物s
	void DlgDrawObstacle(CPoint point ,CRect rect);

	//显示窗体重绘
	void DlgReDraw();

	//序列化数据
	void serial(CFile &file);

	//反序列化
	void enserial(CFile &file);
	
	//设置标定数据 
	void setCalibration(CPoint p,CRect rect,int index);

	//计算误差
	void coumputerDevication(CPoint p,CRect rect);

	//动态调整控件位置 
	void initCtlPosition();

	//初始化状态栏
	void initStatusBar();

	//检查是否载入地图
	bool isLoad();

	//绘制车体
	void drawMyCar(double longlat[2]);

public:
	void showNowGPS(char *buff,long len);
	

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
	afx_msg void OnSaveAsMap();
	afx_msg void OnMenuShowGPS();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuP1();
	afx_msg void OnMenuP2();
	afx_msg void OnMenuP3();
	afx_msg void OnMenuDeviation();
	afx_msg void OnDestroy();
	afx_msg void OnResolution();
	afx_msg void OnMenuDrawcar();
	afx_msg void OnBnClickedBtnob();
	afx_msg void OnMenuPro1();
	afx_msg void OnSaveLinux();
protected:
	afx_msg LRESULT OnMapModifyLine(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNodepro();
protected:
	afx_msg LRESULT OnMapModifNode(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSavethemap();
	afx_msg void OnMenuShowall();
	afx_msg void OnSaveMapTask();
	afx_msg void OnRecordgps();
	afx_msg void OnCondb();
protected:
	afx_msg LRESULT OnMapSeldbmap(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnCreatedbmap();
	afx_msg void OnSaveAsdbmap();
protected:
	afx_msg LRESULT OnMapSetname(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSaveDBMap();
	afx_msg void OnBnClickedButton3();
protected:
	afx_msg LRESULT OnMapGetgps(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMapLocation(WPARAM wParam, LPARAM lParam);
public:
	//CComboBox m_comboxSelectEle;//地图结构列表(用于给地图元素赋GPS序列)
	afx_msg void OnGiveGPS();
	afx_msg void OnBnClickedButtonsavegps();
	afx_msg void OnBnClickedButtonreadgps();
	//CButton m_radioPN;
	int m_radioPN;
	afx_msg void OnBnClickedRadioP();
	afx_msg void OnBnClickedRadioN();
	afx_msg void OnShowtask();
	afx_msg void OnBnClickedButtonrshowreadgps();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
