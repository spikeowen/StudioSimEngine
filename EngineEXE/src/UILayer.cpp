#include "pch.h"

#include "UILayer.h"
#include "Quack.h"
#include "Window.h"
#include "EngineManager.h"
#include "GameObject.h"
#include "Actor.h"
#include "Character.h"
#include "Enemy.h"
#include "EngineManager.h"

namespace fs = std::filesystem;

UILayer::UILayer() : Layer("UI Layer"), m_color(0.0f, 1.0f, 1.0f, 1.0f)
{
	newObjectInfo = CreatClassInfo();

	vertexData = QuackEngine::JsonLoader::LoadObjectData2D("Square");
}

UILayer::~UILayer()
{
}

void UILayer::OnAttach()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	ImGui::StyleColorsDark();

	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui_ImplGlfw_InitForOpenGL(Quack::GetWindow()->GetGLFWWindow(), true);
}

void UILayer::InitWindows()
{
	m_viewportUI = new ViewportUI("Viewport");
	m_editorUI = new EditorUI("Settings", EngineManager::GetGameObject(0));
	m_worldOutliner = new WorldOutlinerUI("World Outliner");
	m_worldOutliner->SetEditorUI(m_editorUI);
	m_contentBrowser = new ContentBrowserUI("Content Browser");
	m_gridEditor = new GridEditor("Grid Editor");

}

void UILayer::OnDetach()
{
}

void UILayer::OnUpdate()
{
	// Start a new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// start a new frame

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.65f, 0.0f, 1.0f));
	// Insert UI Code here
	EnableDocking();
	ImGui::End();
	ImGui::PopStyleColor(1);

	// render the data
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// render the data
}

void UILayer::OnEvent()
{
}

