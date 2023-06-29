#pragma once
#include "UIWindow.h"
#include "Texture.h"


class ContentBrowserUI : public UIWindow
{
public:

	ContentBrowserUI(std::string name);
	~ContentBrowserUI();

	void Render() override;
	void HandleKeyboardInput(KeyEvent key) override;
	void HandleMouseInput(MouseEvent e) override;

private:
	std::string m_ContentRoot = "res";
	std::string m_CurrentDirectory;

	ImGuiIO& m_IO = ImGui::GetIO();

	int m_ThumbnailPadding;
	int m_ThumbnailSize;

	Texture* m_FolderThumbnail;
	Texture* m_FileThumbnail;

	std::map<std::string, Texture*> m_PreviewThumbnails;

	bool m_ButtonHighlighted;
};