#include "pch.h"

#include "GridEditor.h"
#include "Quack.h"
#include "UILayer.h"
#include "PathNode.h"

GridEditor::GridEditor(std::string name) : UIWindow(name)
{
	
}

GridEditor::~GridEditor()
{
}

void GridEditor::Render()
{
	ImGui::Begin("Grid Editor");
	
	if (Quack::GetUILayer()->GetGridEditorActive())
	{
		if (m_Node)
		{
			Vector2 position = Vector2(m_Node->x, m_Node->y);
			string location = "Tile Location: (" + to_string(m_Node->x) + ", " + to_string(m_Node->y) + ")";
			ImGui::TextWrapped(location.c_str());

			ImGui::Checkbox("Is Walkable", &m_Node->isWalkable);
		}
		else
		{
			ImGui::TextWrapped("No Tile Selected");
		}
	}

	ImGui::End();
}

void GridEditor::HandleMouseInput(MouseEvent e)
{
}
