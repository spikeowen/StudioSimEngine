#pragma once
#include "BasicIncludes.h"
#include "Grid.h"

struct PathNode
{
public:
	PathNode(Grid<PathNode>* grid, int x, int y, bool walkable = true);
	~PathNode();

	inline std::string ToString() { return std::to_string(x) + "," + std::to_string(y); }
	void CalculateFCost();
public:
	int x, y;
	Grid<PathNode>* grid;
	int gCost, hCost, fCost;
	bool isWalkable;
	PathNode* parent;	
};

