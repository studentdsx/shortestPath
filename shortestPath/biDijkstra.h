#ifndef _BIDIJKSTRA_FILE_
#define _BIDIJKSTRA_FILE_
#include "graph.h"
#include "biHeap.h"
#include "head.h"


class CBiDijkstra
	:public CGraph
{

public:
	CBiDijkstra(CString strFileNameNode, CString strFileNameEdge, GraphKind graphKind = unDirectGraph);
	~CBiDijkstra();

private:
	bool *frontVisit;
	bool *backVisit;
	double *frontDistance;			
	double *backDistance;
	int *frontNode;					//存储正向的前驱结点
	int *backNode;					//存储逆向的后继结点

	CString shortestPathNode;

	CBiHeap frontQueueSet;	//存储向前的label结点，即已访问但为求出最短路径的结点集
	CBiHeap backQueueSet;	//存储向后的label结点，即已访问但为求出最短路径的结点集

	void outPutShortestPathAndTime(const double *shortestPath, const double *queryTime);
	void generalPath(const int start, const int destination, const int middleNodeID);

private:
	inline void init(const int start, const int destination);
	inline void updateShortPath(CBiHeap &currentQueueSet, 
		int *pCurrentWay, double *pCurrentDistance, double *pNotCurrentDistance, 
		double &miniShortPathValue, int &middleNode, bool *pCurrentVisit);
	inline CString frontPath(int start, int middleNodeID);
	inline CString backPath(int destination, int middleNodeID);

public:
	inline double QueryShortestPath(const int start, const int destination);
	void outPutAllShortestPath();
	void testGroupShortesPath();

};

CBiDijkstra::CBiDijkstra(CString strFileNameNode, CString strFileNameEdge, GraphKind graphKind /* = unDirectGraph */)
	:CGraph(strFileNameNode, strFileNameEdge, graphKind)
{
	frontDistance = new double[totalNumOfNode];
	backDistance = new double[totalNumOfNode];
	frontNode = new int[totalNumOfNode];
	backNode = new int[totalNumOfNode];
	frontVisit = new bool[totalNumOfNode];
	backVisit = new bool[totalNumOfNode];
}

CBiDijkstra::~CBiDijkstra()
{
	delete []frontDistance;
	delete []backDistance;
	delete []frontNode;
	delete []backNode;
	delete []frontVisit;
	delete []backVisit;
}

void CBiDijkstra::init(const int start, const int destination)
{
	for (int i=0; i<totalNumOfNode; ++i)
	{
		//记录已经求出的最短路径
		frontDistance[i] = MAXVALUE;
		backDistance[i] = MAXVALUE;
		//记录最短路径前驱与后继
		frontNode[i] = -1;
		backNode[i] = -1;
		//
		frontVisit[i] = FALSE;
		backVisit[i] = FALSE;
	}
	frontNode[start] = start;
	backNode[destination] = destination;

	frontDistance[start] = 0.0;
	backDistance[destination] = 0.0;

	frontQueueSet.clear();
	backQueueSet.clear();

	frontQueueSet.push(0.0, start);
	backQueueSet.push(0.0, destination);
}

void CBiDijkstra::updateShortPath(CBiHeap &currentQueueSet,
	int *pCurrentWay, double *pCurrentDistance, double *pNotCurrentDistance, 
	double &miniShortPathValue, int &middleNode, bool *pCurrentVisit)
{
	CBiHeapPoint currentNode = currentQueueSet.top();
	currentQueueSet.pop();
	int currentNodeID = currentNode.getNodeID();
	double currentWeigth = currentNode.getWeight();

	pCurrentDistance[currentNodeID] = currentWeigth;
	pCurrentVisit[currentNodeID] = TRUE;

	arcNode *pArcNode = graphData[currentNodeID].firstArcNode;
	int arcID = -1;
	double currentArcWeight;
	while (pArcNode != NULL)
	{
		arcID = pArcNode->id;
		currentArcWeight = currentWeigth + pArcNode->weight;
		if (!pCurrentVisit[arcID] && pCurrentDistance[arcID] > currentArcWeight)
		{
			pCurrentDistance[arcID] = currentWeigth + pArcNode->weight;
			currentQueueSet.update(pCurrentDistance[arcID], arcID);

			pCurrentWay[arcID] = currentNodeID;

			if (miniShortPathValue > pCurrentDistance[arcID] + pNotCurrentDistance[arcID])    
			{
				miniShortPathValue = pCurrentDistance[arcID] + pNotCurrentDistance[arcID];
				middleNode = arcID;
			}
		}
// 		if (!pCurrentVisit[arcID])
// 		{
// 			if (pCurrentDistance[arcID] > currentWeigth + pArcNode->weight)
// 			{
// 				pCurrentWay[arcID] = currentNodeID;
// 				pCurrentDistance[arcID] = currentWeigth + pArcNode->weight;
// 				currentQueueSet.update(pCurrentDistance[arcID], arcID);
// 			}
// 			if (miniShortPathValue > pCurrentDistance[arcID] + pNotCurrentDistance[arcID])
// 			{
// 				miniShortPathValue = pCurrentDistance[arcID] + pNotCurrentDistance[arcID];
// 				middleNode = arcID;
// 			}
// 		}
		pArcNode = pArcNode->next;
	}
}