void UILayer::EnableDocking()
{
	static bool opt_fullscreen = true;
	static bool p_open = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	static glm::vec3 vector;

	m_viewportUI->Render();
	m_editorUI->Render();
	m_worldOutliner->Render();
	m_contentBrowser->Render();
	m_gridEditor->Render();

	ImGui::PopStyleVar();

	// Menu at the top of the window
	if (ImGui::BeginMenuBar())
	{
		//Option to create new objects or scenes
		if (ImGui::BeginMenu("New"))
		{
			SetUpObjectCreator();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::Separator();

			if (ImGui::Button("Save Current Scene"))
			{
				Quack::GetCurrentScene()->SaveScene();
				Quack::GetCurrentScene()->SetInput(false);
			}

			if (!inGridEditor)
			{
				if (ImGui::Button("Edit Grid"))
				{
					inGridEditor = true;
				}
			}
			else
			{
				if (ImGui::Button("Quit Grid Editor"))
				{
					inGridEditor = false;
				}
			}

			if (inEditor)
			{
				if (ImGui::Button("Play"))
				{
					Quack::GetCurrentScene()->SaveScene();
					Quack::GetCurrentScene()->SetInput(true);
					Quack::GetCurrentScene()->SetGravity(true);

					inEditor = false;
					inPlay = true;
					Quack::GetOrthoCam()->SetDefualtPosition(Quack::GetOrthoCam()->GetPosition());
					Quack::GetOrthoCam()->SetZoom(Quack::GetOrthoCam()->GetGameZoom(), Quack::GetUILayer()->GetViewport()->GetSize().x / Quack::GetUILayer()->GetViewport()->GetSize().y);
				}
			}

			if (inPlay)
			{
				if (ImGui::Button("Stop"))
				{
					for (GameObject* gameObject : Quack::GetCurrentScene()->GetGameObjects())
					{
						Quack::GetCurrentScene()->RemoveGameObject(gameObject);
					}
					Quack::GetCurrentScene()->LoadScene();
					Quack::GetCurrentScene()->SetInput(false);
					Quack::GetCurrentScene()->SetGravity(false);
					m_editorUI->SetDisplayedGameObject(nullptr);


					inEditor = true;
					inPlay = false;
					Quack::GetOrthoCam()->ResetZoom();
					Quack::GetOrthoCam()->SetPosition(Quack::GetOrthoCam()->GetDefaultPos());
				}
			}


			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false, &p_open != NULL))
				p_open = false;


			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void UILayer::SetUpObjectCreator()
{
	std::string objectName = "";
	std::string textureName = "";
	TransformData objectTransformData = TransformData();
	CollisionData objectCollisionData = CollisionData();

	if (ImGui::BeginMenu("Create New Object"))
	{

		if (ImGui::Button("Game Object"))
		{
			ImGui::OpenPopup("Create Game Object");
		}

		if (ImGui::BeginPopupModal("Create Game Object", NULL))
		{
			BasePopupContent();

			if (ImGui::Button("Create"))
			{
				GameObject* newGameObject = new GameObject(newObjectInfo.objectName, vertexData, newObjectInfo.transformData, newObjectInfo.collisionData, newObjectInfo.textureName);
				ImGui::CloseCurrentPopup();
				Quack::GetOrthoCam()->SetCanZoom(true);
				Quack::GetCurrentScene()->AddGameObject(newGameObject);
			}

			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
				Quack::GetOrthoCam()->SetCanZoom(true);
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button("Actor"))
		{
			ImGui::OpenPopup("Create Actor");
		}

		if (ImGui::BeginPopupModal("Create Actor"))
		{
			BasePopupContent();
			ActorContent();

			if (ImGui::Button("Create"))
			{
				ImGui::CloseCurrentPopup();
				Quack::GetOrthoCam()->SetCanZoom(true);
			}

			if (ImGui::Button("Close"))
			{
				Actor* newActor = new Actor(newObjectInfo.objectName, vertexData, newObjectInfo.transformData, newObjectInfo.collisionData, newObjectInfo.textureName, newObjectInfo.physicsData, newObjectInfo.animationData);
				ImGui::CloseCurrentPopup();
				Quack::GetOrthoCam()->SetCanZoom(true);
				Quack::GetCurrentScene()->AddGameObject(newActor);

			}
			ImGui::EndPopup();
		}

		if (ImGui::Button("Character"))
		{
			ImGui::OpenPopup("Create Character");
		}

		if (ImGui::BeginPopupModal("Create Character"))
		{
			BasePopupContent();
			ActorContent();
			CharacterContent();

			if (ImGui::Button("Create"))
			{
				Character* newCharacter = new Character(newObjectInfo.objectName, vertexData, newObjectInfo.transformData, newObjectInfo.collisionData, newObjectInfo.textureName, newObjectInfo.physicsData, newObjectInfo.movementData, newObjectInfo.entityData, newObjectInfo.animationData);
				ImGui::CloseCurrentPopup();
				Quack::GetOrthoCam()->SetCanZoom(true);
				Quack::GetCurrentScene()->AddGameObject(newCharacter);
			}

			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
				Quack::GetOrthoCam()->SetCanZoom(true);
			}

			ImGui::EndPopup();
		}

		if (ImGui::Button("Enemy"))
		{
			ImGui::OpenPopup("Create Enemy");
		}

		if (ImGui::BeginPopupModal("Create Enemy"))
		{
			BasePopupContent();
			ActorContent();
			CharacterContent();
			EnemyTypeContent();

			if (ImGui::Button("Create"))
			{
				Enemy* newEnemy = new Enemy(newObjectInfo.objectName, vertexData, newObjectInfo.transformData, newObjectInfo.collisionData, newObjectInfo.textureName, newObjectInfo.physicsData, newObjectInfo.movementData, newObjectInfo.entityData, newObjectInfo.animationData, newObjectInfo.enemyType);
				ImGui::CloseCurrentPopup();
				Quack::GetOrthoCam()->SetCanZoom(true);
				Quack::GetCurrentScene()->AddGameObject(newEnemy);
			}

			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
				Quack::GetOrthoCam()->SetCanZoom(true);
			}
			ImGui::EndPopup();
		}

		ImGui::EndMenu();
	}

}

void UILayer::BasePopupContent()
{
	Quack::GetOrthoCam()->SetCanZoom(false);
	ObjectName();
	ObjectTransformData();
	ObjectCollisionData(newObjectInfo.transformData);
	ObjectTextureName();
}

