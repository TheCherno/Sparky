#include "sp/sp.h"
#include "DebugMenu.h"

#include "sp/app/Window.h"
#include "sp/graphics/ui/Button.h"

namespace sp { namespace debug {

	using namespace maths;
	using namespace graphics::ui;

	DebugMenu* DebugMenu::s_Instance = nullptr;

	DebugMenu::DebugMenu()
		: m_Visible(false), m_Slider(nullptr)
	{
		s_Instance = this;

		m_Settings.padding = 0.75f;
		m_Settings.fontSize = 24.0f;

		Add("Padding", &m_Settings.padding, 0.0f, 2.0f);
		Add("Font Size", &m_Settings.fontSize, 8.0f, 48.0f);

		m_Slider = nullptr;
		m_Panel = new Panel();
	}

	DebugMenu* DebugMenu::Get()
	{
		return s_Instance;
	}

	void DebugMenu::Init()
	{
		s_Instance = new DebugMenu();
	}

	void DebugMenu::Add(const String& name)
	{
		s_Instance->m_ActionList.push_back(new EmptyAction(name));
	}

	void DebugMenu::Add(const String& name, float* value)
	{
		Add(name, value, 0.0f, 100.0f);
	}

	void DebugMenu::Add(const String& name, float* value, float minimum, float maximum)
	{
		s_Instance->m_ActionList.push_back(new FloatAction(name, [value]() { return *value; }, [value](float v) { *value = v; }, minimum, maximum));
	}

	bool DebugMenu::IsVisible()
	{
		return s_Instance->m_Visible;
	}

	void DebugMenu::SetVisible(bool visible)
	{
		s_Instance->m_Visible = visible;
		if (visible)
			s_Instance->OnActivate();
		else
			s_Instance->OnDeactivate();
	}

	DebugMenuSettings& DebugMenu::GetSettings()
	{
		return s_Instance->m_Settings;
	}

	bool DebugMenu::OnMousePressed(events::MousePressedEvent& e)
	{
		return false;
	}

	bool DebugMenu::OnMouseReleased(events::MouseReleasedEvent& e)
	{
		return false;
	}

	void DebugMenu::OnActivate()
	{
		float width = 5.0f + m_Settings.padding;
		float height = 1.0f + m_Settings.padding;
		float yOffset = height;
		for (IAction* action : m_ActionList)
		{
			float y = 18.0f - yOffset;
			m_Panel->Add(new DebugMenuItem(action, Rectangle(0.0f, y, width, height)));
			yOffset += height;
		}

		m_Slider = new Slider({ width, 0.0f, 1.5f, 18.0f }, true);
		m_Panel->Add(m_Slider)->SetActive(false);
	}

	void DebugMenu::OnDeactivate()
	{
		m_Panel->Clear();
	}

	void DebugMenu::EditValue(float value, const Slider::ValueChangedCallback& callback)
	{
		m_Slider->SetCallback(callback);
		m_Slider->SetActive(true);
		m_Slider->SetValue(value);
	}

	void DebugMenu::OnUpdate()
	{
	}

	void DebugMenu::OnRender(graphics::Renderer2D& renderer)
	{
	}

} }