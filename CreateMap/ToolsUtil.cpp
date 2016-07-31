#include "stdafx.h"
#include "ToolsUtil.h"
#include "MAP_BASIC_data.h"
#include <iostream>
#include <fstream>
#include<math.h>
#include<string>
#include<sstream>
#include<string.h>

using namespace std;

//ӳ��·����ֵ
const double FIND_NODE_THRE=50.0;
//ӳ���յ���ֵ
const double FIND_END_THRE=5000.0;

ToolsUtil::ToolsUtil(void)
{
}


ToolsUtil::~ToolsUtil(void)
{
}


//��ֱ��ģ�����޲��ϲ�
void ToolsUtil::CompleGPS(vector<MAP_DOUBLE_POINT> &GPSList){
	int preLen=2;//ȡǰ2������Ϊ����
	double dx,dy,dt,dt1,dtSum;
	bool isV;//������ʱ����ˮƽ
	if(GPSList.size()<preLen)
		return;
	dx=dy=0.0;
	//Step 1 -----------��������--------------
	for(int i=0;i<preLen-1;i++){
		dx+=(GPSList[i+1].x-GPSList[i].x);
		dy+=(GPSList[i+1].y-GPSList[i].y);
	}
	dx/=(preLen-1);
	dy/=(preLen-1);
	if(abs(dx)>abs(dy)){
		dt=dx;
		isV=true;
	}else{
		dt=dy;
		isV=false;
	}
	//Step 2 -----------����ȱ��--------------
	int cutI=-1;//ȱ������
	for(int i=preLen-1;i<GPSList.size()-1;i++){
		if(isV){
			dt1=GPSList[i+1].x-GPSList[i].x;
		}else{
			dt1=GPSList[i+1].y-GPSList[i].y;
		}
		if(abs(dt1)>abs(10*dt)){
			cutI=i+1;//�ϵ����
			break;
		}
	}
	if(cutI==0)
		return;
	//Step 3 -----------��������--------------
	dx=dy=0.0;
	for(int i=0;i<cutI-1;i++){
		dx+=(GPSList[i+1].x-GPSList[i].x);
		dy+=(GPSList[i+1].y-GPSList[i].y);
	}
	assert(cutI!=0);
	dx/=(cutI-1);
	dy/=(cutI-1);
	if(abs(dx)>abs(dy)){
		dt=dx;
		isV=true;
	}else{
		dt=dy;
		isV=false;
	}

	//Step 4 -----------���ȱ��--------------
	MAP_DOUBLE_POINT p=GPSList[cutI-1];
	for(dtSum=0;dtSum<abs(dt1);cutI,dtSum+=abs(dt)){
		p.x+=dx;p.y+=dy;
		GPSList.insert(GPSList.begin()+cutI,p);//ǰ����
	}
}

