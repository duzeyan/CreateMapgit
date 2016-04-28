#include "stdafx.h"
#include "NJUSTMap.h"
#include <stdio.h>
#include <afx.h>
#include <iomanip>
using namespace std;

void NJUSTMap::init(COMPUTE_GPS buildGPS[2]){
	this->buildGPS[0]=buildGPS[0];
	this->buildGPS[1]=buildGPS[1];

	scaleX=(buildGPS[1].lng-buildGPS[0].lng)/(buildGPS[1].x-buildGPS[0].x);
	scaleY=(buildGPS[1].lat-buildGPS[0].lat)/(buildGPS[1].y-buildGPS[0].y);
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

//为了简化过程 默认是按照顺序连接 !!!所以绘图时一定要按照头尾顺序绘制!!! ID向量也是有序的,不可错分
bool NJUSTMap::merge2Line(const vector<DRAW_RECORD> &mergeV,CPoint line2ID){
	unsigned int i,j;
	CREATE_MAP_ROAD road;
	//目前只支持直线和断点的连接 设置线上内容
	for(i=0;i<mergeV.size();i++){
		switch (mergeV[i].type)
		{
	     //line
		case 0:{
				for(j=0;j<mergeV[i].drawPoints.size();j+=MAP_DASH_NUM)
					road.pInLine.push_back(mergeV[i].drawPoints[j]);
				break;
			 }
		//points
		case 2:{
				for(j=0;j<mergeV[i].drawPoints.size();j++)
					road.pInLine.push_back(mergeV[i].drawPoints[j]);
				break;
			}
		default:
				return false;
			break;
		}
	}
	if(roads.empty()){
		road.road.idself=START_LINE_ID;
	}else{
		road.road.idself=roads.back().road.idself+1;
	}
	road.road.idstart=line2ID.x+START_NODE_ID-1;
	road.road.idend=line2ID.y+START_NODE_ID-1;

	//更新 如果之前有相同元素则剔除
	for(i=0;i<roads.size();i++){
		if(roads[i].road.idstart==road.road.idstart && roads[i].road.idend==road.road.idend){
			roads.erase(roads.begin()+i);
			break;
		}
	}
	roads.push_back(road);
	return true;
}

//生成路口
bool NJUSTMap::merge2Cross(const vector<DRAW_RECORD> &mergeV,CPoint line2ID){
	unsigned int i,j;
	CREATE_MAP_CROSS cross;
	//目前只支持直线和断点的连接 设置线上内容
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
		default:
				return false;
			break;
		}
	}

	cross.idstart=line2ID.x+START_LINE_ID-1;
	cross.idend=line2ID.y+START_LINE_ID-1;

	//更新 如果之前有相同元素则剔除
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
	strShow.Format(L"道路%d:路口%d->路口%d",roads[index].road.idself-START_LINE_ID+1,
								roads[index].road.idstart-START_NODE_ID+1,
								roads[index].road.idend-START_NODE_ID+1); 
	return strShow;
}

CString NJUSTMap::printCross(unsigned int index)const{
	CString strShow;
	strShow.Format(L"路口:道路%d->路口%d",
								crosses[index].idstart-START_LINE_ID+1,
								crosses[index].idend-START_LINE_ID+1); 
	return strShow;
}



CString NJUSTMap::printRoadBack()const{
	return printRoad(roads.size()-1);
}


