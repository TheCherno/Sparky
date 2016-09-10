#include "sp/sp.h"
#include "DebugMenuAction.h"

namespace sp { namespace debug {

	template<>
	void ValueAction<float>::OnAction()
	{
		float values[1] = { (m_Getter() - m_Min) / (m_Max - m_Min) };
		graphics::ui::Slider::ValueChangedCallback callback[1] = { [&](float value) { m_Setter(value * (m_Max - m_Min) + m_Min); } };
		DebugMenu::Get()->EditValues(name, values, 1, callback);
	}

	template<>
	String ValueAction<float>::ToString() const
	{
		return name + " " + StringFormat::Float(m_Getter());
	}

	template<>
	void ValueAction<maths::vec2>::OnAction()
	{
		float values[2] =
		{
			(m_Getter().x - m_Min.x) / (m_Max.x - m_Min.x),
			(m_Getter().y - m_Min.y) / (m_Max.y - m_Min.y)
		};
		graphics::ui::Slider::ValueChangedCallback callbacks[2] =
		{
			[&](float value) { m_Setter(maths::vec2(value * (m_Max.x - m_Min.x) + m_Min.x, m_Getter().y)); },
			[&](float value) { m_Setter(maths::vec2(m_Getter().x, value * (m_Max.y - m_Min.y) + m_Min.y)); }
		};
		DebugMenu::Get()->EditValues(name, values, 2, callbacks);
	}

	template<>
	String ValueAction<maths::vec2>::ToString() const
	{
		return name + " " + StringFormat::Float(m_Getter().x) + ", " + StringFormat::Float(m_Getter().y);
	}

	template<>
	void ValueAction<maths::vec3>::OnAction()
	{
		float values[3] =
		{
			(m_Getter().x - m_Min.x) / (m_Max.x - m_Min.x),
			(m_Getter().y - m_Min.y) / (m_Max.y - m_Min.y),
			(m_Getter().z - m_Min.z) / (m_Max.z - m_Min.z)
		};
		graphics::ui::Slider::ValueChangedCallback callbacks[3] =
		{
			[&](float value) { m_Setter(maths::vec3(value * (m_Max.x - m_Min.x) + m_Min.x, m_Getter().y, m_Getter().z)); },
			[&](float value) { m_Setter(maths::vec3(m_Getter().x, value * (m_Max.y - m_Min.y) + m_Min.y, m_Getter().z)); },
			[&](float value) { m_Setter(maths::vec3(m_Getter().x, m_Getter().y, value * (m_Max.z - m_Min.z) + m_Min.z)); }
		};
		DebugMenu::Get()->EditValues(name, values, 3, callbacks);
	}

	template<>
	String ValueAction<maths::vec3>::ToString() const
	{
		return name + " " + StringFormat::Float(m_Getter().x) + ", " + StringFormat::Float(m_Getter().y) + ", " + StringFormat::Float(m_Getter().z);
	}

	template<>
	void ValueAction<maths::vec4>::OnAction()
	{
		float values[4] =
		{
			(m_Getter().x - m_Min.x) / (m_Max.x - m_Min.x),
			(m_Getter().y - m_Min.y) / (m_Max.y - m_Min.y),
			(m_Getter().z - m_Min.z) / (m_Max.z - m_Min.z),
			(m_Getter().w - m_Min.w) / (m_Max.w - m_Min.w)
		};
		graphics::ui::Slider::ValueChangedCallback callbacks[4] =
		{
			[&](float value) { m_Setter(maths::vec4(value * (m_Max.x - m_Min.x) + m_Min.x, m_Getter().y, m_Getter().z, m_Getter().w)); },
			[&](float value) { m_Setter(maths::vec4(m_Getter().x, value * (m_Max.y - m_Min.y) + m_Min.y, m_Getter().z, m_Getter().w)); },
			[&](float value) { m_Setter(maths::vec4(m_Getter().x, m_Getter().y, value * (m_Max.z - m_Min.z) + m_Min.z, m_Getter().w)); },
			[&](float value) { m_Setter(maths::vec4(m_Getter().x, m_Getter().y, m_Getter().z, value * (m_Max.w - m_Min.w) + m_Min.w)); }
		};
		DebugMenu::Get()->EditValues(name, values, 4, callbacks);
	}

	template<>
	String ValueAction<maths::vec4>::ToString() const
	{
		return name + " " + StringFormat::Float(m_Getter().x) + ", " + StringFormat::Float(m_Getter().y) + ", " + StringFormat::Float(m_Getter().z) + ", " + StringFormat::Float(m_Getter().w);
	}


} }