void UILayer::ActorContent()
{
	bool hasPhysicsComponent = newObjectInfo.addPhysicsComponent;

	ImGui::Checkbox("Add Physics Component", &hasPhysicsComponent);

	if (newObjectInfo.addPhysicsComponent != hasPhysicsComponent)
	{
		newObjectInfo.addPhysicsComponent = hasPhysicsComponent;
	}

	if (newObjectInfo.addPhysicsComponent)
	{
		float currentMass = newObjectInfo.physicsData.mass;

		ImGui::SliderFloat("Mass", &currentMass, 1.0f, 30.0f, NULL, ImGuiSliderFlags_AlwaysClamp);
		if (newObjectInfo.physicsData.mass != currentMass)
		{
			newObjectInfo.physicsData.mass = currentMass;
		}

		bool simulatingGravity = newObjectInfo.physicsData.bsimulateGravity;

		ImGui::Checkbox("Simulate Gravity", &simulatingGravity);

		if (newObjectInfo.physicsData.bsimulateGravity != simulatingGravity)
		{
			newObjectInfo.physicsData.bsimulateGravity = simulatingGravity;
		}

		float currentGravMultiplier = newObjectInfo.physicsData.gravityMultiplier;

		if (newObjectInfo.physicsData.bsimulateGravity)
		{
			ImGui::SliderFloat("Gravity Mulitplier", &currentGravMultiplier, 1.0f, 30.0f, NULL, ImGuiSliderFlags_AlwaysClamp);
			if (newObjectInfo.physicsData.gravityMultiplier != currentGravMultiplier)
			{
				newObjectInfo.physicsData.gravityMultiplier = currentGravMultiplier;
			}
		}
	}

	ImGui::Separator();

	bool hasAnimator = newObjectInfo.addAnimator;

	ImGui::Checkbox("Add Animator", &hasAnimator);

	if (newObjectInfo.addAnimator != hasAnimator)
	{
		newObjectInfo.addAnimator = hasAnimator;
	}

	if (newObjectInfo.addAnimator)
	{
		bool animated = newObjectInfo.animationData.banimated;

		ImGui::Checkbox("Animated", &animated);
		if (newObjectInfo.animationData.banimated != animated)
		{
			newObjectInfo.animationData.banimated = animated;
		}
	}

}

void UILayer::CharacterContent()
{
	float currentJumpHeight = newObjectInfo.movementData.jumpHeight;
	float currentMovementSpeed = newObjectInfo.movementData.movementSpeed;

	ImGui::SliderFloat("Jump Height", &currentJumpHeight, 0.0f, 1000.0f, NULL, ImGuiSliderFlags_AlwaysClamp);
	if (newObjectInfo.movementData.jumpHeight != currentJumpHeight)
	{
		newObjectInfo.movementData.jumpHeight = currentJumpHeight;
	}

	ImGui::SliderFloat("Movement Speed", &currentMovementSpeed, 1.0f, 25.0f, NULL, ImGuiSliderFlags_AlwaysClamp);
	if (newObjectInfo.movementData.movementSpeed != currentMovementSpeed)
	{
		newObjectInfo.movementData.movementSpeed = currentMovementSpeed;
	}

	ImGui::Separator();

	float currentMaxHealth = newObjectInfo.entityData.maxHealth;

	ImGui::SliderFloat("Max Health", &currentMaxHealth, 1.0f, 500.0f, NULL, ImGuiSliderFlags_AlwaysClamp);
	if (newObjectInfo.entityData.maxHealth != currentMaxHealth)
	{
		newObjectInfo.entityData.maxHealth = currentMaxHealth;
	}


}

std::string UILayer::CollisionName(CollisionType type)
{
	switch (type)
	{
	case CollisionType::BOX:
		return "BOX";
	case CollisionType::SPHERE:
		return "SPHERE";
	case CollisionType::NONE:
		return "NONE";
	default:
		return "";
	}
}

void UILayer::ObjectName()
{
	std::string name = newObjectInfo.objectName;

	if (ImGui::TreeNode("Object Name"))
	{
		static char charName[128] = "";
		strcpy_s(charName, name.c_str());
		ImGui::InputText("Object Name", charName, IM_ARRAYSIZE(charName));
		name = charName;

		newObjectInfo.objectName = name;

		ImGui::TreePop();
	}


	ImGui::Separator();

}

