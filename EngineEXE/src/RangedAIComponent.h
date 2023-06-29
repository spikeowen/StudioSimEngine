#pragma once
#include "AIComponent.h"

enum class RangedState
{
	CHASING,
	ATTACKING,
	RETREATING,
};

class RangedAIComponent : public AIComponent
{
public:
	RangedAIComponent(Actor* owningActor, const int updateOrder);

	virtual void Update(const float deltaTime) override;
	virtual void SetGrid(Grid<PathNode> grid) { m_pathFinder->SetGrid(grid); }
protected:

	float m_attackTimer;
	bool m_animStarted;
	float m_attackSpeed;


};

