#include "Layer.h"

namespace sp { namespace graphics {

	Layer::Layer()
		: m_Window(Window::GetWindowClass(nullptr))
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::Init()
	{
	}

	bool Layer::OnEvent(const events::Event& event)
	{
		return false;
	}

	void Layer::OnTick()
	{
	}

	void Layer::OnUpdate()
	{
	}

	void Layer::OnRender()
	{
	}

} }