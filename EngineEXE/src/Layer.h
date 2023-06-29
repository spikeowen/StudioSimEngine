#pragma once
#include "BasicIncludes.h"

class Layer
{
public:
	Layer(const std::string& name = "Layer") { m_debugName = name; }
	virtual ~Layer() {}

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnEvent() {}

	inline const std::string& GetName() const { return m_debugName; }
protected:
	std::string m_debugName;
};

