#include "sp/sp.h"
#include "Panel.h"

#include "Widget.h"

#include "sp/app/Application.h"
#include "sp/maths/maths.h"
#include "sp/graphics/shaders/ShaderFactory.h"

namespace sp { namespace graphics { namespace ui {

	using namespace events;
	using namespace maths;

	Panel::Panel()
		: Layer2D(ShaderFactory::DefaultShader(), maths::mat4::Orthographic(0.0f, 32.0f, 0.0f, 18.0f, -1.0f, 1.0f))
	{
		Application::GetApplication().PushOverlay(this);
	}

	Panel::~Panel()
	{
		for (uint i = 0; i < m_Widgets.size(); i++)
			delete m_Widgets[i];

		Application::GetApplication().PopOverlay(this);
	}

	Widget* Panel::Add(Widget* widget)
	{
		m_Widgets.push_back(widget);
		return widget;
	}

	void Panel::Remove(Widget* widget)
	{
		int index = 0;
		for (uint i = 0; i < m_Widgets.size(); i++)
		{
			if (m_Widgets[i] == widget)
			{
				m_Widgets.erase(m_Widgets.begin() + i);
				delete m_Widgets[i];
				break;
			}
		}
	}

	void Panel::Clear()
	{
		for (uint i = 0; i < m_Widgets.size(); i++)
			delete m_Widgets[i];

		m_Widgets.clear();
	}

	void Panel::OnEvent(events::Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MousePressedEvent>(METHOD(&Panel::OnMousePressedEvent));
		dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&Panel::OnMouseReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(METHOD(&Panel::OnMouseMovedEvent));
	}

	bool Panel::OnMousePressedEvent(events::MousePressedEvent& e)
	{
		vec2 mouse(e.GetX() * (32.0f / Window::GetWindowClass(nullptr)->GetWidth()), 18.0f - e.GetY() * (18.0f / Window::GetWindowClass(nullptr)->GetHeight()));
		for (uint i = 0; i < m_Widgets.size(); i++)
		{
			Widget* widget = m_Widgets[i];
			if (widget->GetBounds().Contains(mouse))
			{
				if (widget->OnMousePressed(e))
					return true;
			}
		}
		return false;
	}

	bool Panel::OnMouseReleasedEvent(events::MouseReleasedEvent& e)
	{
		vec2 mouse(e.GetX() * (32.0f / Window::GetWindowClass(nullptr)->GetWidth()), 18.0f - e.GetY() * (18.0f / Window::GetWindowClass(nullptr)->GetHeight()));
		for (uint i = 0; i < m_Widgets.size(); i++)
		{
			Widget* widget = m_Widgets[i];
			if (widget->GetBounds().Contains(mouse))
			{
				if (widget->OnMouseReleased(e))
					return true;
			}
		}
		return false;
	}

	bool Panel::OnMouseMovedEvent(events::MouseMovedEvent& e)
	{
		for (uint i = 0; i < m_Widgets.size(); i++)
		{
			Widget* widget = m_Widgets[i];
			if (widget->OnMouseMoved(e))
				return true;
		}
		return false;
	}

	void Panel::OnUpdate()
	{
		for (Widget* widget : m_Widgets)
		{
			if (widget->IsActive())
				widget->OnUpdate();
		}
	}

	void Panel::OnRender(Renderer2D& renderer)
	{
		for (Widget* widget : m_Widgets)
		{
			if (widget->IsActive())
				widget->OnRender(renderer);
		}

	}

} } }