#ifndef _DIJKSTRA_FILE_
#define _DIJKSTRA_FILE_

#include "Graph.h"
#include "biHeap.h"
#include "head.h"
#include <assert.h>

class CDijkstra
	:public CGraph
{
public:
	CDijkstra(CString strFileNameNode, CString strFileNameEdge, GraphKind graphKind = unDirectGraph);
	~CDijkstra();

private:
	CBiHeap DijkstraQueueSet;					//当前label的节点
	int *shortestPath;							//节点前驱
	double *shortestPathValue;					//最短最路径的值
	BOOL *isVisited;							//节点是否被访问
	CString shortestPathNode;

	inline void init(const int start, const int destination);
	inline void generalPath(const int start, const int destination);

public:
	double QueryShortestPath(const int start, const int destination);
	void testGroupShortestPath();
	void outPutAllShortestPath();

};

CDijkstra::CDijkstra(CString strFileNameNode, CString strFileNameEdge, GraphKind graphKind /* = unDirectGraph */)
	:CGraph(strFileNameNode, strFileNameEdge, graphKind)
{
	isVisited = new BOOL[totalNumOfNode];
	shortestPath = new int[totalNumOfNode];
	shortestPathValue = new double[totalNumOfNode];
}

CDijkstra::~CDijkstra()
{
	delete []isVisited;
	delete []shortestPathValue;
	delete []shortestPath;
}

void CDijkstra::init(const int start, const int destination)
{
	assert(0<=start && destination<totalNumOfNode);
	for (int i=0; i<totalNumOfNode; ++i)
	{
		isVisited[i] = FALSE;
		shortestPathValue[i] = MAXVALUE;
		shortestPath[i] = -1;
	}
	DijkstraQueueSet.clear();
	DijkstraQueueSet.push(0.0, start);
	shortestPath[start] = start;
}

double CDijkstra::QueryShortestPath(const int start, const int destination)
{
	assert(0<=start && destination<totalNumOfNode);
	init(start, destination);

	int currentNodeID = -1;
	double currentNodeWeight = 0.0;
	CBiHeapPoint currentNode;
	arcNode *pCurrentArcNode = NULL;
	int tempID;

	while (!DijkstraQueueSet.empty())
	{
		currentNode = DijkstraQueueSet.top();
		currentNodeID = currentNode.getNodeID();
		currentNodeWeight = currentNode.getWeight();
		DijkstraQueueSet.pop();

		isVisited[currentNodeID] = TRUE;
		shortestPathValue[currentNodeID] = currentNodeWeight;
		if (currentNodeID == destination)
		{
			break;
		}
		//松弛
		pCurrentArcNode = graphData[currentNodeID].firstArcNode;
		while (pCurrentArcNode)
		{
			tempID = pCurrentArcNode->id;
			if (!isVisited[tempID] && pCurrentArcNode->weight + currentNodeWeight < shortestPathValue[tempID])
			{
				shortestPathValue[tempID] = pCurrentArcNode->weight + currentNodeWeight;
				shortestPath[tempID] = currentNodeID;
				DijkstraQueueSet.push(shortestPathValue[tempID], tempID);
			}
			pCurrentArcNode = pCurrentArcNode->next;
		}
	}
	generalPath(start, destination);
	return currentNodeWeight;
}

void CDijkstra::generalPath(const int start, const int destination)
{
	assert (0<=start && destination<totalNumOfNode);
	int middleNodeID = destination;
	CString str;
	shortestPathNode.Empty();
	while (middleNodeID != start)
	{
		str.Format("%d-", middleNodeID);
		shortestPathNode = str + shortestPathNode;
		middleNodeID = shortestPath[middleNodeID];
	}
	str.Format("%d-", start);
	shortestPathNode = str + shortestPathNode;
	shortestPathNode.Trim("-");
#ifdef OUTPUTPAT
	cout<<"迪杰斯特拉,结点"<<start<<"与"<<destination<<"最短路径："<<endl;
	cout<<shortestPathNode<<endl;
#endif
}

void CDijkstra::testGroupShortestPath()
{
	cout<<"迪杰斯特拉:"<<endl;
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

void CDijkstra::outPutAllShortestPath()
{
	DWORD start;
	int nodeNum = totalNumOfNode*(totalNumOfNode-1)/2;
	double *shortest = new double[nodeNum];
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
				shortestPathNode.Empty();
				QueryShortestPath(i, j);
			}
			shortest[num] = QueryShortestPath(i, j);
			queryTime[num] = double(GetTickCount() - start);
			if (num%10000 == 0)
			{
				cout<<"Query"<<num<<endl;
			}
			num++;
		}
	}
	cout<<double(GetTickCount() - start)<<endl;
}

#endif