#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

//��������Ϊdouble �ĵ��
typedef struct MAP_DOUBLE_POINT{
	double x;
	double y;
}MAP_DOUBLE_POINT;


vector<MAP_DOUBLE_POINT> gpslist;

//����ָ��·��gps����
void ReadMapGPS(int a,int b,vector<MAP_DOUBLE_POINT> &GPSList,bool isNode){
	char loadpath[20]="D:\\map\\";

	GPSList.clear();
	char cj=isNode?'+':'-';
	char path[50];
	char filename[20];
	bool isOrder=true;		//�Ƿ����ļ���˳���ȡ
	int GPSnum=0;
	MAP_DOUBLE_POINT tPoint;
	GPSList.reserve(2048);

	
	sprintf(filename,"%d%c%d.db",a,cj,b); //1-2.db or 1+2.db
	strcpy(path,loadpath);
	strcat(path,filename);  //ƴ������Ŀ¼
	

	FILE *pf = fopen(path ,"rb");
	

	//��������򶼳���һ��
	if (pf == NULL)
	{
		isOrder=false;
		memset(path,0,50);
		sprintf(filename,"%d%c%d.db",b,cj,a);
		strcpy(path,loadpath);
		strcat(path,filename);  //ƴ������Ŀ¼

		pf = fopen(path ,"rb");
	}
	if(pf==NULL){
		perror("perror");
		return ;
	}
	

	//��ȡGPS����
	fseek(pf, 0L, SEEK_END);
	GPSnum = ftell(pf) / sizeof(MAP_DOUBLE_POINT);
	fseek(pf, 0L, SEEK_SET);
	for(int i=0;i<GPSnum;i++){
		fread(&tPoint, sizeof(MAP_DOUBLE_POINT), 1, pf);
		GPSList.push_back(tPoint);
	}
	//������
	if(!isOrder){
		reverse(GPSList.begin(),GPSList.end());
	}

	fclose(pf);


}

void WriteGPS(){
	unsigned int i;
	ofstream outstream;
	outstream.open("D:\\map\\1-2.txt");
	if(outstream.is_open()){
		for(i=0;i<gpslist.size();i++){
			 outstream<<setiosflags(ios::fixed)<<setprecision(8);
			outstream<<gpslist[i].x<<" "<<gpslist[i].y<<endl;
		}
		outstream.close();
	}else{
		cout<<"open fail"<<endl;
	}
}



void main(){
	
	ReadMapGPS(1,2,gpslist,false);
	WriteGPS();
}