double CBiDijkstra::QueryShortestPath(const int start, const int destination)
{
	double currentShortestPathValue = MAXVALUE;
	int middleNode = -1;
	init(start, destination);
	while (1)
	{
		if (frontQueueSet.top().getWeight() > backQueueSet.top().getWeight())
		{
			//处理后向
			updateShortPath(backQueueSet,
				backNode, backDistance, frontDistance, 
				currentShortestPathValue, middleNode, backVisit);
		}
		else
		{
			updateShortPath(frontQueueSet,
				frontNode, frontDistance, backDistance,
				currentShortestPathValue, middleNode, frontVisit);
		}
		if (currentShortestPathValue <= frontQueueSet.top().getWeight() + backQueueSet.top().getWeight())  //中间节点先后被取出
		{
			break;
		}
	}
	generalPath(start, destination, middleNode);
	return currentShortestPathValue;
}

CString CBiDijkstra::frontPath(int start, int middleNodeID)
{
	int tempNodeID = middleNodeID;
	CString tempStr;
	CString strPath;
	tempStr.Empty();
	strPath.Empty();
	//front
	while (tempNodeID != start)
	{
		tempStr.Format("%d-", frontNode[tempNodeID]);
		strPath = tempStr + strPath;
		tempNodeID = frontNode[tempNodeID];
	}
	return strPath;
}

CString CBiDijkstra::backPath(int destination, int middleNodeID)
{
	int tempNodeID = middleNodeID;
	CString tempStr;
	CString strPath;
	tempStr.Empty();
	strPath.Empty();

	while (tempNodeID != destination)
	{
		tempStr.Format("%d-", backNode[tempNodeID]);
		strPath += tempStr;
		tempNodeID = backNode[tempNodeID];
	}
	strPath.Trim("-");
	return strPath;
}

void CBiDijkstra::generalPath(const int start, const int destination, const int middleNodeID)
{
	CString tempStr;
	tempStr.Format("%d-", middleNodeID);
	shortestPathNode = frontPath(start, middleNodeID) + tempStr + backPath(destination, middleNodeID); 
#ifdef OUTPUTPAT
	cout<<"双向迪杰斯特拉,结点"<<start<<"与"<<destination<<"最短路径："<<endl;
	cout<<shortestPathNode<<endl;
#endif
}

void CBiDijkstra::outPutAllShortestPath()
{
	DWORD start;
	int nodeNum = totalNumOfNode*(totalNumOfNode-1)/2;
	double *shortestPath = new double[nodeNum];
	double *queryTime = new double[nodeNum];
	int num = 0;
	//////////////////////////////////////////////////////////////////////////
	//check the all time
	for (int i=0; i<totalNumOfNode; ++i)
	{
		for (int j=i+1; j<totalNumOfNode; ++j)
		{

			start = GetTickCount();
			for (int k=0; k<ITTERM-1; ++k)
			{
				QueryShortestPath(i, j);
			}
			shortestPath[num] = QueryShortestPath(i, j);
			queryTime[num] = double(GetTickCount() - start);
			if (num%10000 == 0)
			{
				cout<<"Query"<<num<<endl;
			}
			num++;
		}
	}
	outPutShortestPathAndTime(shortestPath, queryTime);
	cout<<double(GetTickCount() - start)<<endl;
}

void CBiDijkstra::outPutShortestPathAndTime(const double *shortestPath, const double *queryTime)
{
	ofstream outFile(".\\Result\\bidijkstraShortestPath.txt");
	if (!outFile)
	{
		cout<<"can't open bidijkstraShortestPath.txt"<<endl;
		exit(0);
	}
	int num = 0;
	int nodeNum = totalNumOfNode*(totalNumOfNode-1)/2;
	for (int i=0; i<totalNumOfNode; ++i)
	{
		for (int j=i+1; j<totalNumOfNode; ++j)
		{
			outFile<<i<<" "<<j<<" "<<shortestPath[num]<<" "<<queryTime[num]<<endl;
			num++;
		}
	}
	outFile.close();
}

void CBiDijkstra::testGroupShortesPath()
{
	cout<<"双向迪杰斯特拉："<<endl;
#ifndef OUTTIME
	for (int i=0; i<QUERYNUM; ++i)
	{
		cout<<"query"<<i<<": "<<QueryShortestPath(QueryGroup[i][0], QueryGroup[i][1])<<endl;
	}
#else
	DWORD start;
	for (int i=0; i<QUERYNUM; ++i)
	{
		start = GetTickCount();
		for (int j=0; j<ITTERM; ++j)
		{
			QueryShortestPath(QueryGroup[i][0], QueryGroup[i][1]);
		}
		cout<<"Query"<<i<<": "<<(double)(GetTickCount()-start)<<"ms"<<endl;
	}
#endif


}

#endif

