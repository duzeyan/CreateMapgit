#include "stdafx.h"
#include "NJUSTMap.h"
#include <stdio.h>
#include <afx.h>
#include <iomanip>
#include"ToolsUtil.h"
using namespace std;



NJUSTMap::NJUSTMap(){
	buildGPS[0].x=buildGPS[0].y=0;
	buildGPS[1].x=buildGPS[1].y=0;
	buildGPS[2].x=buildGPS[2].y=0;
}



void NJUSTMap::init(){
	/*this->buildGPS[0]=buildGPS[0];
	this->buildGPS[1]=buildGPS[1];

	scaleX=(buildGPS[1].lng-buildGPS[0].lng)/(buildGPS[1].x-buildGPS[0].x);
	scaleY=(buildGPS[1].lat-buildGPS[0].lat)/(buildGPS[1].y-buildGPS[0].y);*/
	roads.clear();
	nodes.clear();
	crosses.clear();
}

//����߶�
void NJUSTMap::computeScale(){
	scaleX=(buildGPS[1].lng-buildGPS[0].lng)/(buildGPS[1].x-buildGPS[0].x);
	scaleY=(buildGPS[1].lat-buildGPS[0].lat)/(buildGPS[1].y-buildGPS[0].y);

	//��������߶�
	/*double scaleX01=(buildGPS[1].lng-buildGPS[0].lng)/(buildGPS[1].x-buildGPS[0].x);
	double scaleX12=(buildGPS[1].lng-buildGPS[2].lng)/(buildGPS[1].x-buildGPS[2].x);
	double scaleX02=(buildGPS[0].lng-buildGPS[2].lng)/(buildGPS[0].x-buildGPS[2].x);

	double scaleY01=(buildGPS[1].lat-buildGPS[0].lat)/(buildGPS[1].y-buildGPS[0].y);
	double scaleY12=(buildGPS[1].lat-buildGPS[2].lat)/(buildGPS[1].y-buildGPS[2].y);
	double scaleY02=(buildGPS[0].lat-buildGPS[2].lat)/(buildGPS[0].y-buildGPS[2].y);
	scaleX=(scaleX01+scaleX02+scaleX12)/3;
	scaleY=(scaleY01+scaleY02+scaleY12)/3;*/
}

//����Ƿ��Ѿ����úò���
bool NJUSTMap::CheckIsCali(){
	if(abs(buildGPS[0].lat-.0l)<0.001) //==0
		return false;
	if(abs(buildGPS[0].lng-.0l)<0.001)
		return false;
	if(abs(buildGPS[1].lat-.0l)<0.001)
		return false;
	if(abs(buildGPS[1].lng-.0l)<0.001)
		return false;

	return true;
}

void NJUSTMap::deleteEleByID(bool isNode,int id){
	unsigned int i;
	if(!isNode){
		for(i=0;i<roads.size();i++){
				if(id==roads[i].road.idself){
					roads.erase(roads.begin()+i);
					return ;
				}
		}
	}else{
		for(i=0;i<nodes.size();i++){
				if(id==nodes[i].node.idself){
					nodes.erase(nodes.begin()+i);
					return ;
				}
		}
	}
}