void UILayer::ObjectTransformData()
{
	TransformData transformData = newObjectInfo.transformData;

	if (ImGui::TreeNode("Transform"))
	{

		Vector3 pos = newObjectInfo.transformData.position;
		ImGui::DragFloat3("Position", &pos.x);

		if (newObjectInfo.transformData.position.x != pos.x || newObjectInfo.transformData.position.y != pos.y || newObjectInfo.transformData.position.z != pos.z)
		{
			newObjectInfo.transformData.position = pos;
		}


		Vector3 rot = newObjectInfo.transformData.rotation;
		ImGui::DragFloat("Rotation", &rot.z);

		if (newObjectInfo.transformData.rotation.x != rot.x || newObjectInfo.transformData.rotation.y != rot.y || newObjectInfo.transformData.rotation.z != rot.z)
		{
			newObjectInfo.transformData.rotation = rot;
		}


		Vector3 scale = newObjectInfo.transformData.scale;
		ImGui::DragFloat3("Scale", &scale.x);

		if (newObjectInfo.transformData.rotation.x != scale.x || newObjectInfo.transformData.rotation.y != scale.y || newObjectInfo.transformData.rotation.z != scale.z)
		{
			newObjectInfo.transformData.rotation = scale;
		}

		ImGui::TreePop();
	}

	ImGui::Separator();
}

void UILayer::ObjectCollisionData(TransformData data)
{
	CollisionData collisionData;

	collisionData.centerPosition = data.position;

	bool box = newObjectInfo.boxCollision;
	bool sphere = newObjectInfo.sphereCollision;
	bool noCollision = newObjectInfo.noCollision;

	if (ImGui::TreeNode("Collision"))
	{
		ImGui::Checkbox("Box Collision", &box);
		if (newObjectInfo.boxCollision != box)
		{
			newObjectInfo.boxCollision = box;

			if (newObjectInfo.boxCollision)
			{
				newObjectInfo.collisionData.collisionType = CollisionType::BOX;
				newObjectInfo.sphereCollision = false;
				newObjectInfo.noCollision = false;
			}
		}

		ImGui::Checkbox("Sphere Collision", &sphere);
		if (newObjectInfo.sphereCollision != sphere)
		{
			newObjectInfo.sphereCollision = sphere;

			if (newObjectInfo.sphereCollision)
			{
				newObjectInfo.collisionData.collisionType = CollisionType::SPHERE;
				newObjectInfo.boxCollision = false;
				newObjectInfo.noCollision = false;
			}
		}

		ImGui::Checkbox("No Collision", &noCollision);
		if (newObjectInfo.noCollision != noCollision)
		{
			newObjectInfo.noCollision = noCollision;

			if (newObjectInfo.noCollision)
			{
				newObjectInfo.collisionData.collisionType = CollisionType::NONE;
				newObjectInfo.boxCollision = false;
				newObjectInfo.sphereCollision = false;
			}
		}

		ImGui::TreePop();
	}

	ImGui::Separator();
}

void UILayer::ObjectTextureName()
{

	ImGui::Image((void*)Quack::GetTexture(newObjectInfo.textureName)->GetRendererID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));

	if (ImGui::TreeNode("Available Textures"))
	{
		for (fs::directory_entry texture : fs::directory_iterator("res/textures"))
		{
			if (ImGui::Button(texture.path().filename().string().c_str()))
			{
				newObjectInfo.textureName = texture.path().filename().string();
			}
		}

		ImGui::TreePop();
	}

	ImGui::Separator();
}

void UILayer::EnemyTypeContent()
{
	static bool melee = false;
	static bool ranged = false;

	if (ImGui::TreeNode("Enemy Type"))
	{
		ImGui::Checkbox("Melee", &melee);
		if (melee)
		{
			ranged = false;
			newObjectInfo.enemyType = EnemyType::MELEE;
		}

		ImGui::Checkbox("Ranged", &ranged);
		if (ranged)
		{
			melee = false;
			newObjectInfo.enemyType = EnemyType::RANGED;
		}

		ImGui::TreePop();
	}
	ImGui::Separator();
}