void ToolsUtil::GPS2Earthy(double x, double y, int &earthx, int &earthy)
{
	int n, L0;
	double X, N54, W54, t, m, a54, e54, e_54;
	double iptr;
	double t_2 = 0, t_4 = 0, yita_2 = 0, yita_4 = 0;
	double lp = 0, lp_2 = 0;
	double SinL, CosL, CosL_2, SinL_2;
	double SinG, CosG;
	double daa, df, db2p, dl2p;
    //double	dahm;
	double deltabo, deltalo;
	double w84, n84, m84, a84, e842, f84, f54, dx, dy, dz;
	double lati, logi;
	double pi = 3.1415926535;

	lati = x;
	logi = y;
	lati = lati*pi / 180;
	logi = logi*pi / 180;
	SinL = sin(lati);
	CosL = cos(lati);
	SinG = sin(logi);
	CosG = cos(logi);
	CosL_2 = CosL * CosL;
	SinL_2 = SinL * SinL;
	a84 = 6378137.0;
	e842 = 0.00669437999014132;
	f84 = 1.0 / 298.257223563;
	a54 = 6378245.0;
	f54 = 1.0 / 298.3;
	dx = -16.0;
	dy = 147.0;
	dz = 77.0;
	w84 = sqrt(1 - e842*SinL_2);
	n84 = a84 / w84;
	m84 = a84*(1 - e842) / (w84*w84*w84);
	daa = a54 - a84;
	df = f54 - f84;
	db2p = (-dx*SinL*CosG - dy*SinL*SinG + dz*CosL + (a84*df + f84*daa)*sin(2 * lati)) / (m84*sin(1 / 3600.0*pi / 180));
	dl2p = (-dx*SinG + dy*CosG) / (n84*CosL*sin(1 / 3600.0*pi / 180));
	//dahm = dx*CosL*CosG + dy*CosL*SinG + dz*SinL + (a84*df + f84*daa)*SinL_2 - daa;
	deltabo = (db2p / 3600.0)*pi / 180.0;
	deltalo = (dl2p / 3600.0)*pi / 180.0;
	logi = logi + deltalo;
	lati = lati + deltabo;
	SinL = sin(lati);
	CosL = cos(lati);
	CosL_2 = CosL * CosL;
	SinL_2 = SinL * SinL;
	a54 = 6378245.0;
	e54 = 0.0066934274898192;
	W54 = sqrt(1.0 - e54*SinL_2);
	N54 = a54 / W54;
	e_54 = 0.0067385254147;
	logi = logi * 180 / pi;
	modf(logi / 6.0, &iptr);
	n = (int)iptr + 1;
	L0 = n * 6 - 3;
	lp = (logi - L0)*pi / 180;
	lp_2 = lp*lp;
	m = CosL_2*lp_2;
	yita_2 = e_54*CosL_2;
	yita_4 = yita_2 * yita_2;
	t = tan(lati);
	t_2 = t*t;
	t_4 = t_2*t_2;
	double gps_xx, gps_yy;
	X = 111134.8611*lati * 180 / pi
		- SinL*CosL*(32005.7799 + 133.9238*SinL_2 + 0.6973*SinL_2*SinL_2 + 0.0039*SinL_2*SinL_2*SinL_2);
	gps_yy = X + N54*t*m*(0.5 + 1.0 / 24.0*(5.0 - t_2 + 9.0*yita_2 + 4.0*yita_4)*m
		+ 1.0 / 720.0*(61.0 - 58.0*t_2 + t_4)*m*m);
	gps_xx = N54*CosL*lp*(1.0 + 1.0 / 6.0*(1 - t_2 + yita_2)*m
		+ 1.0 / 120.0*(5.0 - 18.0*t_2 + t_4 + 14.0*yita_2 - 58.0*yita_2*t_2)*m*m);
	gps_xx = gps_xx + 1000000 * n + 500000;
	gps_xx *= 100;
	gps_yy *= 100;
	earthx = (int)gps_xx;
	earthy = (int)gps_yy;
	return;
}

//�������Ծ�γ�Ȼ�ȡ֮���ֱ�߾���(m)
double ToolsUtil::GetDistanceByGPS(double lng1,double lat1,double lng2,double lat2){
	int earthxcur, earthycur;
	int earthxtemp, earthytemp;
	ToolsUtil::GPS2Earthy(lat1 , lng1 , earthxcur, earthycur);
	ToolsUtil::GPS2Earthy(lat2, lng2, earthxtemp, earthytemp);
	double x = abs(earthxcur - earthxtemp) / 100;
	double y = abs(earthycur - earthytemp) / 100;
	double distance = sqrt(x*x + y*y);
	return distance;
}

void ToolsUtil::WtoA(char* dst,unsigned int dstLen,const CString *src){
	memset(dst,0,dstLen);
	unsigned int strlen=src->GetLength();
	wcstombs(dst, *src, strlen);
}


