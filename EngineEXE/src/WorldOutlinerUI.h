#pragma once
#include "UIWindow.h"
#include "GameObject.h"
#include "Scene.h"
#include "EditorUI.h"


class WorldOutlinerUI : public UIWindow
{
public:
	WorldOutlinerUI(std::string name);
	~WorldOutlinerUI();


	void Render() override;
	void HandleKeyboardInput(KeyEvent key) override;
	void HandleMouseInput(MouseEvent e) override;

	inline void SetCurrentScene(Scene* scene) { m_CurrentScene = scene; }
	inline void SetEditorUI(EditorUI* editor) { m_EditorUI = editor; }

private:
	Scene* m_CurrentScene;
	EditorUI* m_EditorUI;


};
