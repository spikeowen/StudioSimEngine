#include "pch.h"

#include "PathNode.h"

PathNode::PathNode(Grid<PathNode>* grid, int x, int y, bool walkable) : 
	grid(grid),
	isWalkable(walkable)
{
	this->x = x;
	this->y = y;	
}

PathNode::~PathNode()
{
}

void PathNode::CalculateFCost()
{
	fCost = gCost + hCost;
}