//����·���ļ� ��Ȼ����汾
int ToolsUtil::AnalyticalZR(NJUSTMap &map){
	vector<string> strLines;
	string oneline;
	ifstream  fTask;
	string filename="D:\\map\\task.txt";
	//string filename="D:\\map\\KYXZ2016A.txt";
	vector<MAP_TASK_NODE> vTask; //�ַ���������·��ṹ
	vector<MAP_TASK_NODE> vOutTask; //��������initalNodequeue

	//Step 1 -----------��ȡԭ�ļ�--------------
	fTask.open(filename);
	if(fTask.is_open()){
		//���ж���strLines
		while (getline(fTask,oneline)){
			strLines.push_back(oneline);
		}
		fTask.close();

		//Step 2 -----------�������ṹ����--------------
		for(auto &tline:strLines){
			istringstream stream(tline);
			MAP_TASK_NODE tasknode;
			double height;     //û�õ��߶� ��ʱ�޳�
			stream >> tasknode.num      >> tasknode.longtitude
				>> tasknode.latitude >> height 
				>> tasknode.shuxing1 >> tasknode.shuxing2;
			vTask.push_back(tasknode);
		}

		int num=0; //����tOutTask�ı��
		for (auto &tTasknode:vTask)
		{
			if (tTasknode.shuxing1 == 0 || tTasknode.shuxing1 == 7 || tTasknode.shuxing1 == 1)
			{
				int result = map.getIndexByGPS(tTasknode.longtitude, tTasknode.latitude,FIND_NODE_THRE);
				//�ҵ�����ĵ�
				if (result == -1) //û��ID����
					continue;
				if(vOutTask.empty()){ //��һ�β��ø�ǰ֡�Ա�
					MAP_TASK_NODE tasknode;
					tasknode.num = num;
					tasknode.longtitude = tTasknode.longtitude;//�Է�Ϊ��λ
					tasknode.latitude = tTasknode.latitude;
					tasknode.noderesult = result;//����·���еĵ�һ�μ�С
					tasknode.shuxing1 = tTasknode.shuxing1;
					tasknode.shuxing2 = tTasknode.shuxing2;
					tasknode.duiyingludianbianhao=tTasknode.num;
					vOutTask.push_back(tasknode);
					continue;
				}
				//�ظ�������
				if(vOutTask.back().noderesult!=result){
					MAP_TASK_NODE tasknode;
					tasknode.num = num;
					tasknode.longtitude = tTasknode.longtitude;//�Է�Ϊ��λ
					tasknode.latitude = tTasknode.latitude;
					tasknode.noderesult = result;//����·���еĵ�һ�μ�С
					tasknode.shuxing1 = tTasknode.shuxing1;
					tasknode.shuxing2 = tTasknode.shuxing2;
					tasknode.duiyingludianbianhao=tTasknode.num;
					vOutTask.push_back(tasknode);
				}
			}
		}

		//дԭʼ������ļ�InitialNodeQueue
		CFile fileOut;
		fileOut.Open(L"D:\\map\\InitialNodeQueue.db",CFile::modeCreate|CFile::modeWrite);
		FILE *pft = fopen("D:\\map\\InitialNodeQueue.txt", "w");
		for (auto &tTasknode:vOutTask){
			fileOut.Write(&tTasknode, sizeof(MAP_TASK_NODE));
			fprintf(pft,"%d\n",tTasknode.noderesult);
		}
		fileOut.Close();
		fclose(pft);

		//TRACE("�ṹ�峤�ȣ�%d",sizeof(ROADNODE));
		//���Զ�
		pft = fopen("D:\\map\\InitialNodeQueue.db", "rb");
		fseek(pft, 0L, SEEK_END);
		int len = ftell(pft) / sizeof(ROADNODE);
		fseek(pft, 0L, SEEK_SET);
		ROADNODE tROAD;
		for(int i=0;i<len;i++){
			fread(&tROAD, sizeof(ROADNODE), 1, pft);
		}
		fclose(pft);
	}else{
		AfxMessageBox(L"δ�ҵ��ļ�",MB_OK);
	}
	return 0;
}

