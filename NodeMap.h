#pragma once

#ifndef NODEMAP
#define NODEMAP

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

	int NODES_WIDTH;
	int NODES_HEIGHT;

	int** nodes = NULL;
	int** oldNodes = NULL;
};

#endif