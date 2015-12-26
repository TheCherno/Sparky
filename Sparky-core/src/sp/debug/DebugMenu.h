#pragma once

namespace sp { namespace debug {

	class DebugMenu
	{
	private:
		static DebugMenu* s_Instance;
	private:
		bool m_Visible;
	public:
		static void Init();
		static void Add();

		static bool IsVisible();
		static void SetVisible(bool visible);
	private:
		DebugMenu();
	};

} }