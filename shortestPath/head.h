#ifndef _HEAD_FILE_
#define  _HEAD_FILE_

#include "afxtempl.h"
#define MAXVALUE 9999999999999
#define ITTERM 10000
#define OUTPUTPAT
//#define OUTTIME

#define FILECHOICE 1
CString strFileName[2][2] = {".\\data\\testNode.txt", ".\\data\\testEdge.txt",
	".\\data\\City of Oldenburg (OL) Road Network\\OLNodes.txt", ".\\data\\City of Oldenburg (OL) Road Network\\OLEdges.txt"};


#define QUERYNUM 9
int QueryGroup[QUERYNUM][2] = {
	5985, 60,
	828, 3969,
	4405, 4718,					//双向迪杰斯特拉比迪杰斯特拉慢
	6021, 4405,
	6021, 4160,
	4160, 4868,					//双向迪杰斯特拉比迪杰斯特拉慢
	473, 4138,
	4868, 473,
	5335, 3969
};

CString strFileNameReachValue[2]={".\\Result\\testGraphReach.txt",
									".\\Result\\dijkstraReach.txt"};

#endif