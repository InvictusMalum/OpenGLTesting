#include<iostream>

#include <stdlib.h>

#include"NodeMap.h"

NodeMap::NodeMap()
{

}

NodeMap::NodeMap(int NODES_WIDTH_, int NODES_HEIGHT_)
{
	NODES_WIDTH = NODES_WIDTH_;
	NODES_HEIGHT = NODES_HEIGHT_;
	
	nodes = new int* [NODES_HEIGHT];
	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		nodes[i] = new int[NODES_WIDTH];
	}

	oldNodes = new int* [NODES_HEIGHT];
	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		oldNodes[i] = new int[NODES_WIDTH];
	}
}

int NodeMap::GetSurroundingOnNodes(int i, int j)
{
	int count = 0;
	for (int neighborI = i - 1; neighborI <= i + 1; neighborI++)
	{
		for (int neighborJ = j - 1; neighborJ <= j + 1; neighborJ++)
		{
			if (neighborI != i || neighborJ != j)
			{
				if (neighborI >= 0 && neighborI < NODES_HEIGHT && neighborJ >= 0 && neighborJ < NODES_WIDTH)
				{
					if (oldNodes[neighborI][neighborJ] == 0)
					{
						count++;
					}
				} else
				{
					count++;
				}
			}
		}
	}
	return count;
}

void NodeMap::SmoothMap()
{
	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		for (int j = 0; j < NODES_WIDTH; j++)
		{
			oldNodes[i][j] = nodes[i][j];
		}
	}

	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		for (int j = 0; j < NODES_WIDTH; j++)
		{
			int count = GetSurroundingOnNodes(i, j);
			if (count > 4)
			{
				nodes[i][j] = 0;
			} else if (count < 4)
			{
				nodes[i][j] = 1;
			};
		};
	};
}

void NodeMap::GenerateNodeMap()
{
	srand(time(NULL));
	int percentOn = 50;
	int outLayer = 0;
	// Generating random nodes
	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		for (int j = 0; j < NODES_WIDTH; j++)
		{
			if (i == 0 || j == 0 || i == NODES_HEIGHT - 1 || j == NODES_WIDTH - 1 || rand() % 100 + 1 <= percentOn)
			{
				nodes[i][j] = 0;
			} else
			{
				nodes[i][j] = 1;
			}
		}
	}
	// Smoothing nodes
	for (int i = 0; i < 10; i++)
	{
		SmoothMap();
	}
}

void NodeMap::SetRegionNumbers()
{
	int nextSpreadNum = 2;
	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		for (int j = 0; j < NODES_WIDTH; j++)
		{
			if (nodes[i][j] == 1)
			{
				Spread(i, j, nextSpreadNum);
				nextSpreadNum++;
			}
		}
	}
	maxRegionNum = nextSpreadNum-1;
}

void NodeMap::Spread(int i, int j, int num)
{
	nodes[i][j] = num;
	if (nodes[i + 1][j] == 1)
	{
		Spread(i + 1, j, num);
	}
	if (nodes[i - 1][j] == 1)
	{
		Spread(i - 1, j, num);
	}
	if (nodes[i][j+1] == 1)
	{
		Spread(i, j+1, num);
	}
	if (nodes[i][j-1] == 1)
	{
		Spread(i, j-1, num);
	}
}