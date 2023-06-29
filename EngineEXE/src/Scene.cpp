#include "pch.h"

#include "Scene.h"
#include "GameObject.h"
#include "Actor.h"
#include "Shader.h"
#include "OrthographicCamera.h"
#include "CollisionManager.h"
#include "EngineManager.h"
#include "JsonLoader.h"
#include "KeyboardClass.h"
#include "MouseClass.h"
#include "UILayer.h"
#include "Window.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "MiniPontiff.h"

Scene::Scene(const std::string& name, UILayer* uiLayer, Window* window, FrameBuffer* frameBuffer) :
	m_uiMain(uiLayer),
	m_window(window),
	m_name(name),
	m_frameBuffer(frameBuffer)
{
	// Setup camera
	m_activeCamera = new OrthographicCamera(-5.0f, 5.0f, -5.0f, 5.0f);
	m_activeCamera->SetPosition(glm::vec3(0.0f));

	// Setup collision Manager
	m_collisionManager = new CollisionManager();

	m_grid = Grid<PathNode>(160, 40, 0.5, { -15.5,-10, 0 });
	m_bossGrid = Grid<PathNode>(85, 40, 0.5, { 42, -60, 0 });

	// Load scene
	m_sceneInfo = QuackEngine::JsonLoader::LoadScene(name, m_gameObjects, m_grid, m_bossGrid);
	m_gameTimer.Start();

	// Update engine manager
	EngineManager::SetGameObjects(m_gameObjects);
}

