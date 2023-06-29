#pragma once
#include "UIWindow.h"

class PathNode;
class ViewportUI;

class GridEditor : public UIWindow
{
public:
	GridEditor(std::string name);
	~GridEditor();

	void Render();
	void HandleMouseInput(MouseEvent e) override;

	inline PathNode* GetNode() { return m_Node; }
	inline void SetNode(PathNode* node) { m_Node = node; }

private:
	PathNode* m_Node;
};

