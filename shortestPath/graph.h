#pragma once
#include "stdafx.h"
#include "afxtempl.h"
#include "Graph.h"
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

//结点文件：
//		首行为结点总个数，以下为结点信息
//		每行为一个结点，包括三列：结点编号（从0开始）、结点经度、纬度
//
//边文件：
//		每行为一条边，边序号（从0开始）、边开始结点、结束结点、权重



enum GraphKind
{
	unDirectGraph, directGraph
};

struct position
{
	double x;
	double y;
};

struct arcNode
{
	int id;
	double weight;
	struct arcNode * next;
};

struct vertexNode
{
	int id;
	struct position message;
	struct arcNode *firstArcNode;
};


class CGraph
{
	GraphKind kind;

private:
	void InitNode(const CString strFileNameNode);
	void InitEdgeOfUndirect(const CString strFileNameEdge);
	void InitEdgeOfDirect(const CString strFileNameEdge);
	void outGraph();

protected:
	struct vertexNode *graphData;		//the vertex
	int totalNumOfNode;					//the number of nodes
	inline BOOL FindEdge(const int node1, const int node, double &weight);

public:
	inline struct arcNode *FirstEdge(const int nodeID);
	CGraph(CString strFileNameNode, CString strFileNameEdge, GraphKind graphKind = unDirectGraph);
	~CGraph(void);

};


CGraph::CGraph(CString strFileNameNode, CString strFileNameEdge, GraphKind graphKind)
{
	kind = graphKind;
	totalNumOfNode = -1;
	InitNode(strFileNameNode);
	if (unDirectGraph == graphKind)
	{
		InitEdgeOfUndirect(strFileNameEdge);
	}
	else
	{
		InitEdgeOfDirect(strFileNameEdge);
	}
	//	outGraph();
}

CGraph::~CGraph(void)
{
	struct arcNode *pArcNode;
	struct arcNode *pArcNode1;
	for (int i=0; i<totalNumOfNode; ++i)
	{
		pArcNode = graphData[i].firstArcNode;
		while (pArcNode != NULL)
		{
			pArcNode1 = pArcNode;
			pArcNode = pArcNode1->next;
			delete pArcNode1;
		}
	}
}


void CGraph::InitNode(CString strFileNameNode)
{
	ifstream fin(strFileNameNode);
	if (!fin)
	{
		return;
	}
	string strTemp;
	int numOfNode;
	int i = 0;
	fin>>strTemp;
	numOfNode = stoi(strTemp);
	totalNumOfNode = numOfNode;
	graphData = new struct vertexNode[numOfNode];
	while (fin>>strTemp)
	{
		graphData[i].id = stoi(strTemp);
		fin>>strTemp;
		graphData[i].message.x = stof(strTemp);
		fin>>strTemp;
		graphData[i].firstArcNode = NULL;
		graphData[i++].message.y = stof(strTemp);
	}
	fin.close();
}


void CGraph::InitEdgeOfUndirect(const CString strFileNameEdge)
{
	ifstream fin(strFileNameEdge);
	string strTemp;
	int nodeId1 = -1;
	int nodeId2 = -1;
	double weight = -0.0;
	while (fin>>strTemp)
	{
		fin>>strTemp;
		nodeId1 = stoi(strTemp);
		fin>>strTemp;
		nodeId2 = stoi(strTemp);
		fin>>strTemp;
		weight = stof(strTemp);

		struct arcNode *newArc1 = new struct arcNode;
		newArc1->id = nodeId2;
		newArc1->weight = weight;
		newArc1->next = graphData[nodeId1].firstArcNode;
		graphData[nodeId1].firstArcNode = newArc1;

		struct arcNode *newArc2 = new struct arcNode;
		newArc2->id = nodeId1;
		newArc2->weight = weight;
		newArc2->next = graphData[nodeId2].firstArcNode;
		graphData[nodeId2].firstArcNode = newArc2;
	}
	fin.close();
}


void CGraph::InitEdgeOfDirect(const CString strFileNameEdge)
{
	ifstream fin(strFileNameEdge);
	string strTemp;
	int nodeId1 = -1;
	int nodeId2 = -1;
	double weight = -0.0;
	while (fin>>strTemp)
	{
		fin>>strTemp;
		nodeId1 = stoi(strTemp);
		fin>>strTemp;
		nodeId2 = stoi(strTemp);
		fin>>strTemp;
		weight = stof(strTemp);

		struct arcNode *newArc1 = new struct arcNode;
		newArc1->id = nodeId2;
		newArc1->weight = weight;
		newArc1->next = graphData[nodeId1].firstArcNode;
		graphData[nodeId1].firstArcNode = newArc1;
	}
	fin.close();
}

//
//output all the edges
//

void CGraph::outGraph()
{
	int num = totalNumOfNode;
	struct arcNode *pArcNode;
	for (int i=0; i<num; ++i)
	{
		pArcNode = graphData[i].firstArcNode;
		cout<<"node "<<i<<": ";
		while (pArcNode != NULL)
		{
			cout<<pArcNode->id<<"  ";
			pArcNode = pArcNode->next;
		}
		cout<<endl;
	}
}


BOOL CGraph::FindEdge(const int node1, const int node2, double &weight)
{
	struct arcNode *pArcNode;
	pArcNode = graphData[node1].firstArcNode;
	while (pArcNode != NULL)
	{
		if (pArcNode->id == node2)
		{
			weight = pArcNode->weight;
			return TRUE;
		}
		pArcNode = pArcNode->next;
	}
	return FALSE;
}


struct arcNode *CGraph::FirstEdge(const int nodeID)
{
	struct arcNode *temp;
	temp = graphData[nodeID].firstArcNode;
	return temp;
}

