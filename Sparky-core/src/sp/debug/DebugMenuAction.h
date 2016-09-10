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

	template<> void ValueAction<float>::OnAction();
	template<> String ValueAction<float>::ToString() const;
 	template<> void ValueAction<maths::vec2>::OnAction();
	template<> String ValueAction<maths::vec2>::ToString() const;
	template<> void ValueAction<maths::vec3>::OnAction();
	template<> String ValueAction<maths::vec3>::ToString() const;
	template<> void ValueAction<maths::vec4>::OnAction();
	template<> String ValueAction<maths::vec4>::ToString() const;

	typedef ValueAction<int32> IntAction;
	typedef ValueAction<float> FloatAction;

	typedef ValueAction<maths::vec2> Vec2Action;
	typedef ValueAction<maths::vec3> Vec3Action;
	typedef ValueAction<maths::vec4> Vec4Action;

} }