//Ϊ�˼򻯹��� Ĭ���ǰ���˳������ !!!���Ի�ͼʱһ��Ҫ����ͷβ˳�����!!! ID����Ҳ�������,���ɴ��
bool NJUSTMap::merge2Line(const vector<DRAW_RECORD> &mergeV,MAP_ROAD package){
	unsigned int i,j;
	CREATE_MAP_ROAD road;
	road.position.x=road.position.y=0;   //��ʼ��
	memset(&road.road,0,sizeof(MAP_ROAD));

	//Ŀǰֻ֧��ֱ�ߺͶϵ������ ������������
	for(i=0;i<mergeV.size();i++){
		switch (mergeV[i].type)
		{
	     //line
		case 0:{
				for(j=0;j<mergeV[i].drawPoints.size();j+=MAP_DASH_NUM)
					road.pInLine.push_back(mergeV[i].drawPoints[j]);
				if(j!=mergeV[i].drawPoints.size()+MAP_DASH_NUM-1) //��֤�߶���β����
					road.pInLine.push_back(mergeV[i].drawPoints.back());
				break;
			 }
		//points
		case 2:{
				for(j=0;j<mergeV[i].drawPoints.size();j++)
					road.pInLine.push_back(mergeV[i].drawPoints[j]);
				break;
			}
		//����������
		case 1:{
				for(j=0;j<mergeV[i].drawPoints.size();j+=MAP_DASH_NUM)
					road.pInLine.push_back(mergeV[i].drawPoints[j]);
				if(j!=mergeV[i].drawPoints.size()+MAP_DASH_NUM-1) //��֤�߶���β����
					road.pInLine.push_back(mergeV[i].drawPoints.back());
				break;
			   }
		default:
				return false;
			break;
		}
	}
	//����ID
	if(roads.empty()){
		package.idself=START_LINE_ID;
	}else{
		package.idself=roads.back().road.idself+1;
	}
	package.idstart=package.idstart+START_NODE_ID-1;
	package.idend=package.idend+START_NODE_ID-1;

	//ȫ����ֵ���ڵ�ͼ�ṹ��
	road.road=package;

	//���� ���֮ǰ����ͬԪ�����޳�
	for(i=0;i<roads.size();i++){
		if(roads[i].road.idstart==road.road.idstart && roads[i].road.idend==road.road.idend){
			roads.erase(roads.begin()+i);
			break;
		}
	}
	roads.push_back(road);

	//���� ��Ӧ�ڵ���ڽ�ID
	for( int i=0;i<nodes.size();i++){
		if(nodes[i].node.idself==package.idstart){ //���ó���·�� 
			  bool isHave=false;
			  int nindex=nodes[i].node.neigh;
			  for(int j=0;j<nindex;j++){ //�ظ������
				  if(package.idend==nodes[i].node.NeighNoteID[j]){ //�ýڵ��Լ���ӹ�
					  isHave=true;break;
				  }
			  }
			  if(!isHave){
				  nodes[i].node.NeighLineID[nindex]=package.idself;
				  nodes[i].node.NeighNoteID[nindex]=package.idend;
				  nodes[i].node.neigh++;
				  assert(nodes[i].node.neigh<=4); //����ĸ�·��
			  }
		}
		if(nodes[i].node.idself==package.idend){ //���ý���·��
			   bool isHave=false;
			   int nindex=nodes[i].node.neigh;
			   for(int j=0;j<nindex;j++){ //�ظ������
				 if(package.idstart==nodes[i].node.NeighNoteID[j]){ //�ýڵ��Ѿ���ӹ�
						isHave=true;break;
				 }
			   }
			   if(!isHave){
				   nodes[i].node.NeighLineID[nindex]=package.idself;
				   nodes[i].node.NeighNoteID[nindex]=package.idstart;
				   nodes[i].node.neigh++;
				   assert(nodes[i].node.neigh<=4); //����ĸ�·��
			   }
		}//if 
	}//for
	return true;
}

void NJUSTMap::removeObstaclesByID(int id){
	auto it=find(obstacles.begin(),obstacles.end(),id);
	assert(it!=obstacles.end());
	obstacles.erase(it);
}

void NJUSTMap::deleteRoadByIndex(int index){
	//��ȡҪɾ����·��ID
	int id=roads[index].road.idself;

	for(auto &tnode:nodes){
		int  whichIndex=-1;
		for(int i=0;i<tnode.node.neigh;i++){
			if(tnode.node.NeighLineID[i]==id){ //������� ��ɾ��
				whichIndex=i;
				break;        //�������ظ���·
			}
		}
		if(whichIndex!=-1){ //�����˹����õ�·
			for(int i=whichIndex;i<tnode.node.neigh-1;i++){ //ɾ��
				tnode.node.NeighLineID[i]=tnode.node.NeighLineID[i+1];
			}
			tnode.node.neigh--;
		}
	}
	//ɾ���õ�·
	roads.erase(roads.begin()+index);
}

//����·��
bool NJUSTMap::merge2Cross(const vector<DRAW_RECORD> &mergeV,CPoint line2ID){
	unsigned int i,j;
	CREATE_MAP_CROSS cross;
	cross.GPSDataFrom=NJUST_MAP_GPS_FROM_HAND_DRAW;
	//Ŀǰֻ֧��ֱ�ߺͶϵ������ ������������
	for(i=0;i<mergeV.size();i++){
		switch (mergeV[i].type)
		{
	     //line
		case 0:{
				for(j=0;j<mergeV[i].drawPoints.size();j+=MAP_DASH_NUM)
					cross.points.push_back(mergeV[i].drawPoints[j]);
				break;
			 }
		//points
		case 2:{
				for(j=0;j<mergeV[i].drawPoints.size();j++)
					cross.points.push_back(mergeV[i].drawPoints[j]);
				break;
			}
		//����������
		case 1:{
				for(j=0;j<mergeV[i].drawPoints.size();j+=MAP_DASH_NUM)
					cross.points.push_back(mergeV[i].drawPoints[j]);
				if(j!=mergeV[i].drawPoints.size()+MAP_DASH_NUM-1) //��֤�߶���β����
					cross.points.push_back(mergeV[i].drawPoints.back());
				break;
			   }
		default:
				return false;
			break;
		}
	}

	cross.idstart=line2ID.x+START_LINE_ID-1;
	cross.idend=line2ID.y+START_LINE_ID-1;

	//���� ���֮ǰ����ͬԪ�����޳�
	for(i=0;i<crosses.size();i++){
		if(crosses[i].idstart==cross.idstart &&crosses[i].idend==cross.idend){
			crosses.erase(crosses.begin()+i);
			break;
		}
	}
	crosses.push_back(cross);
	return true;
}


