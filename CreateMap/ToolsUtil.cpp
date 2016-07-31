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

//映射路口阈值
const double FIND_NODE_THRE=50.0;
//映射终点阈值
const double FIND_END_THRE=5000.0;

ToolsUtil::ToolsUtil(void)
{
}


ToolsUtil::~ToolsUtil(void)
{
}


//在直线模型下修补断层
void ToolsUtil::CompleGPS(vector<MAP_DOUBLE_POINT> &GPSList){
	int preLen=2;//取前2个点作为先验
	double dx,dy,dt,dt1,dtSum;
	bool isV;//主方向时候是水平
	if(GPSList.size()<preLen)
		return;
	dx=dy=0.0;
	//Step 1 -----------计算先验--------------
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
	//Step 2 -----------查找缺口--------------
	int cutI=-1;//缺口索引
	for(int i=preLen-1;i<GPSList.size()-1;i++){
		if(isV){
			dt1=GPSList[i+1].x-GPSList[i].x;
		}else{
			dt1=GPSList[i+1].y-GPSList[i].y;
		}
		if(abs(dt1)>abs(10*dt)){
			cutI=i+1;//断点后面
			break;
		}
	}
	if(cutI==0)
		return;
	//Step 3 -----------重算先验--------------
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

	//Step 4 -----------填充缺口--------------
	MAP_DOUBLE_POINT p=GPSList[cutI-1];
	for(dtSum=0;dtSum<abs(dt1);cutI,dtSum+=abs(dt)){
		p.x+=dx;p.y+=dy;
		GPSList.insert(GPSList.begin()+cutI,p);//前插入
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

//计算两对经纬度获取之间的直线距离(m)
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


//解析路点文件 自然基金版本
int ToolsUtil::AnalyticalZR(NJUSTMap &map){
	vector<string> strLines;
	string oneline;
	ifstream  fTask;
	string filename="D:\\map\\task.txt";
	//string filename="D:\\map\\KYXZ2016A.txt";
	vector<MAP_TASK_NODE> vTask; //字符串解析成路点结构
	vector<MAP_TASK_NODE> vOutTask; //用于生成initalNodequeue

	//Step 1 -----------读取原文件--------------
	fTask.open(filename);
	if(fTask.is_open()){
		//按行读入strLines
		while (getline(fTask,oneline)){
			strLines.push_back(oneline);
		}
		fTask.close();

		//Step 2 -----------解析到结构体中--------------
		for(auto &tline:strLines){
			istringstream stream(tline);
			MAP_TASK_NODE tasknode;
			double height;     //没用到高度 暂时剔除
			stream >> tasknode.num      >> tasknode.longtitude
				>> tasknode.latitude >> height 
				>> tasknode.shuxing1 >> tasknode.shuxing2;
			vTask.push_back(tasknode);
		}

		int num=0; //用于tOutTask的编号
		for (auto &tTasknode:vTask)
		{
			if (tTasknode.shuxing1 == 0 || tTasknode.shuxing1 == 7 || tTasknode.shuxing1 == 1)
			{
				int result = map.getIndexByGPS(tTasknode.longtitude, tTasknode.latitude,FIND_NODE_THRE);
				//找到最近的点
				if (result == -1) //没有ID返回
					continue;
				if(vOutTask.empty()){ //第一次不用跟前帧对比
					MAP_TASK_NODE tasknode;
					tasknode.num = num;
					tasknode.longtitude = tTasknode.longtitude;//以分为单位
					tasknode.latitude = tTasknode.latitude;
					tasknode.noderesult = result;//不在路网中的点一次减小
					tasknode.shuxing1 = tTasknode.shuxing1;
					tasknode.shuxing2 = tTasknode.shuxing2;
					tasknode.duiyingludianbianhao=tTasknode.num;
					vOutTask.push_back(tasknode);
					continue;
				}
				//重复不考虑
				if(vOutTask.back().noderesult!=result){
					MAP_TASK_NODE tasknode;
					tasknode.num = num;
					tasknode.longtitude = tTasknode.longtitude;//以分为单位
					tasknode.latitude = tTasknode.latitude;
					tasknode.noderesult = result;//不在路网中的点一次减小
					tasknode.shuxing1 = tTasknode.shuxing1;
					tasknode.shuxing2 = tTasknode.shuxing2;
					tasknode.duiyingludianbianhao=tTasknode.num;
					vOutTask.push_back(tasknode);
				}
			}
		}

		//写原始任务点文件InitialNodeQueue
		CFile fileOut;
		fileOut.Open(L"D:\\map\\InitialNodeQueue.db",CFile::modeCreate|CFile::modeWrite);
		FILE *pft = fopen("D:\\map\\InitialNodeQueue.txt", "w");
		for (auto &tTasknode:vOutTask){
			fileOut.Write(&tTasknode, sizeof(MAP_TASK_NODE));
			fprintf(pft,"%d\n",tTasknode.noderesult);
		}
		fileOut.Close();
		fclose(pft);

		//TRACE("结构体长度：%d",sizeof(ROADNODE));
		//测试读
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
		AfxMessageBox(L"未找到文件",MB_OK);
	}
	return 0;
}

//解析路点文件 总装版本 -1终点定位失败
int ToolsUtil::AnalyticalZZ(NJUSTMap &map){
	vector<string> strLines;
	string oneline;
	ifstream  fTask;
	string filename="D:\\map\\KYXZ2016A.txt";
	vector<MAP_TASK_NODE_ZZ> vTask; //字符串解析成路点结构
	vector<MAP_TASK_NODE_ZZ> vOutTask; //用于生成initalNodequeue

	//Step 1 -----------读取原文件--------------
	fTask.open(filename);
	if(fTask.is_open()){
		//按行读入strLines
		while (getline(fTask,oneline)){
			strLines.push_back(oneline);
		}
		fTask.close();

		//Step 2 -----------解析到结构体中--------------
		for(auto &tline:strLines){
			istringstream stream(tline);
			MAP_TASK_NODE_ZZ tasknode;
			double height;     //没用到高度 暂时剔除
			stream >> tasknode.num  >> tasknode.longtitude
				>> tasknode.latitude >> tasknode.heightM 
				>> tasknode.type;
			tasknode.resultCode=-1;
			vTask.push_back(tasknode);
		}
		//Step 3 -----------解析路点文件-------------- 
		//结果 x,y,y,y,z1,t //x起点 y路口 z1解析终点 z2原始终点 t任务点
		MAP_TASK_NODE_ZZ tasknode,endNode,missionNode;
		for (auto &tTasknode:vTask)
		{
			//Step 3.1 -----------解析岔路口和起点--------------
			if (tTasknode.type == 3||tTasknode.type == 0)
			{
				//在50米范围内
				int result = map.getIndexByGPS(tTasknode.longtitude, tTasknode.latitude,FIND_NODE_THRE);
				//找到最近的点
				if (result == -1) //返回code
					continue;
				if(vOutTask.empty()){ //第一次不用跟前帧对比
					tasknode=tTasknode;
					tasknode.resultCode=result;
					vOutTask.push_back(tasknode);
					continue;
				}
				//重复不考虑
				if(vOutTask.back().resultCode!=result){
					tasknode=tTasknode;
					tasknode.resultCode=result;
					vOutTask.push_back(tasknode);
					continue;
				}
			}
			//Step 3.2 -----------暂时记录终点--------------
			if(tTasknode.type == 1){
				endNode=tTasknode;
			}
			//Step 3.3 -----------暂时记录任务点--------------
			if(tTasknode.type == 4){
				missionNode=tTasknode;
			}
		}

		//Step 4.1 ----------解析终点到路网中---------------
		//在5公里范围内最近的节点
		int result = map.getNodeIndexByGPSWithoutV(endNode.longtitude, endNode.latitude,FIND_END_THRE,vOutTask);
		//找到最近的点
		if (result == -1) //返回code
			return -1;
		endNode.resultCode=result;
		vOutTask.push_back(endNode);
		vOutTask.push_back(missionNode);

		//写原始任务点文件InitialNodeQueue
		CFile fileOut;
		fileOut.Open(L"D:\\map\\InitialNodeQueue.db",CFile::modeCreate|CFile::modeWrite);
		FILE *pft = fopen("D:\\map\\InitialNodeQueue.txt", "w");
		for (auto &tTasknode:vOutTask){
			fileOut.Write(&tTasknode, sizeof(MAP_TASK_NODE_ZZ));
			fprintf(pft,"%d\n",tTasknode.resultCode);
		}
		fileOut.Close();
		fclose(pft);

		//TRACE("结构体长度：%d",sizeof(ROADNODE));
		//测试读
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
		AfxMessageBox(L"未找到文件",MB_OK);
	}
	return 0;
}




static NJUST_MC_STATE_INFO  gMCState;
static NJUST_MC_NAV_INFO    gMCNav;
static NJUST_MC_DRIVE_INFO  gMCDrive;

int ToolsUtil::NJUST_MC_Decode_IP_Data( const void* pIPData, const int nBytes,
													NJUST_MC_STATE_INFO  **pState, //当不是状态数据时,值为NULL
													NJUST_MC_NAV_INFO  **pNav, //当不是导航信息时,值为NULL
													NJUST_MC_DRIVE_INFO  **pDrive  //当不是执行数据时,值为NULL
													)
{
	int errCode;

	// step.1----------初始化--------------------------------//
	*pState=NULL;
	*pNav=NULL;
	*pDrive=NULL;
	errCode=1;   //无有效数据 
	//step.2----------数据解析-------------------------------//
	if(((char *)pIPData)[0]==0x24&&((char *)pIPData)[1]==0x09){//状态
		errCode=NJUST_MC_Decode_State(pIPData,nBytes);
		if (!errCode)
		{
			*pState=&gMCState;
		}
	}else if(((char *)pIPData)[0]==0x24&&((char *)pIPData)[1]==0x08){//导航
		errCode=NJUST_MC_Decode_NAV(pIPData,nBytes);
		if (!errCode)
		{
			*pNav=&gMCNav;
		}

	}else if(((char *)pIPData)[0]==0x24&&((char *)pIPData)[1]==0x07){//驱动
		errCode=NJUST_MC_Decode_Drive(pIPData,nBytes);
		if (!errCode)
		{
			*pDrive=&gMCDrive;
		}
	}

	//step.3----------返回-----------------------------------//
	return errCode;
}



int ToolsUtil::NJUST_MC_Decode_State(const void* pIPData, const int nBytes)
{   //解析导航信息:将数据解析到gMCState

	int errCode=0; //无错误
	char *pBuf;
	signed char checksum;
	int i;
	//step.1----------初始化---------------------------------//
	memset(&gMCState,0,sizeof(NJUST_MC_STATE_INFO));


	//step.2----------将数据解析到gMCState-------------------//
	pBuf = (char *)pIPData;
	for(i=0,checksum=0;i<nBytes-1;i++)
	{	
		checksum +=pBuf[i];    
	}
	i=sizeof(NJUST_MC_STATE_INFO);
	if((signed char)(pBuf[nBytes-1])!=checksum||(nBytes)!=sizeof(NJUST_MC_STATE_INFO)){//||(nBytes)!=sizeof(NJUST_MC_STATE_INFO)
		errCode=1;//两个校验位或checksum或大小不对，有错误
	}else{
		memcpy(&gMCState,pBuf,sizeof(NJUST_MC_STATE_INFO));
		if(gMCState.nSize!=sizeof(NJUST_MC_STATE_INFO)){//结构体大小对不上
			memset(&gMCState,0,sizeof(NJUST_MC_STATE_INFO));
			errCode=1;
		}
	}

	//step.3----------返回错误码-----------------------------//
	return errCode;
}

int ToolsUtil::NJUST_MC_Decode_NAV(const void* pIPData, const int nBytes)
{   //解析状态数据,将数据解析到gMCNav
	int errCode=0; //无错误
	char *pBuf;
	signed char checksum;
	int i;
	//step.1----------初始化---------------------------------//
	memset(&gMCNav,0,sizeof(NJUST_MC_NAV_INFO));
	//step.2----------将数据解析到gMCNav---------------------//
	pBuf = (char *)pIPData;
	for(i=0,checksum=0;i<nBytes-1;i++)
	{	
		checksum +=pBuf[i];    
	}
	i=sizeof(NJUST_MC_NAV_INFO);

	//pBuf[nBytes-1]!=0x0a||pBuf[nBytes-2]!=0x0d
	if((signed char)(pBuf[nBytes-1])!=checksum||(nBytes)!=sizeof(NJUST_MC_NAV_INFO)){
		errCode=1;//两个校验位或checksum或大小不对，有错误
	}else{
		memcpy(&gMCNav,pBuf,sizeof(NJUST_MC_NAV_INFO));
		if(gMCNav.nSize!=sizeof(NJUST_MC_NAV_INFO)){//结构体大小对不上
			memset(&gMCNav,0,sizeof(NJUST_MC_NAV_INFO));
			errCode=1;
		}
	}



	//step.3----------返回错误码-----------------------------//
	return errCode;

	return errCode;
}

int ToolsUtil::NJUST_MC_Decode_Drive(const void* pIPData, const int nBytes)
{   //解析执行数据,将数据解析到gMCDrive
	int errCode=0; //无错误
	char *pBuf;
	signed char checksum;
	int i;
	//step.1----------初始化---------------------------------//
	memset(&gMCDrive,0,sizeof(NJUST_MC_DRIVE_INFO));
	//step.2----------将数据解析到gMCDrive-------------------//
	pBuf = (char *)pIPData;
	for(i=0,checksum=0;i<nBytes-3;i++)
	{	
		checksum +=pBuf[i];    
	}

	if(pBuf[nBytes-1]!=0x0a||pBuf[nBytes-2]!=0x0d||(signed char)(pBuf[nBytes-3])!=checksum||(nBytes-2)!=sizeof(NJUST_MC_DRIVE_INFO)){
		errCode=1;//两个校验位或checksum或大小不对，有错误
	}else{
		memcpy(&gMCDrive,pBuf,sizeof(NJUST_MC_DRIVE_INFO));
		if(gMCDrive.nSize!=sizeof(NJUST_MC_DRIVE_INFO)){//结构体大小对不上
			memset(&gMCDrive,0,sizeof(NJUST_MC_DRIVE_INFO));
			errCode=1;
		}
	}

	//step.3----------返回错误码-----------------------------//
	return errCode;
}