//����·���ļ� ��װ�汾 -1�յ㶨λʧ��
int ToolsUtil::AnalyticalZZ(NJUSTMap &map){
	vector<string> strLines;
	string oneline;
	ifstream  fTask;
	string filename="D:\\map\\KYXZ2016A.txt";
	vector<MAP_TASK_NODE_ZZ> vTask; //�ַ���������·��ṹ
	vector<MAP_TASK_NODE_ZZ> vOutTask; //��������initalNodequeue

	//Step 1 -----------��ȡԭ�ļ�--------------
	fTask.open(filename);
	if(fTask.is_open()){
		//���ж���strLines
		while (getline(fTask,oneline)){
			strLines.push_back(oneline);
		}
		fTask.close();

		//Step 2 -----------�������ṹ����--------------
		for(auto &tline:strLines){
			istringstream stream(tline);
			MAP_TASK_NODE_ZZ tasknode;
			double height;     //û�õ��߶� ��ʱ�޳�
			stream >> tasknode.num  >> tasknode.longtitude
				>> tasknode.latitude >> tasknode.heightM 
				>> tasknode.type;
			tasknode.resultCode=-1;
			vTask.push_back(tasknode);
		}
		//Step 3 -----------����·���ļ�-------------- 
		//��� x,y,y,y,z1,t //x��� y·�� z1�����յ� z2ԭʼ�յ� t�����
		MAP_TASK_NODE_ZZ tasknode,endNode,missionNode;
		for (auto &tTasknode:vTask)
		{
			//Step 3.1 -----------������·�ں����--------------
			if (tTasknode.type == 3||tTasknode.type == 0)
			{
				//��50�׷�Χ��
				int result = map.getIndexByGPS(tTasknode.longtitude, tTasknode.latitude,FIND_NODE_THRE);
				//�ҵ�����ĵ�
				if (result == -1) //����code
					continue;
				if(vOutTask.empty()){ //��һ�β��ø�ǰ֡�Ա�
					tasknode=tTasknode;
					tasknode.resultCode=result;
					vOutTask.push_back(tasknode);
					continue;
				}
				//�ظ�������
				if(vOutTask.back().resultCode!=result){
					tasknode=tTasknode;
					tasknode.resultCode=result;
					vOutTask.push_back(tasknode);
					continue;
				}
			}
			//Step 3.2 -----------��ʱ��¼�յ�--------------
			if(tTasknode.type == 1){
				endNode=tTasknode;
			}
			//Step 3.3 -----------��ʱ��¼�����--------------
			if(tTasknode.type == 4){
				missionNode=tTasknode;
			}
		}

		//Step 4.1 ----------�����յ㵽·����---------------
		//��5���ﷶΧ������Ľڵ�
		int result = map.getNodeIndexByGPSWithoutV(endNode.longtitude, endNode.latitude,FIND_END_THRE,vOutTask);
		//�ҵ�����ĵ�
		if (result == -1) //����code
			return -1;
		endNode.resultCode=result;
		vOutTask.push_back(endNode);
		vOutTask.push_back(missionNode);

		//дԭʼ������ļ�InitialNodeQueue
		CFile fileOut;
		fileOut.Open(L"D:\\map\\InitialNodeQueue.db",CFile::modeCreate|CFile::modeWrite);
		FILE *pft = fopen("D:\\map\\InitialNodeQueue.txt", "w");
		for (auto &tTasknode:vOutTask){
			fileOut.Write(&tTasknode, sizeof(MAP_TASK_NODE_ZZ));
			fprintf(pft,"%d\n",tTasknode.resultCode);
		}
		fileOut.Close();
		fclose(pft);

		//TRACE("�ṹ�峤�ȣ�%d",sizeof(ROADNODE));
		//���Զ�
		pft = fopen("D:\\map\\InitialNodeQueue.db", "rb");
		fseek(pft, 0L, SEEK_END);
		int len = ftell(pft) / sizeof(MAP_TASK_NODE_ZZ);
		fseek(pft, 0L, SEEK_SET);
		MAP_TASK_NODE_ZZ tROAD;
		for(int i=0;i<len;i++){
			fread(&tROAD, sizeof(MAP_TASK_NODE_ZZ), 1, pft);
		}
		fclose(pft);
	}else{
		AfxMessageBox(L"δ�ҵ��ļ�",MB_OK);
	}
	return 0;
}




static NJUST_MC_STATE_INFO  gMCState;
static NJUST_MC_NAV_INFO    gMCNav;
static NJUST_MC_DRIVE_INFO  gMCDrive;

int ToolsUtil::NJUST_MC_Decode_IP_Data( const void* pIPData, const int nBytes,
													NJUST_MC_STATE_INFO  **pState, //������״̬����ʱ,ֵΪNULL
													NJUST_MC_NAV_INFO  **pNav, //�����ǵ�����Ϣʱ,ֵΪNULL
													NJUST_MC_DRIVE_INFO  **pDrive  //������ִ������ʱ,ֵΪNULL
													)
{
	int errCode;

	// step.1----------��ʼ��--------------------------------//
	*pState=NULL;
	*pNav=NULL;
	*pDrive=NULL;
	errCode=1;   //����Ч���� 
	//step.2----------���ݽ���-------------------------------//
	if(((char *)pIPData)[0]==0x24&&((char *)pIPData)[1]==0x09){//״̬
		errCode=NJUST_MC_Decode_State(pIPData,nBytes);
		if (!errCode)
		{
			*pState=&gMCState;
		}
	}else if(((char *)pIPData)[0]==0x24&&((char *)pIPData)[1]==0x08){//����
		errCode=NJUST_MC_Decode_NAV(pIPData,nBytes);
		if (!errCode)
		{
			*pNav=&gMCNav;
		}

	}else if(((char *)pIPData)[0]==0x24&&((char *)pIPData)[1]==0x07){//����
		errCode=NJUST_MC_Decode_Drive(pIPData,nBytes);
		if (!errCode)
		{
			*pDrive=&gMCDrive;
		}
	}

	//step.3----------����-----------------------------------//
	return errCode;
}