CString NJUSTMap::printRoad(unsigned int index)const{
	CString strShow;
	strShow.Format(L"��·%d:·��%d->·��%d",roads[index].road.idself-START_LINE_ID+1,
								roads[index].road.idstart-START_NODE_ID+1,
								roads[index].road.idend-START_NODE_ID+1); 
	return strShow;
}

CString NJUSTMap::printCross(unsigned int index)const{
	CString strShow;
	strShow.Format(L"·��:��·%d->��·%d",
								crosses[index].idstart-START_LINE_ID+1,
								crosses[index].idend-START_LINE_ID+1); 
	return strShow;
}



CString NJUSTMap::printRoadBack()const{
	return printRoad(roads.size()-1);
}


bool NJUSTMap::writeRoad(CString path){
	unsigned int i,j;
	//Step 1 -----------�����·--------------
	for(i=0;i<roads.size();i++){
		CString filenameP,filenameN; //����
		filenameP.Format(L"%s\\%d-%d.db",path, 
								roads[i].road.idstart-START_NODE_ID+1,
								roads[i].road.idend-START_NODE_ID+1);
		filenameN.Format(L"%s\\%d-%d.db",path, 
			roads[i].road.idend-START_NODE_ID+1,
			roads[i].road.idstart-START_NODE_ID+1);
		CFile file;
		//Step 1.1 -----------�ֻ�㼯--------------
		if(roads[i].road.GPSDataFrom==NJUST_MAP_GPS_FROM_HAND_DRAW){
			if(file.Open(filenameP,CFile::modeCreate|CFile::modeWrite)){
				for(j=0;j<roads[i].pInLine.size();j++){
					COMPUTE_GPS var(roads[i].pInLine[j].x,roads[i].pInLine[j].y,0.0,0.0);
					pixel2GPS(var);
					MAP_DOUBLE_POINT outpoint;
					outpoint.x=var.lng;outpoint.y=var.lat;
					outpoint.x*=60.0;outpoint.y*=60.0;     //ת��Ϊ��
					file.Write(&outpoint,sizeof(MAP_DOUBLE_POINT));
				}
				file.Close();
			}
		}
		//Step 1.2 -----------����ɼ�GPS--------------
		if(roads[i].road.GPSDataFrom==NJUST_MAP_GPS_FROM_CAR){
			if(!roads[i].realGPS.empty()){//������
				if(file.Open(filenameP,CFile::modeCreate|CFile::modeWrite)){
					for(auto &gps:roads[i].realGPS){
						MAP_DOUBLE_POINT outpoint=gps;
						outpoint.x*=60.0;outpoint.y*=60.0;     //ת��Ϊ��
						file.Write(&outpoint,sizeof(MAP_DOUBLE_POINT));
					}
				}
				file.Close();
			}//if ������
			if(!roads[i].realGPSNeg.empty()){//������
				if(file.Open(filenameN,CFile::modeCreate|CFile::modeWrite)){
					for(auto &gps:roads[i].realGPSNeg){
						MAP_DOUBLE_POINT outpoint=gps;
						outpoint.x*=60.0;outpoint.y*=60.0;     //ת��Ϊ��
						file.Write(&outpoint,sizeof(MAP_DOUBLE_POINT));
					}
				}
				file.Close();
			}//if ������
		}//if from_car	
	}//for roads

	//Step 2 -----------����·��--------------
	for(i=0;i<crosses.size();i++){
		CString filenameP,filenameN;
		filenameP.Format(L"%s\\%d+%d.db",path, 
								crosses[i].idstart-START_LINE_ID+1,
								crosses[i].idend-START_LINE_ID+1);
		filenameN.Format(L"%s\\%d+%d.db",path, 
			crosses[i].idend-START_LINE_ID+1,
			crosses[i].idstart-START_LINE_ID+1);
		CFile file;
		//Step 2.2 -----------�����ֻ�GPS--------------
		if(crosses[i].GPSDataFrom==NJUST_MAP_GPS_FROM_HAND_DRAW){
			if(file.Open(filenameP,CFile::modeCreate|CFile::modeWrite)){
				for(j=0;j<crosses[i].points.size();j++){
					COMPUTE_GPS var(crosses[i].points[j].x,crosses[i].points[j].y,0.0,0.0);
					pixel2GPS(var);
					MAP_DOUBLE_POINT outpoint;
					outpoint.x=var.lng;outpoint.y=var.lat;
					outpoint.x*=60.0;outpoint.y*=60.0;     //ת��Ϊ��
					file.Write(&outpoint,sizeof(MAP_DOUBLE_POINT));
				}
				file.Close();
			}
		}
		//Step 2.3 -----------����GPS--------------
		if(crosses[i].GPSDataFrom==NJUST_MAP_GPS_FROM_CAR){
			if(!crosses[i].realGPS.empty()){ //��
				if(file.Open(filenameP,CFile::modeCreate|CFile::modeWrite)){
						for(auto &gps:crosses[i].realGPS){
							MAP_DOUBLE_POINT outpoint=gps;
							outpoint.x*=60.0;outpoint.y*=60.0;     //ת��Ϊ��
							file.Write(&outpoint,sizeof(MAP_DOUBLE_POINT));
						}
						file.Close();
				}
			}
			if(!crosses[i].realGPSNeg.empty()){ //��
				if(file.Open(filenameN,CFile::modeCreate|CFile::modeWrite)){
					for(auto &gps:crosses[i].realGPSNeg){
						MAP_DOUBLE_POINT outpoint=gps;
						outpoint.x*=60.0;outpoint.y*=60.0;     //ת��Ϊ��
						file.Write(&outpoint,sizeof(MAP_DOUBLE_POINT));
					}
					file.Close();
				}
			}//end ��
	   }//from car	
	}//for cross
	return true;
}


