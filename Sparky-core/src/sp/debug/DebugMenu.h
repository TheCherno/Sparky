#pragma once

#include "sp/sp.h"
#include "sp/graphics/Renderer2D.h"

namespace sp { namespace debug {

	struct IAction
	{
		String name;
		IAction(const String& name)
			: name(name) {}

		virtual String ToString() const { return name; }
	};

	template<typename T>
	struct ValueAction : IAction
	{
	private:
		using Getter = std::function<T()>;
		using Setter = std::function<void(T)>;

		Getter m_Getter;
		Setter m_Setter;
	public:
		ValueAction(const String& name, const Getter& getter, const Setter& setter)
			: IAction(name), m_Getter(getter), m_Setter(setter)
		{
		}
		String ToString() const override
		{
			return name + " " + std::to_string(m_Getter());
		}
	};

	typedef ValueAction<int>	IntAction;
	typedef ValueAction<float>	FloatAction;

	class DebugMenu
	{
	private:
		static DebugMenu* s_Instance;
	private:
		bool m_Visible;
		std::vector<IAction*> m_DebugMenuItems;

		float m_Padding, m_FontSize;
	public:
		static void Init();
		static void Add(const String& name);
		static void Add(const String& name, float* value);

		static bool IsVisible();
		static void SetVisible(bool visible);

		static void OnRender(graphics::Renderer2D& renderer);
	private:
		DebugMenu();
	};

} }