#pragma once
#include "BasicIncludes.h"
#include "GameObject.h"
#include "Actor.h"
#include "Scene.h"
#include "CollisionManager.h"
#include "AIComponent.h"
#include "Enemy.h"
#include "MiniPontiff.h"

using json = nlohmann::json;

namespace nlohmann
{
	template<>
	struct adl_serializer<Vector3>
	{
		static Vector3 from_json(const json& j)
		{
			Vector3 temp = Vector3(j["x"].get<float>(), j["y"].get<float>(), j["z"].get<float>());
			return temp;
		}

		static void to_json(json& j, Vector3 vec)
		{
			j["x"] = vec.x;
			j["y"] = vec.y;
			j["z"] = vec.z;
		}
	};

	template<>
	struct adl_serializer<AnimationRowData>
	{
		static AnimationRowData from_json(const json& j)
		{
			AnimationRowData tempAnim = AnimationRowData(j["rowName"].get<std::string>(), j["rowNumber"].get<int>(), j["amountOfColumns"].get<int>(), j["playRate"].get<float>(), j["blooping"].get<bool>());
			return tempAnim;
		}

		static void to_json(json& j, AnimationRowData anim)
		{
			j["rowName"] = anim.name;
			j["rowNumber"] = anim.rowNumber;
			j["amountOfColumns"] = anim.amountOfColumns;
			j["playRate"] = anim.playRate;
			j["blooping"] = anim.blooping;
		}
	};

	template<>
	struct adl_serializer<std::vector<AnimationRowData>>
	{
		static std::vector<AnimationRowData> from_json(const json& j)
		{
			std::vector<AnimationRowData> temp;
			int size = j["numOfAnim"].get<int>();

			for (int i = 0; i < size; i++)
			{
				AnimationRowData tempAnim = j["animRowData" + std::to_string(i)].get<AnimationRowData>();
				temp.push_back(tempAnim);
			}

			return temp;
		}

		static void to_json(json& j, std::vector<AnimationRowData> animArray)
		{
			for (int i = 0; i < animArray.size(); i++)
			{
				j["animRowData" + std::to_string(i)] = animArray[i];
			}
			j["numOfAnim"] = animArray.size();
		}
	};