//��ָ��Ŀ¼�±����ı��ļ�
bool NJUSTMap::writeRoadTxt(CString path){
	unsigned int i,j;
	//Step 1 -----------�����·--------------
	for(i=0;i<roads.size();i++){
		CString filenameP,filenameN;
		filenameP.Format(L"%s\\%d-%d.txt",path, 
								roads[i].road.idstart-START_NODE_ID+1,
								roads[i].road.idend-START_NODE_ID+1);
		filenameN.Format(L"%s\\%d-%d.txt",path, 
			roads[i].road.idend-START_NODE_ID+1,
			roads[i].road.idstart-START_NODE_ID+1);
		CFile file;
		//Step 1.1 -----------�ֻ�--------------
		if(roads[i].road.GPSDataFrom==NJUST_MAP_GPS_FROM_HAND_DRAW){
			if(file.Open(filenameP,CFile::modeCreate|CFile::modeWrite)){ //���ļ�
				for(j=0;j<roads[i].pInLine.size();j++){
					COMPUTE_GPS var(roads[i].pInLine[j].x,roads[i].pInLine[j].y,0.0,0.0);
					pixel2GPS(var);
					MAP_DOUBLE_POINT outpoint;
					outpoint.x=var.lng;outpoint.y=var.lat;
					char buf[100];  
					sprintf(buf,"%.10lf %.10lf \r\n",outpoint.x,outpoint.y);  
					file.Write(buf,strlen(buf));
				}
				file.Close();
			}
		}
		//Step 1.2 -----------�ɼ�GPS--------------
		if(roads[i].road.GPSDataFrom==NJUST_MAP_GPS_FROM_CAR){
			if(!roads[i].realGPS.empty()){//������
				if(file.Open(filenameP,CFile::modeCreate|CFile::modeWrite)){
					for(auto &gps:roads[i].realGPS){
						MAP_DOUBLE_POINT outpoint=gps;
						char buf[100];  
						sprintf(buf,"%.10lf %.10lf \r\n",outpoint.x,outpoint.y);  
						file.Write(buf,strlen(buf));
					}
					file.Close();
				}
			}//if ����
			if(!roads[i].realGPSNeg.empty()){//������
				if(file.Open(filenameN,CFile::modeCreate|CFile::modeWrite)){
					for(auto &gps:roads[i].realGPSNeg){
						MAP_DOUBLE_POINT outpoint=gps;
						char buf[100];  
						sprintf(buf,"%.10lf %.10lf \r\n",outpoint.x,outpoint.y);  
						file.Write(buf,strlen(buf));
					}
					file.Close();
				}
			}//if ����
		}//if from car
	}//for road
	
	//Step 2 -----------����·��--------------
	for(i=0;i<crosses.size();i++){
		CString filenameP,filenameN;
		filenameP.Format(L"%s\\%d+%d.txt",path, 
								crosses[i].idstart-START_LINE_ID+1,
								crosses[i].idend-START_LINE_ID+1);
		filenameN.Format(L"%s\\%d+%d.txt",path, 
			crosses[i].idend-START_LINE_ID+1,
			crosses[i].idstart-START_LINE_ID+1);
		CFile file;
		//Step 2.1 -----------�ֻ�--------------
		if(crosses[i].GPSDataFrom==NJUST_MAP_GPS_FROM_HAND_DRAW){
			if(file.Open(filenameP,CFile::modeCreate|CFile::modeWrite)){
				for(j=0;j<crosses[i].points.size();j++){
					COMPUTE_GPS var(crosses[i].points[j].x,crosses[i].points[j].y,0.0,0.0);
					pixel2GPS(var);
					MAP_DOUBLE_POINT outpoint;
					outpoint.x=var.lng;outpoint.y=var.lat;
					char buf[100];  
					sprintf(buf,"%.10lf %.10lf\r\n",outpoint.x,outpoint.y);  
					file.Write(buf,strlen(buf));
				}
				file.Close();
			}
		}
		//Step 2.2 -----------����GPS--------------
		if(crosses[i].GPSDataFrom==NJUST_MAP_GPS_FROM_CAR){
			if(!crosses[i].realGPS.empty()){ //������
				if(file.Open(filenameP,CFile::modeCreate|CFile::modeWrite)){
					for(auto &gps:crosses[i].realGPS){
						MAP_DOUBLE_POINT outpoint=gps;
						char buf[100];  
						sprintf(buf,"%.10lf %.10lf\r\n",outpoint.x,outpoint.y);  
						file.Write(buf,strlen(buf));
					}
					file.Close();
				}
			}
			if(!crosses[i].realGPSNeg.empty()){ //������
				if(file.Open(filenameN,CFile::modeCreate|CFile::modeWrite)){
					for(auto &gps:crosses[i].realGPS){
						MAP_DOUBLE_POINT outpoint=gps;
						char buf[100];  
						sprintf(buf,"%.10lf %.10lf\r\n",outpoint.x,outpoint.y);  
						file.Write(buf,strlen(buf));
					}
					file.Close();
				}
			}//����
		}//from car
	}//for cross
	return true;
}

