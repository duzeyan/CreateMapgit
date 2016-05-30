#include "stdafx.h"
#include "NJUSTMap.h"
#include <stdio.h>
#include <afx.h>
#include <iomanip>
#include"ToolsUtil.h"
using namespace std;



NJUSTMap::NJUSTMap(){
	buildGPS[0].x=buildGPS[0].y=0.0;
	buildGPS[1].x=buildGPS[1].y=0.0;
	buildGPS[2].x=buildGPS[2].y=0.0;
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

//计算尺度
void NJUSTMap::computeScale(){
	scaleX=(buildGPS[1].lng-buildGPS[0].lng)/(buildGPS[1].x-buildGPS[0].x);
	scaleY=(buildGPS[1].lat-buildGPS[0].lat)/(buildGPS[1].y-buildGPS[0].y);

	//两两计算尺度
	/*double scaleX01=(buildGPS[1].lng-buildGPS[0].lng)/(buildGPS[1].x-buildGPS[0].x);
	double scaleX12=(buildGPS[1].lng-buildGPS[2].lng)/(buildGPS[1].x-buildGPS[2].x);
	double scaleX02=(buildGPS[0].lng-buildGPS[2].lng)/(buildGPS[0].x-buildGPS[2].x);

	double scaleY01=(buildGPS[1].lat-buildGPS[0].lat)/(buildGPS[1].y-buildGPS[0].y);
	double scaleY12=(buildGPS[1].lat-buildGPS[2].lat)/(buildGPS[1].y-buildGPS[2].y);
	double scaleY02=(buildGPS[0].lat-buildGPS[2].lat)/(buildGPS[0].y-buildGPS[2].y);
	scaleX=(scaleX01+scaleX02+scaleX12)/3;
	scaleY=(scaleY01+scaleY02+scaleY12)/3;*/
}

//检查是否已经设置好参数
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

//为了简化过程 默认是按照顺序连接 !!!所以绘图时一定要按照头尾顺序绘制!!! ID向量也是有序的,不可错分
bool NJUSTMap::merge2Line(const vector<DRAW_RECORD> &mergeV,MAP_ROAD package){
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
				if(j!=mergeV[i].drawPoints.size()+MAP_DASH_NUM-1) //保证线段首尾都有
					road.pInLine.push_back(mergeV[i].drawPoints.back());
				break;
			 }
		//points
		case 2:{
				for(j=0;j<mergeV[i].drawPoints.size();j++)
					road.pInLine.push_back(mergeV[i].drawPoints[j]);
				break;
			}
		//贝塞尔曲线
		case 1:{
				for(j=0;j<mergeV[i].drawPoints.size();j+=MAP_DASH_NUM)
					road.pInLine.push_back(mergeV[i].drawPoints[j]);
				if(j!=mergeV[i].drawPoints.size()+MAP_DASH_NUM-1) //保证线段首尾都有
					road.pInLine.push_back(mergeV[i].drawPoints.back());
				break;
			   }
		default:
				return false;
			break;
		}
	}
	//补充设置ID
	if(roads.empty()){
		package.idself=START_LINE_ID;
	}else{
		package.idself=roads.back().road.idself+1;
	}
	package.idstart=package.idstart+START_NODE_ID-1;
	package.idend=package.idend+START_NODE_ID-1;

	//全部赋值存在地图结构中
	road.road=package;

	//更新 如果之前有相同元素则剔除
	for(i=0;i<roads.size();i++){
		if(roads[i].road.idstart==road.road.idstart && roads[i].road.idend==road.road.idend){
			roads.erase(roads.begin()+i);
			break;
		}
	}
	roads.push_back(road);

	//设置 相应节点的邻接ID
	for( int i=0;i<nodes.size();i++){
		if(nodes[i].node.idself==package.idstart){ //设置出发路口 
			  int nindex=nodes[i].node.neigh;
			  nodes[i].node.NeighLineID[nindex]=package.idself;
			  nodes[i].node.NeighNoteID[nindex]=package.idend;
			  nodes[i].node.neigh++;
			  assert(nodes[i].node.neigh<4); //最多四个路口
		}
		if(nodes[i].node.idself==package.idend){
			  int nindex=nodes[i].node.neigh;
			  nodes[i].node.NeighLineID[nindex]=package.idself;
			  nodes[i].node.NeighNoteID[nindex]=package.idstart;
			  nodes[i].node.neigh++;
			  assert(nodes[i].node.neigh<4); //最多四个路口
		}
	}
	return true;
}

