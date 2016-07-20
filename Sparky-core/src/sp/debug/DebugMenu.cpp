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
		: m_Visible(false), m_Slider(nullptr), m_Path(nullptr)
	{
		s_Instance = this;

		m_Settings.padding = 0.75f;
		m_Settings.fontSize = 24.0f;

		Add("Debug Menu/Padding", &m_Settings.padding, 0.0f, 2.0f);
		Add("Debug Menu/Font Size", &m_Settings.fontSize, 8.0f, 48.0f);

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
		s_Instance->m_ActionList.push_back(spnew EmptyAction(name));
	}

	void DebugMenu::Add(const String& name, bool* value)
	{
		s_Instance->m_ActionList.push_back(spnew BooleanAction(name, [value]() { return *value; }, [value](bool v) { *value = v; }));
	}

	void DebugMenu::Add(const String& name, float* value)
	{
		Add(name, value, 0.0f, 100.0f);
	}

	void DebugMenu::Add(const String& name, float* value, float minimum, float maximum)
	{
		s_Instance->Add(name, spnew FloatAction(name, [value]() { return *value; }, [value](float v) { *value = v; }, minimum, maximum));
	}

	void DebugMenu::Add(const String& name, vec2* value, float minimum, float maximum)
	{
		s_Instance->m_ActionList.push_back(spnew Vec2Action(name, [value]() { return *value; }, [value](vec2 v) { *value = v; }, vec2(minimum), vec2(maximum)));
	}

	void DebugMenu::Add(const String& name, vec3* value, float minimum, float maximum)
	{
		s_Instance->m_ActionList.push_back(spnew Vec3Action(name, [value]() { return *value; }, [value](vec3 v) { *value = v; }, vec3(minimum), vec3(maximum)));
	}

	void DebugMenu::Add(const String& name, vec4* value, float minimum, float maximum)
	{
		s_Instance->m_ActionList.push_back(spnew Vec4Action(name, [value]() { return *value; }, [value](vec4 v) { *value = v; }, vec4(minimum), vec4(maximum)));
	}

	void DebugMenu::Add(const String& path, IAction* action)
	{
		if (StringContains(path, "/"))
		{
			std::vector<String> paths = SplitString(path, "/");
			action->name = paths.back();
			paths.pop_back();
			PathAction* pathAction = CreateOrFindPaths(paths);
			SP_ASSERT(pathAction);
			pathAction->actionList.push_back(action);
		}
		else
		{
			m_ActionList.push_back(action);
		}
	}

	PathAction* DebugMenu::CreateOrFindPaths(std::vector<String>& paths, PathAction* action)
	{
		if (paths.empty())
			return action;

		String name = paths.front();
		paths.erase(paths.begin());

		ActionList* actionList = action ? &action->actionList : &m_ActionList;
		for (IAction* a : *actionList)
		{
			if (a->type == IAction::Type::PATH && a->name == name)
				return CreateOrFindPaths(paths, (PathAction*)a);
		}

		PathAction* pathAction = spnew PathAction(name, action);
		actionList->push_back(pathAction);
		return CreateOrFindPaths(paths, pathAction);
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

	PathAction* DebugMenu::FindPath(const String& name)
	{
		for (IAction* action : m_ActionList)
		{
			if (action->type == IAction::Type::PATH)
			{
				PathAction* a = (PathAction*)action;
				if (a->name == name)
					return a;
				else
					a->FindPath(name);
			}
		}
		return nullptr;
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

	void DebugMenu::SetPath(PathAction* path)
	{
		s_Instance->m_Path = path;
		s_Instance->OnDeactivate();
		s_Instance->OnActivate();
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

		ActionList* actionList = m_Path ? &m_Path->actionList : &m_ActionList;
		if (m_Path)
		{
			DebugMenuItem* item = spnew DebugMenuItem(spnew BackAction(m_Path->parent), Rectangle(0.0f, 18.0f - yOffset, 0.0f, height));
			m_Panel->Add(item);
			yOffset += height;
			width = item->GetFont().GetWidth(item->GetLabel());
		}

		for (IAction* action : *actionList)
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