	template<>
	struct adl_serializer<GameObject*>
	{
		static GameObject* from_json(const json& j)
		{
			VertexData* data = new VertexData();
			TransformData transformData;
			CollisionData collisionData;
			PhysicsData physicsData;
			AnimationData animationData;
			MovementData movementData;
			EntityData entityData;
			bool bconsumeInput = false;

			// Load name
			std::string name = j["name"].get<std::string>();

			// Load Game Object Data
			data->vertices = j["vertices"].get<std::vector<float>>();
			data->colors = j["colors"].get<std::vector<float>>();
			data->texCoords = j["texCoords"].get<std::vector<float>>();

			// Load Transform Data
			transformData.position = j["position"].get<Vector3>();
			transformData.rotation = j["rotation"].get<Vector3>();
			transformData.scale = j["scale"].get<Vector3>();

			// Load Collision Data
			collisionData.centerPosition = j["centerPosition"].get<Vector3>();
			collisionData.centerPositionOffset = j["centerPositionOffset"].get<Vector3>();
			collisionData.collisionType = (CollisionType)j["collisionType"].get<int>();
			collisionData.radius = j["radius"].get<float>();
			collisionData.size = j["size"].get<Vector3>();

			
			std::string textureName = j["textureName"].get<std::string>();
			GameObjectType type = (GameObjectType)j["objectType"];

			switch (type)
			{
			case GameObjectType::OBJECT:
				return new GameObject(name, data, transformData, collisionData, textureName);

			case GameObjectType::ACTOR:

				// Load PhysicsData
				physicsData.bsimulateGravity = j["bsimulateGravity"].get<bool>();
				physicsData.mass = j["mass"].get<float>();
				physicsData.gravityMultiplier = j["gravityMultiplier"].get<float>();

				// Load AnimationData
				animationData.banimated = j["banimated"].get<bool>();

				return new Actor(name, data, transformData, collisionData, textureName, physicsData, animationData);
			case GameObjectType::CHARACTER:

				// Load PhysicsData
				physicsData.bsimulateGravity = j["bsimulateGravity"].get<bool>();
				physicsData.mass = j["mass"].get<float>();
				physicsData.gravityMultiplier = j["gravityMultiplier"].get<float>();

				// Load AnimationData
				animationData.banimated = j["banimated"].get<bool>();
				if (animationData.banimated)
				{
					animationData.totalRows = j["totalRows"].get<int>();
					animationData.totalColumns = j["totalColumns"].get<int>();
					animationData.animationRowData = j["animationRowData"].get<std::vector<AnimationRowData>>();
				}
				
				// Load movement data
				movementData.jumpHeight = j["jumpHeight"].get<float>();
				movementData.movementSpeed = j["movementSpeed"].get<float>();

				// load entity data
				entityData.maxHealth = j["maxHealth"].get<float>();

				// load input data
				bconsumeInput = j["bconsumeInput"].get<bool>();

				return new Character(name, data, transformData, collisionData, textureName, physicsData, movementData, entityData, animationData, bconsumeInput);
			case GameObjectType::ENEMY:

				EnemyType enemyType = (EnemyType)j["EnemyType"].get<int>();

				// Load PhysicsData
				physicsData.bsimulateGravity = j["bsimulateGravity"].get<bool>();
				physicsData.mass = j["mass"].get<float>();
				physicsData.gravityMultiplier = j["gravityMultiplier"].get<float>();

				// Load AnimationData
				animationData.banimated = j["banimated"].get<bool>();
				if (animationData.banimated)
				{
					animationData.totalRows = j["totalRows"].get<int>();
					animationData.totalColumns = j["totalColumns"].get<int>();
					animationData.animationRowData = j["animationRowData"].get<std::vector<AnimationRowData>>();
				}

				// Load movement data
				movementData.jumpHeight = j["jumpHeight"].get<float>();
				movementData.movementSpeed = j["movementSpeed"].get<float>();

				// load entity data
				entityData.maxHealth = j["maxHealth"].get<float>();

				if (enemyType == EnemyType::MELEE || enemyType == EnemyType::RANGED)
					return new Enemy(name, data, transformData, collisionData, textureName, physicsData, movementData, entityData, animationData, enemyType);
				else if (enemyType == EnemyType::MINI_PONTIFF)
					return new MiniPontiff(name, data, transformData, collisionData, textureName, physicsData, movementData, entityData, animationData, enemyType);
			}
		}

		static void to_json(json& j, GameObject* gameObject)
		{
			// Store Name
			j["name"] = gameObject->GetName();
			j["objectType"] = (int)gameObject->GetType();

			// Store Object Data
			j["vertices"] = gameObject->GetGameObjectData()->vertices;
			j["colors"] = gameObject->GetGameObjectData()->colors;
			j["texCoords"] = gameObject->GetGameObjectData()->texCoords;

			// Store Transform Data
			Transform* transformData = gameObject->GetTransform();
			j["position"] = transformData->GetPosition();
			j["rotation"] = transformData->GetRotation();
			j["scale"] = transformData->GetScale();

			// Store Collision Data
			CollisionData collisionData = gameObject->GetCollisionData();
			j["collisionType"] = (int)collisionData.collisionType;
			j["centerPosition"] = collisionData.centerPosition;
			j["centerPositionOffset"] = collisionData.centerPositionOffset;
			j["size"] = collisionData.size;
			j["radius"] = collisionData.radius;

			// Store Texture Data
			std::string textureName = gameObject->GetTextureName();
			j["textureName"] = textureName;

			if (gameObject->GetType() == GameObjectType::ACTOR || gameObject->GetType() == GameObjectType::CHARACTER || gameObject->GetType() == GameObjectType::ENEMY)
			{
				Actor* actor = dynamic_cast<Actor*>(gameObject);

				// Store Physics Data
				PhysicsData physicsData = actor->GetPhysicsData();
				j["mass"] = physicsData.mass;
				j["bsimulateGravity"] = physicsData.bsimulateGravity;
				j["gravityMultiplier"] = physicsData.gravityMultiplier;

				// Store Animation Data
				AnimationData animationData = actor->GetAnimationData();
				j["banimated"] = animationData.banimated;
				if (animationData.banimated)
				{
					j["totalRows"] = animationData.totalRows;
					j["totalColumns"] = animationData.totalColumns;
					j["animationRowData"] = animationData.animationRowData;
				}

				if (gameObject->GetType() == GameObjectType::CHARACTER || gameObject->GetType() == GameObjectType::ENEMY)
				{
					Character* character = dynamic_cast<Character*>(actor);
					MovementData movementData = character->GetMovementData();
					j["jumpHeight"] = movementData.jumpHeight;
					j["movementSpeed"] = movementData.movementSpeed;
					
					EntityData entityData = character->GetEntityData();
					j["maxHealth"] = entityData.maxHealth;

					const bool bconsumingInput = character->GetConsumingInput();
					j["bconsumeInput"] = bconsumingInput;

					if (gameObject->GetType() == GameObjectType::ENEMY)
					{
						Enemy* enemy = dynamic_cast<Enemy*>(gameObject);
						j["EnemyType"] = (int)enemy->GetEnemyType();
					}
				}
			}
		}
	};
}