//buildGPS[2]Ϊ��׼��
void NJUSTMap::pixel2GPS(COMPUTE_GPS &var){
	var=COMPUTE_GPS(var.x,
					var.y
					,(var.x-buildGPS[2].x)*1.0*scaleX+buildGPS[2].lng
					,(var.y-buildGPS[2].y)*1.0*scaleY+buildGPS[2].lat);
	return ;
}

void NJUSTMap::GPS2pexel(COMPUTE_GPS &var){
	int x=(int)((var.lng-buildGPS[2].lng)/scaleX);
	int y=(int)((var.lat-buildGPS[2].lat)/scaleY);
	x+=buildGPS[2].x;
	y+=buildGPS[2].y;
	var=COMPUTE_GPS(x
					,y
					,var.lng
					,var.lat);
	return ;
}

//��ָ���ļ��־û���
void NJUSTMap::serial(CFile &file){
	unsigned int i,j;
	unsigned int len=roads.size();
	int id;
	MAP_CPOINT pt(0,0);


	// --- Step.1 --- д��roads��Ϣ
	//д��roads����
	file.Write(&len,sizeof(unsigned int));
	for(i=0;i<roads.size();i++){
		len=roads[i].pInLine.size();
		//д��roads[i]�ĵ㼯����
		file.Write(&len,sizeof(unsigned int));
		for(j=0;j<roads[i].pInLine.size();j++){
			//д��roads[i]�ĵ㼯
			pt.x=roads[i].pInLine[j].x;
			pt.y=roads[i].pInLine[j].y;
			file.Write(&pt,sizeof(MAP_CPOINT)); 
		}
		//д��roads[i]��λ��
		pt.x=roads[i].position.x;
		pt.y=roads[i].position.y;
		file.Write(&pt,sizeof(MAP_CPOINT)); 
		//д��roads[i]�ĵ�·�ṹ
		file.Write(&(roads[i].road),sizeof(MAP_ROAD)); 
	}

	// --- Step.2---д��nodes�ṹ
	//д��nodes����
	len=nodes.size();
	file.Write(&len,sizeof(unsigned int));
	for(i=0;i<nodes.size();i++){
		//д��nodes[i]�ĵ�
		pt.x=nodes[i].position.x;
		pt.y=nodes[i].position.y;
		file.Write(&pt,sizeof(MAP_CPOINT)); 
		//д��roads[i]�ĵ�·�ṹ
		file.Write(&(nodes[i].node),sizeof(MAP_NODE)); 
	}
	// --- Step.3 --- д��cross�ṹ
	len=crosses.size();
	file.Write(&len,sizeof(unsigned int));
	for(i=0;i<crosses.size();i++){
		//д��ID
		id=crosses[i].idstart;
		file.Write(&id,sizeof(int));
		id=crosses[i].idend;
		file.Write(&id,sizeof(int));

		len=crosses[i].points.size();
		//д��crosses[i]�ĵ㼯����
		file.Write(&len,sizeof(unsigned int));
		for(j=0;j<crosses[i].points.size();j++){
			//д��crosses[i]�ĵ㼯
			pt.x=crosses[i].points[j].x;
			pt.y=crosses[i].points[j].y;
			file.Write(&pt,sizeof(MAP_CPOINT)); 
		}
	}

	// --- Step.4 ---д���ͼ�ϰ�������
	len=obstacles.size();
	file.Write(&len,sizeof(unsigned int));
	for(i=0;i<len;i++){
		//д��roads[i]�ĵ�·�ṹ
		file.Write(&(obstacles[i]),sizeof(CREATE_MAP_OBSTACLE)); 
	}

	// --- Step.5 ---д���ͼ�������
	file.Write(&buildGPS[0],sizeof(COMPUTE_GPS)); 
	file.Write(&buildGPS[1],sizeof(COMPUTE_GPS)); 
	file.Write(&buildGPS[2],sizeof(COMPUTE_GPS)); 

	file.Write(&scaleX,sizeof(double)); 
	file.Write(&scaleY,sizeof(double)); 

}

