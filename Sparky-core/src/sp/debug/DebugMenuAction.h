#pragma once

#include "sp/sp.h"
#include "sp/String.h"

#include "DebugMenu.h"
#include "sp/graphics/ui/Slider.h"

#include "sp/maths/maths.h"

namespace sp { namespace debug {

	struct IAction
	{
		enum class Type
		{
			NONE = 0, EMPTY, PATH, BOOLEAN, VALUE, VEC2, VEC3, VEC4
		};

		String name;
		Type type;

		virtual void OnAction() = 0;
		virtual String ToString() const = 0;
	};

	typedef std::vector<IAction*> ActionList;

	struct EmptyAction : public IAction
	{
		EmptyAction(const String& name)
		{
			this->name = name;
			type = Type::EMPTY;
		}

		void OnAction() override {}
		String ToString() const override { return name; }
	};

	struct CustomAction : public IAction
	{
	private:
		std::function<void()> m_Function;
	public:
		CustomAction(const String& name, const std::function<void()>& function)
			: m_Function(function)
		{
			this->name = name;
		}

		void OnAction() override
		{
			m_Function();
		}

		String ToString() const override
		{
			return name;
		}
	};

	struct BackAction : public IAction
	{
		PathAction* destination;

		BackAction(PathAction* destination)
		{
			this->name = "..  ";
			this->destination = destination;
			type = Type::PATH;
		}

		void OnAction() override
		{
			DebugMenu::SetPath(destination);
		}

		String ToString() const override
		{
			return name;
		}
	};

	struct PathAction : public IAction
	{
		ActionList actionList;
		PathAction* parent;

		PathAction(const String& name, PathAction* parent)
		{
			this->name = name;
			this->parent = parent;
			type = Type::PATH;
		}

		void OnAction() override
		{
			DebugMenu::SetPath(this);
		}

		String ToString() const override
		{
			return name + "  >";
		}

		bool ContainsAction(const String& name)
		{
			for (IAction* action : actionList)
			{
				if (action->name == name)
					return true;
			}
			return false;
		}

		PathAction* FindPath(const String& name)
		{
			for (IAction* action : actionList)
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

		bool DeleteChild(PathAction* child)
		{
			for (uint i = 0; i < actionList.size(); i++)
			{
				if (actionList[i] == child)
				{
					spdel actionList[i];
					actionList.erase(actionList.begin() + i);
					return true;
				}
			}
			return false;
		}
	};

	struct BooleanAction : public IAction
	{
		using Getter = std::function<bool()>;
		using Setter = std::function<void(bool)>;
	private:
		Getter m_Getter;
		Setter m_Setter;
	public:
		BooleanAction(const String& name, const Getter& getter, const Setter& setter)
			: m_Getter(getter), m_Setter(setter) { this->name = name; }

		void OnAction() override
		{
			m_Setter(!m_Getter());
		}

		String ToString() const override
		{
			return name + "     " + (m_Getter() ? "v" : "x");
		}
	};

	template<typename T>
	struct ValueAction : public IAction
	{
		using Getter = std::function<T()>;
		using Setter = std::function<void(T)>;
	private:
		Getter m_Getter;
		Setter m_Setter;
		T m_Min, m_Max;
	public:
		ValueAction(const String& name, const Getter& getter, const Setter& setter, T minValue, T maxValue)
			: m_Getter(getter), m_Setter(setter), m_Min(minValue), m_Max(maxValue) { this->name = name; }

		void OnAction() override
		{
			SP_ASSERT(false, "Not implemented!");
		}

		String ToString() const override
		{
			return name + " " + StringFormat::ToString(m_Getter());
		}
	};

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

	typedef ValueAction<int32> IntAction;
	typedef ValueAction<float> FloatAction;

	typedef ValueAction<maths::vec2> Vec2Action;
	typedef ValueAction<maths::vec3> Vec3Action;
	typedef ValueAction<maths::vec4> Vec4Action;

} }