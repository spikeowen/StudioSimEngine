#pragma once
#include "BasicIncludes.h"
#include "Renderer.h"

enum class GridSide
{
    TopRight,
    TopLeft,
    BottomRight,
    BottomLeft
};

template<class TGridObject>
class Grid
{
private:
    int m_width;
    int m_height;
    float m_tileSize;
    Vector3 m_originPos;
    std::vector<TGridObject*> m_gridArray;
public:
    Grid() {}

    Grid(int width, int height, float tileSize, Vector3 originPosition)
    {
        m_width = width;
        m_height = height;
        m_tileSize = tileSize;
        m_originPos = originPosition;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                m_gridArray.push_back(new TGridObject(this, x, y));
            }
        }   

        // set debug to true to draw the grid 
        bool showDebug = true;
        Vector3 white = { 0.0f, 0.0f, 0.0f};
        if (showDebug)
        {
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    Renderer::DrawLine(GetWorldPosition(x, y), GetWorldPosition(x + 1, y), white);
                    Renderer::DrawLine(GetWorldPosition(x, y), GetWorldPosition(x, y + 1), white);
                }
            }
            
            Renderer::DrawLine(GetWorldPosition(0, height), GetWorldPosition(width, height), white);
            Renderer::DrawLine(GetWorldPosition(width, 0), GetWorldPosition(width, height), white);
        }
    }

    Vector3 GetOriginPos()
    {
        return m_originPos;
    }

    void DrawLine()
    {

    }

    // Calculates which area of the grid the position is in and returns a corresponding enum
    void GetGridSide(glm::vec3 pos)
    {
        int x, y;
        GetXY(pos, x, y);

        if (y > m_height / 2)
        {
            if (x > m_width / 2)
                return GridSide::TopRight;
            else
                return GridSide::TopLeft;
        }
        else
        {
            if (x > m_width / 2)
                return GridSide::BottomRight;
            else
                return GridSide::BottomLeft;
        }
    }

    /// <summary>
    /// returns the width of the grid
    /// </summary>
    int GetWidth() { return m_width; }

    /// <summary>
    /// returns the height of the grid
    /// </summary>
    inline int GetHeight() { return m_height; }

    /// <summary>
    /// returns the size of the grid
    /// </summary>
    inline float GetCellSize() { return m_tileSize; }

    /// <summary>
    /// Converts x and y location of the grid to a world position
    /// </summary>
    Vector3 GetWorldPosition(int x, int y)
    {
        Vector3 temp = Vector3(x, y, 0) * m_tileSize + m_originPos;
        std::string output = std::to_string(temp.x) + "," + std::to_string(temp.y) + "," + std::to_string(temp.z);
        return temp;
    }

    /// <summary>
    /// <para> Returns the x and y of the world position on the grid </para>
    /// <para> The function outputs the result to the passed in x and y</para>
    /// </summary>
    void GetXY(Vector3 worldPos, int& x, int& y)
    {
        x = std::floor((worldPos.x - m_originPos.x) / m_tileSize);
        y = std::floor((worldPos.y - m_originPos.y) / m_tileSize);
    }

    void SetGridObject(Vector3 worldPosition, TGridObject value)
    {
        int x, y;
        GetXY(worldPosition, x, y);
        SetGridObject(x, y, value);
    }

    TGridObject* GetGridObject(int x, int y)
    {
        if (x >= 0 && y >= 0 && x < m_width && y < m_height)
        {
            return m_gridArray[y * m_height + x];
        }
        else
        {
            QE_ERROR("x or y out of bounds");   
            return nullptr;
        }
    }

    TGridObject* GetGridObject(Vector3 worldPosition)
    {
        int x, y;
        GetXY(worldPosition, x, y);
        return GetGridObject(x, y);
    }

};