void NJUSTMap::serial2DB(MYSQL *conn,unsigned int mapID){
	auto winroad=WinRoadDAO::getInstance();
	winroad->init(conn);
	auto winnode=WinNodeDAO::getInstance();
	winnode->init(conn);
	auto wincross=WinCrossDAO::getInstance();
	wincross->init(conn);
	auto winobs=WinObsDAO::getInstance();
	winobs->init(conn);
	auto wincail=WinCailbrationDAO::getInstance();
	wincail->init(conn);

	//Step 0 -----------ɾ��ԭ����ؼ�¼--------------
	winroad->deleteAllByMapID(mapID);
	winnode->deleteAllByMapID(mapID);
	wincross->deleteEntitiesByMapID(mapID);
	winobs->deleteAllByMapID(mapID);
	wincail->deleteEntityByKey(mapID);

	//Step 1 ------------�����·��Ϣ-------------
	for(auto &road:roads){
		MODEL_WINROAD model;
		model.mapRoad=road;
		winroad->insertEntity(model,mapID);	
	}

	//Step 2 -----------����ڵ���Ϣ--------------
	for(auto &node:nodes){
		MODEL_WINNODE model;
		model.mapNode=node;
		winnode->insertEntity(model,mapID);	
	}

	//Step 3 -----------����·����Ϣ-------------- 
	for(auto &cross:crosses){
		wincross->insertEntity(cross,mapID);	
	}

	//Step 4 -----------�����ϰ�����Ϣ--------------
	for(auto &obs:obstacles){
		winobs->insertEntity(obs,mapID);	
	}

	//Step 5 -----------����궨��Ϣ--------------
	MODEL_WINCALIBRATION model;
	model.mapID=mapID;
	model.scaleX=scaleX;
	model.scaleY=scaleY;
	for(int i=0;i<3;i++)
		model.buildGPS[i]=buildGPS[i];
	wincail->insertEntity(model);
}

