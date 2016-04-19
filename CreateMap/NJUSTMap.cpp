#include "stdafx.h"
#include "NJUSTMap.h"
#include <stdio.h>


void NJUSTMap::init(COMPUTE_GPS buildGPS[2]){
	this->buildGPS[0]=buildGPS[0];
	this->buildGPS[1]=buildGPS[1];
	//buildGPS.x=.y=0
	scaleX=(buildGPS[1].lng-buildGPS[0].lng)/buildGPS[1].x;
	scaleY=(buildGPS[1].lat-buildGPS[0].lat)/buildGPS[1].y;
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
	road.road.idstart=line2ID.x+START_NODE_ID;
	road.road.idend=line2ID.y+START_NODE_ID;
	

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


CString NJUSTMap::printRoad(unsigned int index)const{
	CString strShow;
	strShow.Format(L"%d:%d->%d",roads[index].road.idself,
								roads[index].road.idstart,
								roads[index].road.idend); 
	return strShow;
}

CString NJUSTMap::printRoadBack()const{
	return printRoad(roads.size()-1);
}


bool NJUSTMap::writeRoad(CString path){

	unsigned int i,j;
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
				file.Write(&outpoint,sizeof(MAP_DOUBLE_POINT));
			}
			file.Close();
		}	
	}
	return true;
}

void NJUSTMap::pixel2GPS(COMPUTE_GPS &var){
	var=COMPUTE_GPS(var.x,
					var.y
					,var.x*scaleX+buildGPS[0].lng
					,var.y*scaleY+buildGPS[0].lat);
	return ;
}