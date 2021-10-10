#pragma once

#ifndef NODEMAP_H
#define NODEMAP_H

using namespace std;

#include <stdlib.h>

class NodeMap
{
public:
	NodeMap();
	NodeMap(int NODES_WIDTH, int NODES_HEIGHT);
	
	int GetSurroundingOnNodes(int i, int j);
	void SmoothMap();
	void GenerateNodeMap();

	void SetRegionNumbers();
	void Spread(int i, int j, int num);

	int NODES_WIDTH;
	int NODES_HEIGHT;

	int** nodes = NULL;
	int** oldNodes = NULL;

	int maxRegionNum = 0;
};

#endif