int ToolsUtil::NJUST_MC_Decode_State(const void* pIPData, const int nBytes)
{   //����������Ϣ:�����ݽ�����gMCState

	int errCode=0; //�޴���
	char *pBuf;
	signed char checksum;
	int i;
	//step.1----------��ʼ��---------------------------------//
	memset(&gMCState,0,sizeof(NJUST_MC_STATE_INFO));


	//step.2----------�����ݽ�����gMCState-------------------//
	pBuf = (char *)pIPData;
	for(i=0,checksum=0;i<nBytes-1;i++)
	{	
		checksum +=pBuf[i];    
	}
	i=sizeof(NJUST_MC_STATE_INFO);
	if((signed char)(pBuf[nBytes-1])!=checksum||(nBytes)!=sizeof(NJUST_MC_STATE_INFO)){//||(nBytes)!=sizeof(NJUST_MC_STATE_INFO)
		errCode=1;//����У��λ��checksum���С���ԣ��д���
	}else{
		memcpy(&gMCState,pBuf,sizeof(NJUST_MC_STATE_INFO));
		if(gMCState.nSize!=sizeof(NJUST_MC_STATE_INFO)){//�ṹ���С�Բ���
			memset(&gMCState,0,sizeof(NJUST_MC_STATE_INFO));
			errCode=1;
		}
	}

	//step.3----------���ش�����-----------------------------//
	return errCode;
}

int ToolsUtil::NJUST_MC_Decode_NAV(const void* pIPData, const int nBytes)
{   //����״̬����,�����ݽ�����gMCNav
	int errCode=0; //�޴���
	char *pBuf;
	signed char checksum;
	int i;
	//step.1----------��ʼ��---------------------------------//
	memset(&gMCNav,0,sizeof(NJUST_MC_NAV_INFO));
	//step.2----------�����ݽ�����gMCNav---------------------//
	pBuf = (char *)pIPData;
	for(i=0,checksum=0;i<nBytes-1;i++)
	{	
		checksum +=pBuf[i];    
	}
	i=sizeof(NJUST_MC_NAV_INFO);

	//pBuf[nBytes-1]!=0x0a||pBuf[nBytes-2]!=0x0d
	if((signed char)(pBuf[nBytes-1])!=checksum||(nBytes)!=sizeof(NJUST_MC_NAV_INFO)){
		errCode=1;//����У��λ��checksum���С���ԣ��д���
	}else{
		memcpy(&gMCNav,pBuf,sizeof(NJUST_MC_NAV_INFO));
		if(gMCNav.nSize!=sizeof(NJUST_MC_NAV_INFO)){//�ṹ���С�Բ���
			memset(&gMCNav,0,sizeof(NJUST_MC_NAV_INFO));
			errCode=1;
		}
	}



	//step.3----------���ش�����-----------------------------//
	return errCode;

	return errCode;
}

int ToolsUtil::NJUST_MC_Decode_Drive(const void* pIPData, const int nBytes)
{   //����ִ������,�����ݽ�����gMCDrive
	int errCode=0; //�޴���
	char *pBuf;
	signed char checksum;
	int i;
	//step.1----------��ʼ��---------------------------------//
	memset(&gMCDrive,0,sizeof(NJUST_MC_DRIVE_INFO));
	//step.2----------�����ݽ�����gMCDrive-------------------//
	pBuf = (char *)pIPData;
	for(i=0,checksum=0;i<nBytes-3;i++)
	{	
		checksum +=pBuf[i];    
	}

	if(pBuf[nBytes-1]!=0x0a||pBuf[nBytes-2]!=0x0d||(signed char)(pBuf[nBytes-3])!=checksum||(nBytes-2)!=sizeof(NJUST_MC_DRIVE_INFO)){
		errCode=1;//����У��λ��checksum���С���ԣ��д���
	}else{
		memcpy(&gMCDrive,pBuf,sizeof(NJUST_MC_DRIVE_INFO));
		if(gMCDrive.nSize!=sizeof(NJUST_MC_DRIVE_INFO)){//�ṹ���С�Բ���
			memset(&gMCDrive,0,sizeof(NJUST_MC_DRIVE_INFO));
			errCode=1;
		}
	}

	//step.3----------���ش�����-----------------------------//
	return errCode;
}
