#pragma once
#include "BasicIncludes.h"
#include "Layer.h"

/// <summary>
/// Layer stack keeps track of layers and overlays. Overlays are always stored in the seconnd half of the vector
/// as they should always be rendered last. The way this is set up is the class keeps track of the index for the 
/// layer and it keeps inserting between last layer and first overlay, on the other hand overlays are always 
/// inserted at the end of the vector. When the layer or overlay are removed they are not deleted as all deleteing
/// is done once the layer stack is deleted which should be end of the application.
/// </summary>

class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);
	void PopLayer(Layer* layer);
	void PopOverlay(Layer* overlay);
	inline std::vector<Layer*> GetLayers() { return m_layers; }

	std::vector<Layer*>::iterator Begin() { return m_layers.begin(); }
	std::vector<Layer*>::iterator End() { return m_layers.end(); }
private:
	std::vector<Layer*> m_layers;
	std::vector<Layer*>::iterator m_layerInsert; // keep track of index of layers
};

