#include "pch.h"

#include "Component.h"

Component::Component(Actor* owningActor, const int updateOrder)
	: m_owningActor(owningActor), m_updateOrder(updateOrder)
{

}

Component::~Component()
{

}

void Component::Update(const float deltaTime)
{

}