//�����ݿ��ѯ��ȡ����
void NJUSTMap::enserial2DB(MYSQL *conn,unsigned int mapID){
	int  i;
	auto winroad=WinRoadDAO::getInstance();
	winroad->init(conn);
	auto winnode=WinNodeDAO::getInstance();
	winnode->init(conn);
	auto wincross=WinCrossDAO::getInstance();
	wincross->init(conn);
	auto winobs=WinObsDAO::getInstance();
	winobs->init(conn);
	auto wincail=WinCailbrationDAO::getInstance();
	wincail->init(conn);

	//Step 0 -----------���ԭ����Ϣ--------------
	init();

	//Step 1 -----------��ȡ��·��Ϣ--------------
	winroad->getEntitiesByMapID(mapID,roads);

	//Step 2 -----------��ȡ�ڵ���Ϣ--------------
	winnode->getEntitiesByMapID(mapID,nodes);

	//Step 3 ------------��ȡ·����Ϣ-------------
	wincross->getEntitiesByMapID(mapID,crosses);

	//Step 4 -----------��ȡ�ϰ�����Ϣ--------------
	winobs->getEntitiesByMapID(mapID,obstacles);

	//Step 5 -----------��ȡ��ͼУ׼��Ϣ--------------
	MODEL_WINCALIBRATION wcail= wincail->getEntityByMapID(mapID);
	for (i=0;i<3;i++)
	{
		buildGPS[i]=wcail.buildGPS[i];
	}
	scaleX=wcail.scaleX;
	scaleY=wcail.scaleY;
}
//�ú�����û���ļ������� TODO���
void NJUSTMap::enserial(CFile& file){
	unsigned int i,j;
	unsigned int len=0;
	unsigned int len1=0;
	MAP_CPOINT pt(0,0);

	//ɾ��ԭ��������
	roads.clear();
	nodes.clear();
	crosses.clear();
	file.Read(&len,sizeof(unsigned int));
	// --- Step.1 --- ��ȡroads����
	for(i=0;i<len;i++){
		//��ȡroads[i]�ĵ㼯����
		CREATE_MAP_ROAD troad;
		file.Read(&len1,sizeof(unsigned int));
		for(j=0;j<len1;j++){
			//��ȡroads[i]�ĵ㼯
			file.Read(&pt,sizeof(MAP_CPOINT)); 
			troad.pInLine.push_back(CPoint(pt.x,pt.y));
		}
		//��ȡ��·λ��
		file.Read(&pt,sizeof(MAP_CPOINT)); 
		troad.position=CPoint(pt.x,pt.y);
		//��ȡroads[i]�ĵ�·�ṹ
		file.Read(&(troad.road),sizeof(MAP_ROAD)); 
		//��ӵ��б���
		roads.push_back(troad);
	}

	// --- Step.2 ---��ȡnodes����
	file.Read(&len,sizeof(unsigned int));
	for(i=0;i<len;i++){
		CREATE_MAP_NODE tnode;
		//��ȡnodes[i]�ĵ�
		file.Read(&pt,sizeof(MAP_CPOINT)); 
		tnode.position=CPoint(pt.x,pt.y);
		//��ȡroads[i]�ĵ�·�ṹ
		file.Read(&(tnode.node),sizeof(MAP_NODE)); 
		nodes.push_back(tnode);
	}

	// --- Step.3 --- ��ȡcroesses������
	file.Read(&len,sizeof(unsigned int));
	for(i=0;i<len;i++){
		//��ȡID
		CREATE_MAP_CROSS tcross;
		file.Read(&tcross.idstart,sizeof(int));
		file.Read(&tcross.idend,sizeof(int));

		file.Read(&len1,sizeof(unsigned int));
		for(j=0;j<len1;j++){
			//��ȡcrosses[i]�ĵ㼯
			file.Read(&pt,sizeof(MAP_CPOINT)); 
			tcross.points.push_back(CPoint(pt.x,pt.y));
		}
		crosses.push_back(tcross);
	}

	// --- Step.4 --- ��ȡ���ݵ�ͼת������
	CREATE_MAP_OBSTACLE tobs;
	file.Read(&len,sizeof(unsigned int));
	for(i=0;i<len;i++){
		//д��roads[i]�ĵ�·�ṹ
		file.Read(&tobs,sizeof(CREATE_MAP_OBSTACLE));
		obstacles.push_back(tobs);
	}

	// --- Step.5 --- ��ȡ���ݵ�ͼת������
	file.Read(&buildGPS[0],sizeof(COMPUTE_GPS)); 
	file.Read(&buildGPS[1],sizeof(COMPUTE_GPS)); 
	file.Read(&buildGPS[2],sizeof(COMPUTE_GPS)); 
	//DEBUG ɾ����仰������
	//buildGPS[2]=buildGPS[0];

	file.Read(&scaleX,sizeof(double)); 
	file.Read(&scaleY,sizeof(double)); 
}



//��ָ���ļ�����������
bool NJUSTMap::saveBuildMapForLinux(CString filename){
	MAP_BUILD_FILE_HEAD mapHead;
	int x,y;			//ת���Ĵ������
	ToolsUtil::GPS2Earthy(buildGPS[0].lat,buildGPS[0].lng,x,y);
	//�������� ������ֹ�������
	mapHead.m_adjustx=x;
	mapHead.m_adjusty=y;

    //����Ԫ�ظ���
	mapHead.linecounter=roads.size();
	mapHead.notecounter=nodes.size();
	mapHead.obstaclecounter=obstacles.size();

	CFile file;
	BOOL isOpen=file.Open(filename,CFile::modeCreate|CFile::modeWrite);
	//�����ļ��ɹ�
	if(isOpen){
		 //д���ļ�ͷ
		file.Write(&mapHead,sizeof(MAP_BUILD_FILE_HEAD));             

		//д��ڵ�����
		COMPUTE_GPS cgps;
		int ex,ey;//�������
		for(auto &node:nodes){
			//����ת����GPS�ʹ������
			cgps.x=node.position.x;
			cgps.y=node.position.y;
			pixel2GPS(cgps);
			ToolsUtil::GPS2Earthy(cgps.lat,cgps.lng,ex,ey);

			//��д
			node.node.earthx=ex;
			node.node.earthy=ey;
			node.node.gpsx=cgps.lng;
			node.node.gpsy=cgps.lat;
			file.Write(&node.node,sizeof(MAP_NODE));
		}

		//д���·����
		for(auto &road:roads){
			//����ֱ��
			getLineFunctionPara(road.road.idstart,
								road.road.idend,
								road.road.k,
								road.road.b,
								road.road.c);
			file.Write(&road.road,sizeof(MAP_ROAD));
		}

		for(auto &obstacle:obstacles){
			cgps.x=obstacle.x;  //ԭ������
			cgps.y=obstacle.y;		
			pixel2GPS(cgps);
			obstacle.ob.ObstacleCenterGPS.longtitude=cgps.lng;
			obstacle.ob.ObstacleCenterGPS.latitude=cgps.lat;

			cgps.x=obstacle.x+obstacle.rPix;  //��������
			cgps.y=obstacle.y+obstacle.rPix;
			pixel2GPS(cgps);
			double disM=ToolsUtil::GetDistanceByGPS(obstacle.ob.ObstacleCenterGPS.longtitude,
										obstacle.ob.ObstacleCenterGPS.latitude,
										cgps.lng,
										cgps.lat);
			obstacle.ob.RadialCM=disM*100;  //������뾶���� 
			file.Write(&obstacle.ob,sizeof(NJUST_MAP_OBSTACLE));
		}

		file.Close();
		return true;
	}
	return false;
}

