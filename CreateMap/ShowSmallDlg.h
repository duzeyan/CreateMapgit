#pragma once


// ShowSmallDlg 对话框

class ShowSmallDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ShowSmallDlg)

public:
	ShowSmallDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ShowSmallDlg();

// 对话框数据
	enum { IDD = IDD_LOCATION_DIALOG };

private:
	int _srcW;//原图高
	int _srcH;//原图宽
	int _srcBlockW;//原图宽度分块
	int _srcBlockH;//原图高度分块
	int _nowIndexX; //大图中块的索引
	int _nowIndexY;
	CImage _smallImage;
	CRect  _picRect;//图片控件大小
	CDC *_picDC;//图片控件DC
	CRect _drawRect; //绘制区域大小
public :
	void init(CString imagepath);

	//设置原图的高宽 分块个数
	void setSrcImageInfo(int width,int height,int blockW,int blockH);

	//给出现在的位置 在缩略图中突出子块
	void setPosition(CPoint p);



private:
	//根据图片大小和图片控件大小
	CRect getDrawRect();

	//画出分割线
	void drawCorr();
	//高亮当前块
	void higthLightBlock();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};
