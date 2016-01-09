#pragma once

#include "sp/sp.h"
#include "sp/graphics/Renderer2D.h"
#include "sp/graphics/ui/Panel.h"

#include "DebugMenuAction.h"
#include "DebugMenuItem.h"
#include "DebugMenuSlider.h"
#include "sp/graphics/ui/Slider.h"

namespace sp { namespace debug {

	typedef std::vector<IAction*> ActionList;

	struct DebugMenuSettings
	{
		float padding;
		float fontSize;
	};

	class SP_API DebugMenu
	{
	private:
		static DebugMenu* s_Instance;
	private:
		bool m_Visible;
		DebugMenuSettings m_Settings;
		ActionList m_ActionList;

		// DebugMenuSlider* m_Slider;
		graphics::ui::Panel* m_Panel;
		graphics::ui::Slider* m_Slider;
	public:
		static DebugMenu* Get();

		static void Init();
		static void Add(const String& name);
		static void Add(const String& name, float* value);
		static void Add(const String& name, float* value, float mininmum, float maximum);

		static bool IsVisible();
		static void SetVisible(bool visible);

		static DebugMenuSettings& GetSettings();

		void OnActivate();
		void OnDeactivate();
		void EditValue(float value, const graphics::ui::Slider::ValueChangedCallback& callback);

		bool OnMousePressed(events::MousePressedEvent& e);
		bool OnMouseReleased(events::MouseReleasedEvent& e);
		void OnUpdate();
		void OnRender(graphics::Renderer2D& renderer);
	private:
		DebugMenu();
	};

} }