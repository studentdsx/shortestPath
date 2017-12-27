#ifndef _BIHEAP_FILE_
#define _BIHEAP_FILE_

#include <iostream>
#include "head.h"
#define INITHEAP 100000
#define COMPARE <

class CBiHeapPoint
{
private:
	int weight;
	int nodeID;

public:
	CBiHeapPoint();	
	CBiHeapPoint(double _weight, int _nodeID);
	~CBiHeapPoint();

public:
	inline double getWeight();
	inline int getNodeID();
	bool operator<(CBiHeapPoint y);
	bool operator>(CBiHeapPoint y);
	bool operator==(CBiHeapPoint y);
};

CBiHeapPoint::CBiHeapPoint()
{
	weight = 0.0;
	nodeID = -1;
}

CBiHeapPoint::~CBiHeapPoint()
{

}

CBiHeapPoint::CBiHeapPoint(double _weight, int _nodeID)
{
	weight = _weight;
	nodeID = _nodeID;
}

int CBiHeapPoint::getWeight()
{
	return weight;
}

int CBiHeapPoint::getNodeID()
{
	return nodeID;
}

bool CBiHeapPoint::operator<(CBiHeapPoint y)
{
	return weight < y.weight;
}

bool CBiHeapPoint::operator>(CBiHeapPoint y)
{
	return weight > y.weight;
}

bool CBiHeapPoint::operator==(CBiHeapPoint y)
{
	return weight == y.weight;
}

class CBiHeap
{
private:
	CBiHeapPoint *arry;
	int *index;
	int size;
	int len;

public:
	CBiHeap();
	~CBiHeap();
private:
	void updateDown(int downPosition);
	void updateUp(int upPosition);
	void expandHeap();

public:
	void push(double weight, int nodeID);
	void push(CBiHeapPoint heapPoint);

	void update(double weight, int nodeID);
	void update(CBiHeapPoint updateHeapPoint);

	CBiHeapPoint top();
	void pop();

	void clear();
	bool empty();

};

CBiHeap::CBiHeap()
{
	len = 0;
	size = INITHEAP;
	arry = new CBiHeapPoint[INITHEAP];
	index = new int[INITHEAP];
	memset(index, -1, sizeof(int)*INITHEAP);
}

CBiHeap::~CBiHeap()
{
	delete []arry;
	delete []index;
}

void CBiHeap::expandHeap()
{
	CBiHeapPoint *tp = new CBiHeapPoint[2*size];
	memcpy(tp, arry, size*sizeof(CBiHeapPoint));
	delete []arry;
	arry = tp;
	int *newIndex = new int[2*size];
	memcpy(newIndex, index, size*sizeof(int));
	delete index;
	index = newIndex;
	size *= 2;
}

void CBiHeap::push(double weight, int nodeID)
{
	push(CBiHeapPoint(weight, nodeID));
}

void CBiHeap::push(CBiHeapPoint heapPoint)
{
	if (len == size - 1)
	{
		expandHeap();
	}
	if (index[heapPoint.getNodeID()] != -1)
	{
		update(heapPoint);
	}
	else
	{
		arry[++len] = heapPoint;
		index[heapPoint.getNodeID()] = len;
		updateUp(len);
		updateDown(len);
	}
}

void CBiHeap::updateUp(int upPosition)
{
	CBiHeapPoint heapPoint = arry[upPosition];
	int nullPosition = upPosition;
	int fatherPosition = upPosition/2;
	for (; fatherPosition>=1; fatherPosition/=2)
	{
		if (arry[fatherPosition] COMPARE heapPoint)
		{
			break;
		}
		arry[nullPosition] = arry[fatherPosition];
		index[arry[fatherPosition].getNodeID()] = nullPosition;
		nullPosition = fatherPosition;
	}
	arry[nullPosition] = heapPoint;
	index[heapPoint.getNodeID()] = nullPosition;
}

void CBiHeap::updateDown(int downPosition)
{
	CBiHeapPoint heapPoint = arry[downPosition];
	int nullPosition = downPosition;
	int minChildPosition = nullPosition * 2;
	while (minChildPosition <= len)
	{
		if (minChildPosition < len && arry[minChildPosition+1] COMPARE arry[minChildPosition])
		{
			minChildPosition ++;
		}
		if (heapPoint COMPARE arry[minChildPosition])
		{
			break;
		}
		arry[nullPosition] = arry[minChildPosition];
		index[arry[nullPosition].getNodeID()] = nullPosition;
		nullPosition = minChildPosition;
		minChildPosition *= 2;
	}
	arry[nullPosition] = heapPoint;
	index[heapPoint.getNodeID()] = nullPosition;
}

void CBiHeap::update(double weight, int nodeID)
{
	update(CBiHeapPoint(weight, nodeID));
}

void CBiHeap::update(CBiHeapPoint updateHeapPoint)
{
	int updatePointNodeID = updateHeapPoint.getNodeID();
	if (index[updatePointNodeID] == -1)
	{
		push(updateHeapPoint);
		return;
	}
	if (updateHeapPoint COMPARE arry[index[updatePointNodeID]])
	{
		arry[index[updatePointNodeID]] = updateHeapPoint;
		updateUp(index[updatePointNodeID]);
	}
	else
	{
		arry[index[updatePointNodeID]] = updateHeapPoint;
		updateDown(index[updatePointNodeID]);
	}
}

CBiHeapPoint CBiHeap::top()
{
	if (len <= 0)
	{
		return CBiHeapPoint(INITHEAP, -1);
	}
	return arry[1];
}

void CBiHeap::pop()
{
	if (0 == len)
	{
		return;
	}
	index[arry[1].getNodeID()] = -1;
	len --;
	if (0 == len)
	{
		return;
	}
	arry[1] = arry[len+1];
	index[arry[1].getNodeID()] = 1;
	updateDown(1);
}

bool CBiHeap::empty()
{
	return 0 == len;
}

void CBiHeap::clear()
{
	len = 0;
	memset(index, -1, sizeof(int)*size);
}

#endif