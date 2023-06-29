#pragma once
#include "component.h"
#include "Pathfinding.h"
#include "Character.h"

enum class MeleeState
{
	IDLE,
	CHASING,
	ATTACKING
};

class AIComponent : public Component
{
public:
	AIComponent(Actor* owningActor, const int updateOrder);

	virtual void Update(const float deltaTime) override;
	virtual void SetGrid(Grid<PathNode> grid) { m_pathFinder->SetGrid(grid); }
protected:
	Pathfinding* m_pathFinder;
	std::vector<Vector3> m_path;
	MeleeState m_state;

	float m_speed;
	float m_timer; 
	float m_pathUpdateTime;
	float m_attackRange;
	float m_targetRange;

	// reference to player
	Character* m_player;
};