namespace QuackEngine {
	namespace JsonLoader {

		static VertexData* LoadObjectData2D(const std::string& name)
		{
			std::ifstream file("res/ObjectData/" + name + ".json");

			// check if the file was found
			if (!file)
			{
				QE_LOG(name + " does not exist!");
				return new VertexData();
			}

			json j = json::parse(file);

			// load the json values	
			VertexData* data = new VertexData();
			data->vertices = j["vertices"].get<std::vector<float>>();
			data->colors = j["colors"].get<std::vector<float>>();
			data->texCoords = j["texCoords"].get<std::vector<float>>();
			// return the sphere
			return data;
		}

		static bool StoreGameObjectData2D(const std::string& fileName, const VertexData& data)
		{
			json j;
			j["vertices"] = data.vertices;
			j["colors"] = data.colors;
			j["texCoords"] = data.texCoords;

			std::ofstream o("res/ObjectData/" + fileName + ".json");
			o << std::setw(4) << j << std::endl;
			return true;
		}

		static bool StoreGameObject2D(GameObject* gameObject)
		{
			json j;

			j[gameObject->GetName()] = gameObject;

			std::ofstream o("res/Objects/" + gameObject->GetName() + ".json");
			o << std::setw(4) << j << std::endl;
			return true;
		}

		static GameObject* LoadGameObject2D(std::string name)
		{
			std::ifstream file("res/Objects/" + name + ".json");

			// Check if the file was found
			if (!file)
			{
				QE_LOG(name + " does not exist!");
				return nullptr;
			}

			json j = json::parse(file);
			return j[name].get<GameObject*>();
		}

		static SceneInfo LoadScene(std::string sceneName, std::vector<GameObject*>& gameObjects, Grid<PathNode> grid, Grid<PathNode> bossGrid)
		{
			std::string path = "res/scenes/" + sceneName + ".json";

			std::ifstream file(path);
				
			// check if the file was found
			if (!file)
			{
				QE_LOG(path + " does not exist!");
				return SceneInfo({0});
			}

			json j = json::parse(file);

			SceneInfo sceneInfo;
			sceneInfo.objectCount = j["ObjectCount"].get<int>();
			sceneInfo.sceneName = sceneName;

			for (int i = 0; i < sceneInfo.objectCount; i++)
			{
				GameObject* obj = j["GameObject" + std::to_string(i)].get<GameObject*>();
				gameObjects.push_back(obj);

				// Set Grid for enemies
				if (obj->GetType() == GameObjectType::ENEMY)
				{
					Enemy* enemy = static_cast<Enemy*>(obj);

					if (enemy->GetEnemyType() == EnemyType::MINI_PONTIFF)
						enemy->GetAIComponent()->SetGrid(bossGrid);
					else
						enemy->GetAIComponent()->SetGrid(grid);
				}
			}
			
			return sceneInfo;
		}

		static void StoreScene(SceneInfo sceneInfo, std::vector<GameObject*>& gameObjects)
		{
			json j;

			j["ObjectCount"] = gameObjects.size();

			for (uint32_t i = 0; i < gameObjects.size(); i++)
				j["GameObject" + std::to_string(i)] = gameObjects[i];

			std::string path = "res/scenes/" + sceneInfo.sceneName + ".json";
			std::ofstream o(path);
			o << std::setw(4) << j << std::endl;
		}
	}
}


