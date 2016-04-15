#include "sp/sp.h"
#include "DebugMenu.h"

#include "DebugMenuAction.h"

#include "sp/app/Window.h"
#include "sp/graphics/ui/Button.h"

#include "sp/system/Memory.h"

namespace sp { namespace debug {

	using namespace maths;
	using namespace graphics;
	using namespace ui;

	DebugMenu* DebugMenu::s_Instance = nullptr;

	DebugMenu::DebugMenu()
		: m_Visible(false), m_Slider(nullptr)
	{
		s_Instance = this;

		m_Settings.padding = 0.75f;
		m_Settings.fontSize = 24.0f;

		Add("Padding", &m_Settings.padding, 0.0f, 2.0f);
		Add("Font Size", &m_Settings.fontSize, 8.0f, 48.0f);

		m_Slider = spnew Slider*[4];
		m_Panel = spnew Panel();
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

	void DebugMenu::Add(const String& name, bool* value)
	{
		s_Instance->m_ActionList.push_back(new BooleanAction(name, [value]() { return *value; }, [value](bool v) { *value = v; }));
	}

	void DebugMenu::Add(const String& name, float* value)
	{
		Add(name, value, 0.0f, 100.0f);
	}

	void DebugMenu::Add(const String& name, float* value, float minimum, float maximum)
	{
		s_Instance->m_ActionList.push_back(new FloatAction(name, [value]() { return *value; }, [value](float v) { *value = v; }, minimum, maximum));
	}

	void DebugMenu::Add(const String& name, vec2* value, float minimum, float maximum)
	{
		s_Instance->m_ActionList.push_back(new Vec2Action(name, [value]() { return *value; }, [value](vec2 v) { *value = v; }, vec2(minimum), vec2(maximum)));
	}

	void DebugMenu::Add(const String& name, vec3* value, float minimum, float maximum)
	{
		s_Instance->m_ActionList.push_back(new Vec3Action(name, [value]() { return *value; }, [value](vec3 v) { *value = v; }, vec3(minimum), vec3(maximum)));
	}

	void DebugMenu::Add(const String& name, vec4* value, float minimum, float maximum)
	{
		s_Instance->m_ActionList.push_back(new Vec4Action(name, [value]() { return *value; }, [value](vec4 v) { *value = v; }, vec4(minimum), vec4(maximum)));
	}

	void DebugMenu::Remove(const String& name)
	{
		auto& actions = s_Instance->m_ActionList;
		for (uint i = 0; i < actions.size(); i++)
		{
			if (actions[i]->name == name)
			{
				spdel actions[i];
				actions.erase(actions.begin() + i);
				break;
			}
		}
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
		float width = 0.0f;
		float height = 1.0f + m_Settings.padding;
		float yOffset = height;
		for (IAction* action : m_ActionList)
		{
			float y = 18.0f - yOffset;
			DebugMenuItem* item = spnew DebugMenuItem(action, Rectangle(0.0f, y, 0.0f, height));
			m_Panel->Add(item);
			yOffset += height;

			const Font& font = item->GetFont();
			float stringWidth = font.GetWidth(item->GetLabel());
			if (stringWidth > width)
				width = stringWidth;
		}

		width += m_Settings.padding;
		for (Widget* widget : m_Panel->GetWidgets())
		{
			DebugMenuItem* item = (DebugMenuItem*)widget;
			item->GetBounds().width = width;
		}

		for (uint i = 0; i < 4; i++)
		{
			m_Slider[i] = spnew Slider({ width + i * 1.5f, 0.0f, 1.5f, 18.0f }, true);
			m_Panel->Add(m_Slider[i])->SetActive(false);
		}
	}

	void DebugMenu::OnDeactivate()
	{
		m_Panel->Clear();
	}

	void DebugMenu::EditValues(const String& name, float* values, uint count, const Slider::ValueChangedCallback* callbacks)
	{
		for (uint i = 0; i < 4; i++)
		{
			m_Slider[i]->SetActive(false);
			if (i < count)
			{
				m_Slider[i]->SetCallback(callbacks[i]);
				m_Slider[i]->SetActive(m_LastEditedName != name);
				m_Slider[i]->SetValue(values[i]);
			}
		}
		m_LastEditedName = m_LastEditedName != name ? name : "";
	}

	void DebugMenu::OnUpdate()
	{
	}

	void DebugMenu::OnRender(graphics::Renderer2D& renderer)
	{
	}

} }