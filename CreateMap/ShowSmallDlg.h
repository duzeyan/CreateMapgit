#pragma once

#include "MAP_BASIC_data.h"
#include "DrawMapMark.h"
#include <vector>
using std::vector;
// ShowSmallDlg �Ի���

class ShowSmallDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ShowSmallDlg)

public:
	ShowSmallDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ShowSmallDlg();

// �Ի�������
	enum { IDD = IDD_LOCATION_DIALOG };

private:
	int _srcW;//ԭͼ��
	int _srcH;//ԭͼ��
	int _srcBlockW;//ԭͼ��ȷֿ�
	int _srcBlockH;//ԭͼ�߶ȷֿ�
	int _nowIndexX; //��ͼ�п������
	int _nowIndexY;
	bool _isTransPath; //·�ߵ㼯��ת����
	bool _isTransTask; //·�ߵ㼯��ת����
	vector<CPoint> _pathList;//·�ߵ㼯
	vector<MAP_TASK_NODE_ZZ> _tasks;
	CImage _smallImage;
	CRect  _picRect;//ͼƬ�ؼ���С
	CDC *_picDC;//ͼƬ�ؼ�DC
	CRect _drawRect; //���������С
public :
	void init(CString imagepath);

	//����ԭͼ�ĸ߿� �ֿ����
	void setSrcImageInfo(int width,int height,int blockW,int blockH);

	//�������ڵ�λ�� ������ͼ��ͻ���ӿ�
	void setPosition(CPoint p);

	//���ý��·����
	void setPathPix(vector<CPoint> &pixs);

	//��������·������
	void setTaskPix(vector<MAP_TASK_NODE_ZZ> &tasks);

private:
	//����ͼƬ��С��ͼƬ�ؼ���С
	CRect getDrawRect();
	//�����ָ���
	void drawCorr();
	//������ǰ��
	void higthLightBlock();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
};