bool NJUSTMap::writeRoad(CString path){

	unsigned int i,j;
	//保存道路
	for(i=0;i<roads.size();i++){
		CString filename;
		filename.Format(L"%s\\%d-%d.db",path, 
								roads[i].road.idstart-START_NODE_ID+1,
								roads[i].road.idend-START_NODE_ID+1);
		
		CFile file;
		if(file.Open(filename,CFile::modeCreate|CFile::modeWrite)){
			for(j=0;j<roads[i].pInLine.size();j++){
				COMPUTE_GPS var(roads[i].pInLine[j].x,roads[i].pInLine[j].y,0.0,0.0);
				pixel2GPS(var);
				MAP_DOUBLE_POINT outpoint;
				outpoint.x=var.lng;outpoint.y=var.lat;
				outpoint.x*=60.0;outpoint.y*=60.0;     //转化为分
				file.Write(&outpoint,sizeof(MAP_DOUBLE_POINT));
			}
			file.Close();
		}	
	}
	//保存路口
	for(i=0;i<crosses.size();i++){
		CString filename;
		filename.Format(L"%s\\%d+%d.db",path, 
								crosses[i].idstart-START_LINE_ID+1,
								crosses[i].idend-START_LINE_ID+1);
		CFile file;
		if(file.Open(filename,CFile::modeCreate|CFile::modeWrite)){
			for(j=0;j<crosses[i].points.size();j++){
				COMPUTE_GPS var(crosses[i].points[j].x,crosses[i].points[j].y,0.0,0.0);
				pixel2GPS(var);
				MAP_DOUBLE_POINT outpoint;
				outpoint.x=var.lng;outpoint.y=var.lat;
				outpoint.x*=60.0;outpoint.y*=60.0;     //转化为分
				file.Write(&outpoint,sizeof(MAP_DOUBLE_POINT));
			}
			file.Close();
		}	
	}
	return true;
}


//在指定目录下保存文本文件
bool NJUSTMap::writeRoadTxt(CString path){
	unsigned int i,j;
	//保存道路
	for(i=0;i<roads.size();i++){
		CString filename;
		filename.Format(L"%s\\%d-%d.txt",path, 
								roads[i].road.idstart-START_NODE_ID+1,
								roads[i].road.idend-START_NODE_ID+1);
		
		CFile file;
		//CArchive ar(&file,CArchive::store);
		if(file.Open(filename,CFile::modeCreate|CFile::modeWrite)){
			for(j=0;j<roads[i].pInLine.size();j++){
				COMPUTE_GPS var(roads[i].pInLine[j].x,roads[i].pInLine[j].y,0.0,0.0);
				pixel2GPS(var);
				MAP_DOUBLE_POINT outpoint;
				outpoint.x=var.lng;outpoint.y=var.lat;
				//outpoint.x*=60.0;outpoint.y*=60.0;     //转化为分
				
				char buf[100];  
				sprintf(buf,"%.10lf %.10lf \n",outpoint.x,outpoint.y);  
				file.Write(buf,strlen(buf));
			}
		
			file.Close();
		}	
	}
	//保存路口
	for(i=0;i<crosses.size();i++){
		CString filename;
		filename.Format(L"%s\\%d+%d.txt",path, 
								crosses[i].idstart-START_LINE_ID+1,
								crosses[i].idend-START_LINE_ID+1);
		CFile file;
		CArchive ar(&file,CArchive::store);
		if(file.Open(filename,CFile::modeCreate|CFile::modeWrite|CFile::typeText)){
			for(j=0;j<crosses[i].points.size();j++){
				COMPUTE_GPS var(crosses[i].points[j].x,crosses[i].points[j].y,0.0,0.0);
				pixel2GPS(var);
				MAP_DOUBLE_POINT outpoint;
				outpoint.x=var.lng;outpoint.y=var.lat;
				//outpoint.x*=60.0;outpoint.y*=60.0;     //转化为分
				
				char buf[100];  
				sprintf(buf,"%.10lf %.10lf\r\n",outpoint.x,outpoint.y);  
				file.Write(buf,strlen(buf));
			}
			ar.Close();
			file.Close();
		}	
	}
	return true;
}


void NJUSTMap::pixel2GPS(COMPUTE_GPS &var){
	var=COMPUTE_GPS(var.x,
					var.y
					,(var.x-buildGPS[0].x)*1.0*scaleX+buildGPS[0].lng
					,(var.y-buildGPS[0].y)*1.0*scaleY+buildGPS[0].lat);
	return ;
}

void NJUSTMap::GPS2pexel(COMPUTE_GPS &var){
	int x=(int)((var.lng-buildGPS[0].lng)/scaleX);
	int y=(int)((var.lat-buildGPS[0].lat)/scaleY);
	x+=buildGPS[0].x;
	y+=buildGPS[0].y;
	var=COMPUTE_GPS(x
					,y
					,var.lng
					,var.lat);
	return ;
}