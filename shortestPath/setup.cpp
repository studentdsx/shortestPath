// shortesPath.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "biHeap.h"
#include "biDijkstra.h"
#include "Dijkstra.h"
#include "reach_biDijkstra.h"
#include "head.h"
#include <istream>
#include <time.h>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	CBiDijkstra biDijkstra(strFileName[FILECHOICE][0], strFileName[FILECHOICE][1]);
	biDijkstra.testGroupShortesPath();
	
 	CDijkstra dijkstra(strFileName[FILECHOICE][0], strFileName[FILECHOICE][1]);
 	dijkstra.testGroupShortestPath();

	CReachBiDijkstra reachBiDijkstra(strFileName[FILECHOICE][0], strFileName[FILECHOICE][1]);
	reachBiDijkstra.testGroupShortesPath();

	system("pause");
	return 0;
}

