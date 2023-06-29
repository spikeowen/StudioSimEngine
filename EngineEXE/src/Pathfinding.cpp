#include "pch.h"

#include "Pathfinding.h"

#define MOVE_STRAIGHT_COST 10
#define MOVE_DIAGONAL_COST 14

std::vector<Vector3> Pathfinding::FindPath(Vector3 startWorldPosition, Vector3 endWorldPosition)
{
    int startX, startY;
    int endX, endY;
    // convert the start and end world position to X and Y format
    m_grid.GetXY(startWorldPosition,  startX, startY);
    m_grid.GetXY(endWorldPosition, endX, endY);

    std::vector<PathNode*> path = FindPathNode(startX, startY, endX, endY);
    if (path.empty())
    {
        QE_ERROR("returned path was null in Vector3 FindPath - ");
        return std::vector<Vector3>();
    }
    else
    {
        std::vector<Vector3> vectorPath = std::vector<Vector3>();

        // convert the path nodes to vectors in the world
        for (PathNode* pathNode : path)
        {
            Vector3 result = Vector3(pathNode->x, pathNode->y, 0.0f) * m_grid.GetCellSize() + m_grid.GetOriginPos() + Vector3(1.0f, 1.0f, 1.0f) * m_grid.GetCellSize() * 0.5f;
            vectorPath.push_back(result);
        }

        // Reverse the order of the vector as index 0 will be final node
        std::reverse(vectorPath.begin(), vectorPath.end());
        return vectorPath;
    }
}

std::vector<PathNode*> Pathfinding::FindPathNode(int startX, int startY, int endX, int endY)
{
    PathNode* startNode = m_grid.GetGridObject(startX, startY);
    PathNode* endNode = m_grid.GetGridObject(endX, endY);

    if (startNode == nullptr || endNode == nullptr)
    {
        QE_ERROR("Error: start or end node is null");
        return std::vector<PathNode*>();
    }

    m_openList = std::vector<PathNode*>{ startNode };
    m_closedList = std::vector<PathNode*>();

    for (int x = 0; x < m_grid.GetWidth(); x++) {
        for (int y = 0; y < m_grid.GetHeight(); y++) {

            PathNode* pathNode = m_grid.GetGridObject(x, y);
            pathNode->gCost = 9999.0f;
            pathNode->CalculateFCost();
            pathNode->parent = nullptr;
        }
    }

    startNode->gCost = 0;
    startNode->hCost = CalculateDistanceCost(startNode, endNode);
    startNode->CalculateFCost();

    // Keep looping while there are still nodes in the open vector
    while (m_openList.size() > 0)
    {
        PathNode* currentNode = GetLowestFcost(m_openList);
        if (currentNode == endNode)
        {
            return CalculatePath(endNode);
        }

        m_openList.erase(std::find(m_openList.begin(), m_openList.end(), currentNode));
        m_closedList.push_back(currentNode);

        for (PathNode* neighbourNode : GetNeighbourList(currentNode))
        {
            // add a way to check if vector containts pointer
            if (std::find(m_closedList.begin(), m_closedList.end(), neighbourNode) != m_closedList.end())
                continue;

            if (!neighbourNode->isWalkable)
            {
                m_closedList.push_back(neighbourNode);
                continue;
            }

            int tentativeGCost = currentNode->gCost + CalculateDistanceCost(currentNode, neighbourNode);

            if (tentativeGCost < neighbourNode->gCost)
            {
                neighbourNode->parent= currentNode;
                neighbourNode->gCost = tentativeGCost;
                neighbourNode->hCost = CalculateDistanceCost(neighbourNode, endNode);
                neighbourNode->CalculateFCost();
                  
                // Check if the closed list contains the neighbour
                if (std::find(m_closedList.begin(), m_closedList.end(), neighbourNode) == m_closedList.end())
                    m_openList.push_back(neighbourNode);
            }
        }
    }

    // Out of nodes on the openList
    return std::vector<PathNode*>();
}

std::vector<PathNode*> Pathfinding::GetNeighbourList(PathNode* currentNode)
{
    std::vector<PathNode*> neighbourList = std::vector<PathNode*>();

    if (currentNode->x - 1 >= 0) {
        // Add Left Node
        neighbourList.push_back(GetNode(currentNode->x - 1, currentNode->y));

        // Add left up node
        if (currentNode->y + 1 < m_grid.GetHeight())
            neighbourList.push_back(GetNode(currentNode->x - 1, currentNode->y + 1));

        // Add left down node
        if (currentNode->y - 1 >= 0)
            neighbourList.push_back(GetNode(currentNode->x - 1, currentNode->y - 1));

    }
    if (currentNode->x + 1 < m_grid.GetWidth()) {
        // Add Right Node
        neighbourList.push_back(GetNode(currentNode->x + 1, currentNode->y));

        // Add Right Down
        if (currentNode->y - 1 >= 0)
            neighbourList.push_back(GetNode(currentNode->x + 1, currentNode->y - 1));
        // Add Right Up
        if (currentNode->y + 1 < m_grid.GetHeight())
            neighbourList.push_back(GetNode(currentNode->x + 1, currentNode->y + 1));
    }
    // Add Down
    if (currentNode->y - 1 >= 0)
        neighbourList.push_back(GetNode(currentNode->x, currentNode->y - 1));
    // Add Up
    if (currentNode->y + 1 < m_grid.GetHeight())
        neighbourList.push_back(GetNode(currentNode->x, currentNode->y + 1));

    return neighbourList;
}

std::vector<PathNode*> Pathfinding::CalculatePath(PathNode* endNode)
{
    std::vector<PathNode*> path = std::vector<PathNode*>();
    path.push_back(endNode);
    PathNode* currentNode = endNode;

    while (currentNode->parent != nullptr)
    {
        path.push_back(currentNode->parent);
        currentNode = currentNode->parent;
    }

    //path.Reverse();
    return path;
}

int Pathfinding::CalculateDistanceCost(PathNode* a, PathNode* b)
{
    int xDistance = std::abs(a->x - b->x);
    int yDistance = std::abs(a->y - b->y);
    int remaining = std::abs(xDistance - yDistance);
    int min = std::min(xDistance, yDistance);

    int result = 0;
    result = MOVE_DIAGONAL_COST * min;
    result += (MOVE_STRAIGHT_COST * remaining);
    return result;
}

PathNode* Pathfinding::GetLowestFcost(std::vector<PathNode*> pathList)
{
    PathNode* lowestFCostNode = pathList[0];

    for (int i = 1; i < pathList.size(); i++)
    {
        if (pathList[i]->fCost < lowestFCostNode->fCost)
        {
            lowestFCostNode = pathList[i];
        }
    }

    return lowestFCostNode;
}