void NJUSTMap::removeObstaclesByID(int id){
	auto it=find(obstacles.begin(),obstacles.end(),id);
	assert(it!=obstacles.end());
	obstacles.erase(it);
}

void NJUSTMap::deleteRoadByIndex(int index){
	//获取要删除道路的ID
	int id=roads[index].road.idself;

	for(auto &tnode:nodes){
		int  whichIndex=-1;
		for(int i=0;i<tnode.node.neigh;i++){
			if(tnode.node.NeighLineID[i]==id){ //如果存在 则删除
				whichIndex=i;
				break;        //不存在重复道路
			}
		}
		if(whichIndex!=-1){ //发现了关联该道路
			for(int i=whichIndex;i<tnode.node.neigh-1;i++){ //删除
				tnode.node.NeighLineID[i]=tnode.node.NeighLineID[i+1];
			}
			tnode.node.neigh--;
		}
	}
	//删除该道路
	roads.erase(roads.begin()+index);
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
		//贝塞尔曲线
		case 1:{
				for(j=0;j<mergeV[i].drawPoints.size();j+=MAP_DASH_NUM)
					cross.points.push_back(mergeV[i].drawPoints[j]);
				if(j!=mergeV[i].drawPoints.size()+MAP_DASH_NUM-1) //保证线段首尾都有
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
	strShow.Format(L"路口:道路%d->道路%d",
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
				sprintf(buf,"%.10lf %.10lf \r\n",outpoint.x,outpoint.y);  
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
		if(file.Open(filename,CFile::modeCreate|CFile::modeWrite)){
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
		}
		file.Close();
	}
	return true;
}

//buildGPS[2]为基准点
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

//在指定文件持久化类
void NJUSTMap::serial(CFile &file){
	unsigned int i,j;
	unsigned int len=roads.size();
	int id;
	MAP_CPOINT pt(0,0);


	// --- Step.1 --- 写入roads信息
	//写入roads长度
	file.Write(&len,sizeof(unsigned int));
	for(i=0;i<roads.size();i++){
		len=roads[i].pInLine.size();
		//写入roads[i]的点集长度
		file.Write(&len,sizeof(unsigned int));
		for(j=0;j<roads[i].pInLine.size();j++){
			//写入roads[i]的点集
			pt.x=roads[i].pInLine[j].x;
			pt.y=roads[i].pInLine[j].y;
			file.Write(&pt,sizeof(MAP_CPOINT)); 
		}
		//写入roads[i]的位置
		pt.x=roads[i].position.x;
		pt.y=roads[i].position.y;
		file.Write(&pt,sizeof(MAP_CPOINT)); 
		//写入roads[i]的道路结构
		file.Write(&(roads[i].road),sizeof(MAP_ROAD)); 
	}

	// --- Step.2---写入nodes结构
	//写入nodes长度
	len=nodes.size();
	file.Write(&len,sizeof(unsigned int));
	for(i=0;i<nodes.size();i++){
		//写入nodes[i]的点
		pt.x=nodes[i].position.x;
		pt.y=nodes[i].position.y;
		file.Write(&pt,sizeof(MAP_CPOINT)); 
		//写入roads[i]的道路结构
		file.Write(&(nodes[i].node),sizeof(MAP_NODE)); 
	}
	// --- Step.3 --- 写入cross结构
	len=crosses.size();
	file.Write(&len,sizeof(unsigned int));
	for(i=0;i<crosses.size();i++){
		//写入ID
		id=crosses[i].idstart;
		file.Write(&id,sizeof(int));
		id=crosses[i].idend;
		file.Write(&id,sizeof(int));

		len=crosses[i].points.size();
		//写入crosses[i]的点集长度
		file.Write(&len,sizeof(unsigned int));
		for(j=0;j<crosses[i].points.size();j++){
			//写入crosses[i]的点集
			pt.x=crosses[i].points[j].x;
			pt.y=crosses[i].points[j].y;
			file.Write(&pt,sizeof(MAP_CPOINT)); 
		}
	}

	// --- Step.4 ---写入地图障碍物序列
	len=obstacles.size();
	file.Write(&len,sizeof(unsigned int));
	for(i=0;i<len;i++){
		//写入roads[i]的道路结构
		file.Write(&(obstacles[i]),sizeof(CREATE_MAP_OBSTACLE)); 
	}

	// --- Step.5 ---写入地图换算参数
	file.Write(&buildGPS[0],sizeof(COMPUTE_GPS)); 
	file.Write(&buildGPS[1],sizeof(COMPUTE_GPS)); 
	file.Write(&buildGPS[2],sizeof(COMPUTE_GPS)); 

	file.Write(&scaleX,sizeof(double)); 
	file.Write(&scaleY,sizeof(double)); 

}

//该函数内没有文件检测机制 TODO添加
void NJUSTMap::enserial(CFile& file){
	unsigned int i,j;
	unsigned int len=0;
	unsigned int len1=0;
	MAP_CPOINT pt(0,0);

	//删除原来的内容
	roads.clear();
	nodes.clear();
	crosses.clear();
	file.Read(&len,sizeof(unsigned int));
	// --- Step.1 --- 获取roads内容
	for(i=0;i<len;i++){
		//获取roads[i]的点集长度
		CREATE_MAP_ROAD troad;
		file.Read(&len1,sizeof(unsigned int));
		for(j=0;j<len1;j++){
			//获取roads[i]的点集
			file.Read(&pt,sizeof(MAP_CPOINT)); 
			troad.pInLine.push_back(CPoint(pt.x,pt.y));
		}
		//获取道路位置
		file.Read(&pt,sizeof(MAP_CPOINT)); 
		troad.position=CPoint(pt.x,pt.y);
		//读取roads[i]的道路结构
		file.Read(&(troad.road),sizeof(MAP_ROAD)); 
		//添加到列表中
		roads.push_back(troad);
	}

	// --- Step.2 ---获取nodes内容
	file.Read(&len,sizeof(unsigned int));
	for(i=0;i<len;i++){
		CREATE_MAP_NODE tnode;
		//获取nodes[i]的点
		file.Read(&pt,sizeof(MAP_CPOINT)); 
		tnode.position=CPoint(pt.x,pt.y);
		//获取roads[i]的道路结构
		file.Read(&(tnode.node),sizeof(MAP_NODE)); 
		nodes.push_back(tnode);
	}

	// --- Step.3 --- 获取croesses的内容
	file.Read(&len,sizeof(unsigned int));
	for(i=0;i<len;i++){
		//获取ID
		CREATE_MAP_CROSS tcross;
		file.Read(&tcross.idstart,sizeof(int));
		file.Read(&tcross.idend,sizeof(int));

		file.Read(&len1,sizeof(unsigned int));
		for(j=0;j<len1;j++){
			//获取crosses[i]的点集
			file.Read(&pt,sizeof(MAP_CPOINT)); 
			tcross.points.push_back(CPoint(pt.x,pt.y));
		}
		crosses.push_back(tcross);
	}

	// --- Step.4 --- 获取内容地图转化参数
	CREATE_MAP_OBSTACLE tobs;
	file.Read(&len,sizeof(unsigned int));
	for(i=0;i<len;i++){
		//写入roads[i]的道路结构
		file.Read(&tobs,sizeof(CREATE_MAP_OBSTACLE));
		obstacles.push_back(tobs);
	}

	// --- Step.5 --- 获取内容地图转化参数
	file.Read(&buildGPS[0],sizeof(COMPUTE_GPS)); 
	file.Read(&buildGPS[1],sizeof(COMPUTE_GPS)); 
	file.Read(&buildGPS[2],sizeof(COMPUTE_GPS)); 
	//DEBUG 删除这句话！！！
	//buildGPS[2]=buildGPS[0];

	file.Read(&scaleX,sizeof(double)); 
	file.Read(&scaleY,sizeof(double)); 
}



//在指定文件中输入内容
bool NJUSTMap::saveBuildMapForLinux(CString filename){
	MAP_BUILD_FILE_HEAD mapHead;
	int x,y;			//转化的大地坐标
	ToolsUtil::GPS2Earthy(buildGPS[0].lat,buildGPS[0].lng,x,y);
	//中心坐标 用来防止数据溢出
	mapHead.m_adjustx=x;
	mapHead.m_adjusty=y;

    //设置元素个数
	mapHead.linecounter=roads.size();
	mapHead.notecounter=nodes.size();
	mapHead.obstaclecounter=obstacles.size();

	CFile file;
	BOOL isOpen=file.Open(filename,CFile::modeCreate|CFile::modeWrite);
	//创建文件成功
	if(isOpen){
		 //写入文件头
		file.Write(&mapHead,sizeof(MAP_BUILD_FILE_HEAD));             

		//写入节点序列
		COMPUTE_GPS cgps;
		int ex,ey;//大地坐标
		for(auto &node:nodes){
			//坐标转化成GPS和大地坐标
			cgps.x=node.position.x;
			cgps.y=node.position.y;
			pixel2GPS(cgps);
			ToolsUtil::GPS2Earthy(cgps.lat,cgps.lng,ex,ey);

			//填写
			node.node.earthx=ex;
			node.node.earthy=ey;
			node.node.gpsx=cgps.lng;
			node.node.gpsy=cgps.lat;
			file.Write(&node.node,sizeof(MAP_NODE));
		}

		//写入道路序列
		for(auto &road:roads){
			//计算直线
			getLineFunctionPara(road.road.idstart,
								road.road.idend,
								road.road.k,
								road.road.b,
								road.road.c);
			file.Write(&road.road,sizeof(MAP_ROAD));
		}

		for(auto &obstacle:obstacles){
			cgps.x=obstacle.x;  //原点坐标
			cgps.y=obstacle.y;		
			pixel2GPS(cgps);
			obstacle.ob.ObstacleCenterGPS.longtitude=cgps.lng;
			obstacle.ob.ObstacleCenterGPS.latitude=cgps.lat;

			cgps.x=obstacle.x+obstacle.rPix;  //左上坐标
			cgps.y=obstacle.y+obstacle.rPix;
			pixel2GPS(cgps);
			double disM=ToolsUtil::GetDistanceByGPS(obstacle.ob.ObstacleCenterGPS.longtitude,
										obstacle.ob.ObstacleCenterGPS.latitude,
										cgps.lng,
										cgps.lat);
			obstacle.ob.RadialCM=disM*100;  //计算出半径长度 
			file.Write(&obstacle.ob,sizeof(NJUST_MAP_OBSTACLE));
		}

		file.Close();
		return true;
	}
	return false;
}

//保存节点之间的邻接矩阵  该函数必须跟在saveBuildMapForLinux后
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
			int dx=(ex1-ex2)/100;            //单位转成米
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
	int x,y;			//转化的大地坐标
	ToolsUtil::GPS2Earthy(buildGPS[0].lat,buildGPS[0].lng,x,y); //已第一个点为基础点

	
	startpoint.x=nodes[startID-START_NODE_ID].node.earthx-x;
	startpoint.y=nodes[startID-START_NODE_ID].node.earthy-y;
	endpoint.x=nodes[endStart-START_NODE_ID].node.earthx-x;
	endpoint.y=nodes[endStart-START_NODE_ID].node.earthy-y;
	if (startpoint.x==endpoint.x)//垂直于x轴
	{
		k=1;
		b=0;
    	c=-endpoint.x;
	}
	else if(startpoint.y==endpoint.y)//垂直于y轴
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

int NJUSTMap::getIndexByGPS(double lng,double lat){
	double minDlen=100;//最大值不会超过100
	int index; //最近的索引
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
	if(minDlen<0.005*0.005){ //0.001 == 2米  10米
		return nodes[index].node.idself-START_NODE_ID+1;
	}
	return -1;
}