#pragma once

#include "NJUSTMap.h"
#include"DrawMapMark.h"
#include "afxwin.h"
#include <iostream>
#include "resource.h"
using namespace std;

// CreateLineDlg 对话框
//本类有两个用处 

//1.用来构建一条道路信息(创建模式)  使用Initialize


//2.用于修改一条道路的信息(修改模式) 此时只需要简单的保存属性即可


class CreateLineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateLineDlg)

public:
	CreateLineDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CreateLineDlg();

// 对话框数据
	enum { IDD = IDD_CREATELINE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	vector<int> m_nodesIndex;  //路口编号序列
	CComboBox m_startBox;   //头部列表
	CComboBox m_endBox;     //尾部列表
	bool m_isModify;        //是否是修改模式 不是则为创建模式
	MAP_ROAD m_node;        //用于存储接原始结构的数据
public:
	//设置必要参数 节点序列
	void Initialize(vector<int> nodes);

	void InitializeView();		 //初始化视窗参数

	bool packageMAP_ROAD(MAP_ROAD *package); //打包数据

	void setModify(const MAP_ROAD &package);    //使用修改模式 ，需要传入原来的信息

	

	afx_msg void OnBnClickedLinedlgOk();
};
