#pragma once
#include "BasicIncludes.h"
#include "Quack.h"
#include "Character.h"

// Helper class to get reference to objects
class EngineManager
{
public:
	static GameTimer* GetGameTimer() { return m_gameTimer; }
	static std::vector<GameObject*> GetGameObjects() { return m_gameObjects; }
	static void SetGameObjects(std::vector<GameObject*> gameObjects) { m_gameObjects = gameObjects; }
	static GameObject* GetGameObject(std::string name);
	static GameObject* GetGameObject(const int index);
	static int GetGameObjectIndex(std::string name);
	/// <summary>
	/// Gets object index from scene object vector
	/// </summary>
	/// <param name="gameObject"></param>
	/// <param name="gameObjectArray"></param>
	/// <returns>if object not found returns -1</returns>
	static int GetGameObjectIndex(GameObject* gameObject);
	static int GetGameObjectIndex(GameObject* gameObject, std::vector<GameObject*> gameObjectArray);
	static void SetInputCharacter(Character* newInputCharacter);
	static inline Character* GetInputCharacter() { return m_inputCharacter; }
	static const bool GetCharacterConsumingInput(Character* inputCharacter);

private:
	static GameTimer* m_gameTimer;
	static std::vector<GameObject*> m_gameObjects;
	static Character* m_inputCharacter;

private:
	EngineManager();
	~EngineManager();
};

