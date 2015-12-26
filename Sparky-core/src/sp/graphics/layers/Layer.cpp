#include "sp/sp.h"
#include "Layer.h"

namespace sp { namespace graphics {

	Layer::Layer()
		: m_Window(Window::GetWindowClass(nullptr)), m_Visible(true)
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::Init()
	{
	}

	void Layer::OnEvent(events::Event& event)
	{
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