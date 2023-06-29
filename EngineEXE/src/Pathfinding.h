#pragma once
#include "Grid.h"
#include "PathNode.h"
#include <vector>

class Pathfinding
{
public:
    Pathfinding()
    {
        m_grid = Grid<PathNode>(0, 0, 0.0f, Vector3::Zero);
    }

    Pathfinding(int width, int height)
    {
        m_grid = Grid<PathNode>(width, height, 1.0f, Vector3::Zero);
    }

    Pathfinding(Grid<PathNode> grid)
    {
        m_grid = grid;
    }

    /// <summary>
    /// Returns the grid for pathfinding
    /// </summary>
    inline Grid<PathNode> GetGrid() { return m_grid; }

    inline void SetGrid(Grid<PathNode> grid) { m_grid = grid; }

    /// <summary>
    /// Get a node from the grid using x and y
    /// </summary>
    inline PathNode* GetNode(int x, int y) { return m_grid.GetGridObject(x, y); }

    /// <summary>
    /// <para>Finds the path from one vector to another </para>
    /// <para>This overload returns a vector of vector3's which can make movement easier for the character </para>
    /// <para>Returns null and prints an error if the path was not found </para>
    /// </summary>
    std::vector<Vector3> FindPath(Vector3 startWorldPosition, Vector3 endWorldPosition);
private:
    /// <summary>
    /// <para>Finds the path from one point to another </para>
    /// <para>Returns a vector of pathnodes, the vector does not need to be reversed as the function does it for you </para>
    /// <para>If the function fails it prints out an error message and returns null </para>
    /// </summary>
    std::vector<PathNode*> FindPathNode(int startX, int startY, int endX, int endY);
    /// <summary>
    /// Returns a vector of 8 neighbours around the node
    /// </summary>
    std::vector<PathNode*> GetNeighbourList(PathNode* currentNode);

    /// <summary>
    /// Once the path has been found loops through all the parent nodes of the endNode and returns a vector of the path
    /// </summary>
    std::vector<PathNode*> CalculatePath(PathNode* endNode);

    /// <summary>
    /// Calculates the cost of going from node a to node b
    /// </summary>
    int CalculateDistanceCost(PathNode* a, PathNode* b);

    /// <summary>
    /// Goes through all the nodes and finds the current lowest F cost node
    /// </summary>
    PathNode* GetLowestFcost(std::vector<PathNode*> pathvector);

private:
    Grid<PathNode> m_grid;
    std::vector<PathNode*> m_openList;
    std::vector<PathNode*> m_closedList;
};
