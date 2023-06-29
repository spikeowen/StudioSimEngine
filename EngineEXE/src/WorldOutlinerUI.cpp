#include "pch.h"

#include "WorldOutlinerUI.h"
#include "ObjectIncludes.h"


WorldOutlinerUI::WorldOutlinerUI(std::string name) : UIWindow(name)
{
	m_CurrentScene = nullptr;
	m_EditorUI = nullptr;

	m_isHovered = false;
}

WorldOutlinerUI::~WorldOutlinerUI()
{
	delete m_CurrentScene;
	m_CurrentScene = nullptr;

	delete m_EditorUI;
	m_EditorUI = nullptr;
}

void WorldOutlinerUI::Render()
{

	ImGui::Begin("World Outliner");

	m_isHovered = ImGui::IsWindowHovered();

	//Creates initial node
	//Opening this dropdown will show the game objects within the current scene
	//This dropdown is set to be open by default as it is the root node
	bool dropdown = ImGui::TreeNodeEx("Current Scene Game Objects", ImGuiTreeNodeFlags_DefaultOpen);

	if (dropdown)
	{
		//Loops through game objects in the current scene
		//and creates a tree node for each game object that in the current scene
		for (GameObject* object : m_CurrentScene->GetGameObjects())
		{
			bool dropdownOpen = ImGui::TreeNodeEx(object->GetName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow);

			//Changes the game object in the settings window
			//based off the object the user clicked on in the list of objects
			if (ImGui::IsItemClicked())
			{
				m_EditorUI->SetDisplayedGameObject(object);
			}


			if (dropdownOpen)
			{
				//Opening this drop down will show all the components attached to the game object
				//if it is an actor or a child of actor
				if (Actor* actorObject = dynamic_cast<Actor*>(object))
				{
					for (Component* components : actorObject->GetComoponents())
					{
						if (actorObject->GetComoponents().size() > 0)
						{
							//ImGui::Text(typeid(*components).name());

							if (ImGui::Selectable(typeid(*components).name()))
							{
								m_EditorUI->SetDisplayedGameObject(object);
							}

						}
						else
						{
							ImGui::Text("No Components Attached To Object");
						}
					}

					ImGui::TreePop();
				}
				else
				{
					ImGui::TreePop();
				}
			}

		}

		ImGui::TreePop();
	}

	ImGui::End();
}

void WorldOutlinerUI::HandleKeyboardInput(KeyEvent key)
{

}

void WorldOutlinerUI::HandleMouseInput(MouseEvent e)
{

}
