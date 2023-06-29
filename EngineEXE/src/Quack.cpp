#include "pch.h"

#include "Quack.h"
#include "Shader.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "KeyboardClass.h"
#include "MouseClass.h"
#include "Animate.h"
#include "QuackCallbacks.h"
#include "EngineManager.h"
#include "Renderer.h"
#include "Animate.h"
#include "JsonLoader.h"
#include "LayerStack.h"
#include "UIRenderer.h"
#include "UILayer.h"
#include "Scene.h"
#include "InputComponent.h"
#include "WorldOutlinerUI.h"

namespace fs = std::filesystem;

#pragma region DeclareMembers
bool Quack::s_glfwInitialised = false;
bool Quack::s_running = false;
Window* Quack::m_window;

GameTimer Quack::m_gameTimer;

LayerStack* Quack::m_layerStack;

Shader* Quack::m_primitiveShader;
Shader* Quack::m_textureShader;

int Quack::m_frameCounter;
int Quack::m_currentFrameRate;

UILayer* Quack::m_uiMain;

Scene Quack::m_mainScene;

OrthographicCamera* Quack::m_mainCamera;

QuackAudio* Quack::m_AudioEngine;

std::map<std::string, Texture*> Quack::m_textures;

#pragma endregion DeclareMembers

int Quack::InitEngine()
{
	s_running = true;

	m_window = new Window("Quack", 1920, 1080, FullScreenMode::WINDOWED);
	m_layerStack = new LayerStack();
	m_uiMain = new UILayer();

	m_layerStack->PushOverlay(m_uiMain);

	// Initilaize window
	m_window->UseWindow();

	m_gameTimer.Start();

	/* Initialize the Glew Library*/
	glewExperimental = GL_TRUE;
	glewInit();

	/* Initialize the input classes */
	KeyboardClass::Init();
	MouseClass::Init();

	/* Setup glfw callbacks */
	glfwSetKeyCallback(m_window->GetGLFWWindow(), QuackEngine::key_callback);
	glfwSetWindowCloseCallback(m_window->GetGLFWWindow(), QuackEngine::window_close_callback);
	glfwSetWindowSizeCallback(m_window->GetGLFWWindow(), QuackEngine::window_size_callback);
	glfwSetMouseButtonCallback(m_window->GetGLFWWindow(), QuackEngine::mouse_button_callback);
	glfwSetCursorPosCallback(m_window->GetGLFWWindow(), QuackEngine::cursor_position_callback);
	glfwSetScrollCallback(m_window->GetGLFWWindow(), scroll_callback);


	// According to laws beyond my comprehensions frame buffers must be made exactly here
	FrameBufferSpecificiation fbs;
	fbs.width = m_window->GetWidth();
	fbs.height = m_window->GetHeight();
	FrameBuffer* frameBuffer = new FrameBuffer(fbs);

	///
	///	Initialize IMGUI (Must be after keyboard and mouse callbacks)
	///
	m_uiMain->OnAttach();

	m_primitiveShader = new Shader("res/shaders/Primitive.shader");
	m_textureShader = new Shader("res/shaders/basic.shader");
	m_textureShader->Bind();
	m_textureShader->SetUniform4f("u_lightColor", 1.0f, 1.0f, 1.0f, 1.0f);

	//Ambient light
	m_textureShader->SetUniform4f("u_light.position", 0.0f, 0.0f, 0.0f, -2.0f);
	m_textureShader->SetUniform4f("u_light.ambient", 1.0f, 1.0f, 1.0f, 1.0f);
	m_textureShader->Unbind();

	GenerateTextureList();

	// Must be after shaders
	Renderer::Init();
	m_mainScene = Scene("MainScene", m_uiMain, m_window, frameBuffer);

	m_uiMain->InitWindows(); // should always be after init objects

	m_AudioEngine->Init();

	return 0;
}

void Quack::GenerateTextureList()
{
	for (fs::directory_entry file : fs::directory_iterator("res/textures"))
	{
		std::string imagePath = file.path().string();

		std::string imageName = file.path().filename().string();

		Texture* texture = new Texture(TextureData(imagePath));

		m_textures[imageName] = texture;
	}
}

/// <summary>
/// Takes in name of the texure and if the specified texture exists returns a pointer to it.
/// If the texture does not exist it returns a nullptr and prints out an error.
/// </summary>
/// <param name="textureName"> The name of the texture </param>
/// <returns></returns>
Texture* Quack::GetTexture(std::string textureName)
{
	Texture* objectTexture = m_textures[textureName];

	if (!objectTexture)
	{
		QE_LOG(textureName + " Not found");
		return nullptr;
	}

	return objectTexture;
}

void Quack::HandleInput()
{
	if (!m_uiMain->GetInPlay())
	{
		if (!KeyboardClass::KeyBufferIsEmpty())
		{
			KeyEvent key = KeyboardClass::ReadKey();

			if (key.GetKeyCode() != 0)
			{
				m_uiMain->GetViewport()->HandleKeyboardInput(key);
			}
		}


		//Mouse Input
		if (!MouseClass::IsEventBufferEmpty())
		{
			MouseEvent e = MouseClass::ReadEvent();

			m_uiMain->GetEditorUI()->HandleMouseInput(e);
			m_uiMain->GetViewport()->HandleMouseInput(e);
			m_uiMain->GetWorldOutliner()->HandleMouseInput(e);
			m_uiMain->GetGridEditor()->HandleMouseInput(e);
		}
	}

}

void Quack::Update()
{
	m_gameTimer.Tick();

	m_uiMain->GetWorldOutliner()->SetCurrentScene(&m_mainScene);

	// get mouse position
	double xpos, ypos;
	glfwGetCursorPos(m_window->GetGLFWWindow(), &xpos, &ypos);

	HandleInput();
}

void Quack::RenderUpdate()
{
	glClearColor(m_uiMain->GetColor().x, m_uiMain->GetColor().y, m_uiMain->GetColor().z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// Draw layers
	for (Layer* layer : m_layerStack->GetLayers())
	{
		if (layer) layer->OnUpdate();
	}

	m_mainScene.RenderScene();
}

void Quack::ShutDown()
{
	//m_mainScene.SaveScene();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(m_window->GetGLFWWindow());
	glfwTerminate();
};
