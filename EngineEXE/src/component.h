#pragma once
#include "QuackDataTypes.h"
#include "Actor.h"
#include "MouseEvent.h"
#include "KeyboardClass.h"
#include "MouseClass.h"



class Component
{
public:
	Component(Actor* owningActor, const int updateOrder);
	~Component();

	virtual void Update(const float deltaTime);

	inline Actor* GetOwningActor() { return m_owningActor; }
	inline const int GetUpdateOrder() const { return m_updateOrder; }

protected:
	Actor* m_owningActor = nullptr;
	int m_updateOrder = -1;
};