void Scene::RenderScene()
{
	m_frameBuffer->Bind();
	/* Render here */
	glClearColor(12.0f / 255.0f, 19.0f / 255.0f, 25.0f / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Update();
	PhysicsUpdate();
	Render();

	/* Swap front and back buffers */
	glfwSwapBuffers(m_window->GetGLFWWindow());
	/* Poll for and process events */
	glfwPollEvents();

	m_frameBuffer->Unbind();
}

void Scene::SetGravity(const bool bactive)
{
	for (GameObject* gameObject : m_gameObjects)
	{
		if (Actor* actorObject = dynamic_cast<Actor*>(gameObject))
		{
			if (bactive)
			{
				actorObject->SetSimulateGravity(true);
			}
			else
			{
				actorObject->SetSimulateGravity(false);
			}
		}
	}
}

void Scene::SetInput(const bool bactive)
{
	m_StopInput = !bactive;
}

void Scene::Update()
{
	m_gameTimer.Tick();

	if (!m_gameObjectsToAdd.empty())
	{
		for (GameObject* gameObjectToAdd : m_gameObjectsToAdd)
		{
			m_gameObjects.push_back(gameObjectToAdd);
		}
		m_gameObjectsToAdd.clear();
	}

	if (!m_gameObjectsToRemove.empty())
	{
		for (GameObject* gameObjectToRemove : m_gameObjectsToRemove)
		{
			m_gameObjects.erase(m_gameObjects.begin() + EngineManager::GetGameObjectIndex(gameObjectToRemove));
		}
		m_gameObjectsToRemove.clear();
	}

	const float deltaTime = m_gameTimer.GetDeltaTime();


	// Make the camera and the background follow the player
	if (Quack::GetUILayer()->GetInPlay())
	{
		Vector3 playerPos = EngineManager::GetInputCharacter()->GetPosition();
		m_activeCamera->SetPosition(playerPos.GetglmVec3());
		playerPos.y = 0;
		m_gameObjects[1]->SetPosition(playerPos);
	}

	// Update game objects
	for (GameObject* gameObject : m_gameObjects)
	{
		if (gameObject) gameObject->Update(deltaTime);
	}

	if (bQueueReset)
	{
		bQueueReset = false;
		ResetScene();
	}

	// get mouse position
	double xpos, ypos;
	glfwGetCursorPos(Quack::GetWindow()->GetGLFWWindow(), &xpos, &ypos);

	HandleInput();
}

void Scene::Render()
{
	GameObject* inputCharacter = EngineManager::GetInputCharacter();

	if (!inputCharacter)
	{
		return;
	}

	int i = -1;
	// Draw game objects
	for (GameObject* gameObject : m_gameObjects)
	{
		i++;
		if (gameObject)
		{
			if (gameObject->GetName() != inputCharacter->GetName())
			{
				gameObject->Draw(m_activeCamera);
			}
		}
	}

	inputCharacter->Draw(m_activeCamera);

	if (m_StopInput)
	{
		Renderer::DrawDebugLines(m_activeCamera);
	}
}

void Scene::PhysicsUpdate()
{
	float deltaTime = m_gameTimer.GetDeltaTime();
	m_collisionManager->Update(deltaTime);
}

void Scene::HandleInput()
{
	const float deltaTime = m_gameTimer.GetDeltaTime();

	Character* inputCharacter = dynamic_cast<Character*>(EngineManager::GetInputCharacter());

	if (inputCharacter && !m_StopInput && inputCharacter->GetCurrentHealth() > 0.0f)
	{
		if (InputComponent* inputComponent = inputCharacter->GetComponent<InputComponent>())
		{
			// JUMP
			if (inputComponent->GetKeyPressed(' '))
			{
				inputCharacter->Jump();
			}

			// LIGHT ATTACK
			if (inputComponent->GetKeyPressed('j'))
			{
				inputCharacter->LightAttack();
			}

			// HEAVY ATTACK
			if (inputComponent->GetKeyPressed('l'))
			{
				inputCharacter->HeavyAttack();
			}

			// SPECIAL ATTACK
			if (inputComponent->GetKeyPressed('k'))
			{
				inputCharacter->SpecialAttack();
			}

			// MOVE RIGHT
			if (inputComponent->GetKeyDown('d'))
			{
				if (!inputCharacter->GetHittingWallLeft())
				{
					inputCharacter->AdjustPosition(Vector3((inputCharacter->GetMovementSpeed() * deltaTime), 0.0f, 0.0f));
				}
			}

			// MOVE LEFT
			if (inputComponent->GetKeyDown('a'))
			{
				if (!inputCharacter->GetHittingWallRight())
				{
					inputCharacter->AdjustPosition(Vector3((-inputCharacter->GetMovementSpeed() * deltaTime), 0.0f, 0.0f));
				}
			}

			// IDLE
			if (inputComponent->GetKeyUp('d') && !inputComponent->GetKeyDown('a') || inputComponent->GetKeyUp('a') && !inputComponent->GetKeyDown('d'))
			{
				inputCharacter->SetIdleAnimation();
			}

			if (inputComponent->GetKeyPressed('z'))
			{
				inputCharacter->AttemptToDash();
			}
		}
	}
}

void Scene::SaveScene()
{
	QuackEngine::JsonLoader::StoreScene(m_sceneInfo, m_gameObjects);
}

void Scene::LoadScene()
{
	if (!m_gameObjects.empty())
	{
		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			delete m_gameObjects[i];
		}
		m_gameObjects.clear();
		m_gameObjectsToAdd.clear();
		m_gameObjectsToRemove.clear();
	}
	QuackEngine::JsonLoader::LoadScene(m_sceneInfo.sceneName, m_gameObjects, m_grid, m_bossGrid);
}

void Scene::ResetScene()
{
	for (GameObject* gameObject : Quack::GetCurrentScene()->GetGameObjects())
	{
		Quack::GetCurrentScene()->RemoveGameObject(gameObject);
	}

	Quack::GetCurrentScene()->LoadScene();
	Quack::GetUILayer()->GetEditorUI()->SetDisplayedGameObject(nullptr);
	EngineManager::GetInputCharacter()->SetSimulateGravity(true);

	//Quack::GetOrthoCam()->ResetZoom();
	//Quack::GetOrthoCam()->SetPosition(Quack::GetOrthoCam()->GetDefaultPos());
}

void Scene::AddGameObject(GameObject* newGameObject)
{
	m_gameObjectsToAdd.push_back(newGameObject);
}

void Scene::RemoveGameObject(GameObject* gameObject)
{
	m_gameObjectsToRemove.push_back(gameObject);
}