//����ڵ�֮����ڽӾ���  �ú����������saveBuildMapForLinux��
bool NJUSTMap::saveAdjForLinux(CString filename){
	int nodeLen=nodes.size();
	int* adj=new int[nodeLen*nodeLen];
	for(int i=0;i<nodeLen*nodeLen;i++){
		adj[i]=INF;
	}

	for(int i=0;i<nodeLen;i++){
		int ex1=nodes[i].node.earthx;
		int ey1=nodes[i].node.earthy;
		for(int j=0;j<nodes[i].node.neigh;j++){
			int id=nodes[i].node.NeighNoteID[j];
			int index2=-1;
		    for(int k=0;k<nodeLen;k++){
				if(nodes[k].node.idself==id){
					index2=k;break;
				}
			}
			int ex2=nodes[index2].node.earthx;
			int ey2=nodes[index2].node.earthy;
			int dx=(ex1-ex2)/100;            //��λת����
			int dy=(ey1-ey2)/100;
			int dis=sqrt(dx*dx+dy*dy);

			adj[i*nodeLen+index2]=dis;
			
		}
	}
	CFile file;
	BOOL isOpen=file.Open(filename,CFile::modeCreate|CFile::modeWrite);
	if(isOpen){
		for(int i=0;i<nodeLen*nodeLen;i++){
			file.Write(&adj[i],sizeof(int));
		}
		file.Close();
	}
	delete []adj;
	return isOpen;

}


void NJUSTMap::getLineFunctionPara(int startID,int endStart,double &k,double &b,double &c){

	CPoint startpoint,endpoint;
	int x,y;			//ת���Ĵ������
	ToolsUtil::GPS2Earthy(buildGPS[0].lat,buildGPS[0].lng,x,y); //�ѵ�һ����Ϊ������

	
	startpoint.x=nodes[startID-START_NODE_ID].node.earthx-x;
	startpoint.y=nodes[startID-START_NODE_ID].node.earthy-y;
	endpoint.x=nodes[endStart-START_NODE_ID].node.earthx-x;
	endpoint.y=nodes[endStart-START_NODE_ID].node.earthy-y;
	if (startpoint.x==endpoint.x)//��ֱ��x��
	{
		k=1;
		b=0;
    	c=-endpoint.x;
	}
	else if(startpoint.y==endpoint.y)//��ֱ��y��
	{
	    k=0;
		b=1;
		c=-startpoint.y;
	}
	else
	{
		k=(double)min((endpoint.y-startpoint.y)/(endpoint.x-startpoint.x),999999);
		b=-1;
		c=(double)startpoint.y-k*startpoint.x;
	}	
	return ;

}

int NJUSTMap::getIndexByGPS(double lng,double lat,double dis){
	double minDlen=100;//���ֵ���ᳬ��100
	double thres=dis/100000; //��->��
	thres=thres*thres;
	int index; //���������
	int k=0;
	for(auto &node:nodes){
		double dlng=node.node.gpsx-lng;
		double dlat=node.node.gpsy-lat;
		double dlen2=dlat*dlat+dlng*dlng;
		if(dlen2<minDlen){  //
			minDlen=dlen2;
			index=k;
		}
		k++;
	}
	if(minDlen<thres){ //0.001 == 2��  40��
		return nodes[index].node.idself-START_NODE_ID+1;
	}
	return -1;
}


//���ų��ض��������� ����dis��Χ�ڵ�����ڵ�
int NJUSTMap::getNodeIndexByGPSWithoutV(double lng,double lat,double dis,vector<MAP_TASK_NODE_ZZ> v){
	double minDlen=100;//���ֵ���ᳬ��100
	double thres=dis/100000; //��->��
	thres=thres*thres;
	int index; //���������
	int k=0;
	for(auto &node:nodes){
		bool isHave=false;
		for(auto &tv:v){
			if(node.node.idself-START_NODE_ID+1==tv.resultCode){//�Ѿ���·����
				isHave=true;break;
			}
		}
		if(isHave){ //����������
			k++;
			continue;
		}
		double dlng=node.node.gpsx-lng;
		double dlat=node.node.gpsy-lat;
		double dlen2=dlat*dlat+dlng*dlng;
		if(dlen2<minDlen){  //
			minDlen=dlen2;
			index=k;
		}
		k++;
	}
	if(minDlen<thres){ 
		return nodes[index].node.idself-START_NODE_ID+1;
	}
	return -1;
}