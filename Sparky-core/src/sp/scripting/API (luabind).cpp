#include "sp/sp.h"
#include "API.h"

#include <luabind\luabind.hpp>
#include <luabind\operator.hpp>

#include "sp\debug\DebugMenu.h"

#include "sp\graphics\ui\Button.h"
#include "sp\graphics\ui\Panel.h"
#include "sp\graphics\ui\Slider.h"
#include "sp\graphics\ui\Widget.h"

#include "sp\entity\component\Components.h"
#include "sp\entity\Entity.h"

#include "Sparky.h"

using namespace sp;
using namespace audio;
using namespace debug;
using namespace entity;
using namespace events;
using namespace graphics;
using namespace maths;
using namespace ui;
using namespace component;

using namespace luabind;

namespace sp { namespace scripting {

	template <typename Enumeration>
	auto as_integer(Enumeration const value)
		-> typename std::underlying_type<Enumeration>::type
	{
		return static_cast<typename std::underlying_type<Enumeration>::type>(value);
	}
	
	void RegisterAudio(lua_State* L) {
		module(L)
		[
			class_<Sound>("Sound")
			.def(constructor<std::string, std::string>())
			.def("Play", &Sound::Play)
			.def("Stop", &Sound::Stop)
			.def("Loop", &Sound::Loop)
			.def("Resume", &Sound::Resume)
			.def("Pause", &Sound::Pause)
			.def("GetGain", &Sound::GetGain)
			.def("SetGain", &Sound::SetGain)
			.def("IsPlaying", &Sound::IsPlaying)
			.def("GetFileName", &Sound::GetFileName)
			.def("GetName", &Sound::GetName)
		];

		module(L)
		[
			class_<SoundManager>("SoundManager")
			.scope
			[
				def("Add", &SoundManager::Add),
				def("Get", &SoundManager::Get)
			]
		];
	}

	void RegisterDebug(lua_State* L)
	{
		module(L)
		[
			class_<DebugMenu>("DebugMenu")
			.scope
				[
					def("Add", ((void(*)(const std::string&)) &DebugMenu::Add)),
					def("AddBool", ((void(*)(const std::string&, bool*)) &DebugMenu::Add)),
					def("AddFloat", ((void(*)(const std::string&, float*)) &DebugMenu::Add)),
					def("AddFloatMinMax", ((void(*)(const std::string&, float*, float, float)) &DebugMenu::Add)),
					def("AddVec2", ((void(*)(const std::string&, vec2*, float, float)) &DebugMenu::Add)),
					def("AddVec3", ((void(*)(const std::string&, vec3*, float, float)) &DebugMenu::Add)),
					def("AddVec4", ((void(*)(const std::string&, vec4*, float, float)) &DebugMenu::Add)),
					def("Remove", &DebugMenu::Remove),
					def("GetSettings", &DebugMenu::GetSettings)
				]
			.def("EditValues", &DebugMenu::EditValues)
		];

		module(L)
		[
			class_<DebugMenuSettings>("DebugMenuSettings")
			.def_readwrite("padding", &DebugMenuSettings::padding)
			.def_readwrite("fontSize", &DebugMenuSettings::fontSize)
		];

		module(L)
		[
			class_<DebugMenuItem, Button>("DebugMenuItem")
			.def(constructor<IAction*, const Rectangle&>())
			.def("GetAction", &DebugMenuItem::GetAction)
		];

		module(L)
		[
			class_<DebugMenuSlider>("DebugMenuSlider")
		];

		module(L)
		[
			class_<DebugLayer, Layer2D>("DebugLayer")
		];
	}

	void RegisterEntity(lua_State* L)
	{
		module(L)
		[
			class_<Component>("Component")
			.def(constructor<>())
			.def("GetEntity", &Component::GetEntity)
			.def("GetType", &Component::GetType)
		];

		module(L)
		[
			class_<ComponentType>("ComponentType")
			.def(constructor<>())
			.def_readwrite("name", &ComponentType::name)
		];

		module(L)
		[
			class_<Entity>("Entity")
			.def(constructor<>())
			.def("AddComponent", &Entity::AddComponent)
		];

		module(L)
		[
			class_<MeshComponent, Component>("MeshComponent")
			.def(constructor<Mesh*>())
			.def_readwrite("mesh", &MeshComponent::mesh)
		];

		module(L)
		[
			class_<TransformComponent, Component>("TransformComponent")
			.def(constructor<const mat4&>())
			.def_readwrite("transform", &TransformComponent::transform)
		];
	}

	void RegisterEvents(lua_State* L)
	{

		module(L)
		[
			class_<Event>("Event")
			.def("IsHandled", &Event::IsHandled)
			.def("ToString", &Event::ToString)
			.scope[ def("TypeToString", &Event::TypeToString) ]
			.enum_("Type")
			[
				value("KEY_PRESSED", as_integer(Event::Type::KEY_PRESSED)),
				value("KEY_RELEASED", as_integer(Event::Type::KEY_RELEASED)),
				value("MOUSE_PRESSED", as_integer(Event::Type::MOUSE_PRESSED)),
				value("MOUSE_RELEASED", as_integer(Event::Type::MOUSE_RELEASED)),
				value("MOUSE_MOVED", as_integer(Event::Type::MOUSE_MOVED)),
				value("WINDOW_RESIZE", as_integer(Event::Type::WINDOW_RESIZE))
			]
			// .def("GetType", &Event::GetType)
		];

		module(L)
		[
			class_<KeyEvent, Event>("KeyEvent")
			.def(constructor<int32, Event::Type>())
			.def("GetKeyCode", &KeyEvent::GetKeyCode)
			.scope[def("GetStaticType", &KeyEvent::GetStaticType)]
		];

		module(L)
		[
			class_<KeyPressedEvent, Event>("KeyPressedEvent")
			.def(constructor<int32, int32, int32>())
			.def("GetRepeat", &KeyPressedEvent::GetRepeat)
			.def("GetModifiers", &KeyPressedEvent::GetModifiers)
			.def("IsModifier", &KeyPressedEvent::IsModifier)
			.scope[def("GetStaticType", &KeyEvent::GetStaticType)]
		];

		module(L)
		[
			class_<KeyReleasedEvent, Event>("KeyReleasedEvent")
			.def(constructor<int32>())
			.scope[def("GetStaticType", &KeyEvent::GetStaticType)]
		];

		module(L)
		[
			class_<MouseButtonEvent, Event>("MouseButtonEvent")
			// .def(constructor<int32, float, float, Event::Type>())
			.def("GetX", &MouseButtonEvent::GetX)
			.def("GetY", &MouseButtonEvent::GetY)
			.def("GetPosition", &MouseButtonEvent::GetPosition)
			.def("GetButton", &MouseButtonEvent::GetButton)
			.scope[def("GetStaticType", &KeyEvent::GetStaticType)]
		];

		module(L)
		[
			class_<MousePressedEvent, Event>("MousePressedEvent")
			.def(constructor<int32, float, float>())
			.def("ToString", &MousePressedEvent::ToString)
			.scope[def("GetStaticType", &KeyEvent::GetStaticType)]
		];

		module(L)
		[
			class_<MouseReleasedEvent, Event>("MouseReleasedEvent")
			.def(constructor<int32, float, float>())
			.scope[def("GetStaticType", &KeyEvent::GetStaticType)]
		];

		module(L)
		[
			class_<MouseMovedEvent, Event>("MouseMovedEvent")
			.def(constructor<float, float, bool>())
			.def("GetX", &MouseMovedEvent::GetX)
			.def("GetY", &MouseMovedEvent::GetY)
			.def("GetPosition", &MouseMovedEvent::GetPosition)
			.def("IsDragged", &MouseMovedEvent::IsDragged)
			.scope[def("GetStaticType", &KeyEvent::GetStaticType)]
		];

		module(L)
		[
			class_<ResizeWindowEvent, Event>("ResizeWindowEvent")
			.def(constructor<uint, uint>())
			.def("GetWidth", &ResizeWindowEvent::GetWidth)
			.def("GetHeight", &ResizeWindowEvent::GetHeight)
			.def("GetSize", &ResizeWindowEvent::GetSize)
			.scope[def("GetStaticType", &KeyEvent::GetStaticType)]
		];
	}

	void RegisterGraphics(lua_State* L)
	{
		module(L)
		[
			class_<Button>("Button")
		];

		module(L)
		[
			class_<Layer2D>("Layer2D")
		];

		module(L)
		[
			class_<IAction>("IAction")
		];

		module(L)
		[
			class_<Mesh>("Mesh")
		];

		module(L)
		[
			class_<Scene>("Scene")
			.def(constructor<>())
			.def("Add", &Scene::Add)
			.def("SetCamera", &Scene::SetCamera)
			.def("GetEntities", &Scene::GetEntities)
			.def("print", &Scene::print)
		];

		module(L)
		[
			class_<Camera>("Camera")
		];
	}

	void RegisterMaths(lua_State* L)
	{
		module(L)
		[
			class_<vec4>("vec4")
			.def(constructor<float, float, float, float>())
			.def_readwrite("x", &vec4::x)
			.def_readwrite("y", &vec4::y)
			.def_readwrite("z", &vec4::z)
			.def_readwrite("w", &vec4::w)
			.def("Add", &vec4::Add)
			.def("Subtract", &vec4::Subtract)
			.def("Multiply", (vec4&(vec4::*)(const vec4&))&vec4::Multiply)
			.def("Divide", &vec4::Divide)
			.def("Multiply", (vec4(vec4::*)(const mat4&) const)&vec4::Multiply)
			.def("Dot", &vec4::Dot)
			//.def(self + other<vec4>())
			//.def(self - other<vec4>())
			//.def(self * other<vec4>())
			//.def(self / other<vec4>())
		];

		module(L)
		[
			class_<vec3>("vec3")
			.def(constructor<float, float, float>())
			.def_readwrite("x", &vec3::x)
			.def_readwrite("y", &vec3::y)
			.def_readwrite("z", &vec3::z)
			.scope
			[
				def("Up", vec3::Up),
				def("Down", vec3::Down),
				def("Left", vec3::Left),
				def("Right", vec3::Right),
				def("Zero", vec3::Zero),
				def("XAxis", vec3::XAxis),
				def("YAxis", vec3::YAxis),
				def("ZAxis", vec3::ZAxis)
			]
			.def("Add", (vec3&(vec3::*)(const vec3&))&vec3::Add)
			.def("Subtract", (vec3&(vec3::*)(const vec3&))&vec3::Subtract)
			.def("Multiply", (vec3&(vec3::*)(const vec3&))&vec3::Multiply)
			.def("Divide", (vec3&(vec3::*)(const vec3&))&vec3::Divide)
			.def("Add", (vec3&(vec3::*)(float))&vec3::Add)
			.def("Subtract", (vec3&(vec3::*)(float))&vec3::Subtract)
			.def("Multiply", (vec3&(vec3::*)(float))&vec3::Multiply)
			.def("Divide", (vec3&(vec3::*)(float))&vec3::Divide)
			.def("Multiply", (vec3(vec3::*)(const mat4&) const)&vec3::Multiply)
			.def("Cross", &vec3::Cross)
			.def("Dot", &vec3::Dot)
			.def("Magnitude", &vec3::Magnitude)
			.def("Normalize", &vec3::Normalize)
			.def("Distance", &vec3::Distance)
			//.def(self + other<vec3>())
			//.def(self - other<vec3>())
			//.def(self * other<vec3>())
			//.def(self / other<vec3>())
			//.def(self + float())
			//.def(self - float())
			//.def(self * float())
			//.def(self / float())
		];

		module(L)
		[
			class_<vec2>("vec2")
			.def(constructor<float, float>())
			.def_readwrite("x", &vec2::x)
			.def_readwrite("y", &vec2::y)
			.def("Add", (vec2&(vec2::*)(const vec2&))&vec2::Add)
			.def("Subtract", (vec2&(vec3::*)(const vec2&))&vec2::Subtract)
			.def("Multiply", (vec2&(vec3::*)(const vec2&))&vec2::Multiply)
			.def("Divide", (vec2&(vec2::*)(const vec2&))&vec2::Divide)
			.def("Add", (vec2&(vec2::*)(float))&vec2::Add)
			.def("Subtract", (vec2&(vec2::*)(float))&vec2::Subtract)
			.def("Multiply", (vec2&(vec2::*)(float))&vec2::Multiply)
			.def("Divide", (vec2&(vec2::*)(float))&vec2::Divide)
			.def("Dot", &vec2::Dot)
			.def("Magnitude", &vec2::Magnitude)
			.def("Normalize", &vec2::Normalise)
			.def("Distance", &vec2::Distance)
			//.def(self + other<vec2>())
			//.def(self - other<vec2>())
			//.def(self * other<vec2>())
			//.def(self / other<vec2>())
			//.def(self + float())
			//.def(self - float())
			//.def(self * float())
			//.def(self / float())
			//.def(self == other<vec2>())
			//.def(self < other<vec2>())
			//.def(self <= other<vec2>())
			.def("ToString", &vec2::ToString)
		];

		module(L)
		[
			class_<Rectangle>("Rectangle")
			.def(constructor<float, float, float, float>())
			.def("Intersects", &Rectangle::Intersects)
			.def("Contains", (bool(Rectangle::*)(const vec2&) const)&Rectangle::Contains)
			.def("Contains", (bool(Rectangle::*)(const vec3&) const)&Rectangle::Contains)
			.def("Center", &Rectangle::Center)
			//.def(self == other<Rectangle>())
			//.def(self < other<Rectangle>())
		];

		module(L)
		[
			class_<AABB>("AABB")
			.def(constructor<float, float, float, float, float, float>())
			.def("Intersects", &AABB::Intersects)
			.def("Contains", (bool(AABB::*)(const vec2&) const)&AABB::Contains)
			.def("Contains", (bool(AABB::*)(const vec3&) const)&AABB::Contains)
			.def("Center", &AABB::Center)
			//.def(self == other<AABB>())
			//.def(self < other<AABB>())
		];

		module(L)
		[
			class_<mat4>("mat4")
			.def(constructor<vec4, vec4, vec4, vec4>())
			.scope
			[
				def("Orthographic", &mat4::Orthographic),
				def("Perspective", &mat4::Perspective),
				def("LookAt", &mat4::LookAt),
				def("Translate", &mat4::Translate),
				def("Rotate", (mat4(*)(float, const vec3&))&mat4::Rotate),
				def("Rotate", (mat4(*)(const Quaternion&))&mat4::Rotate),
				def("Scale", &mat4::Scale),
				def("Invert", (mat4(*)(const mat4&))&mat4::Invert),
				def("Transpose", &mat4::Transpose)
			]
			.def("Invert", (mat4&(mat4::*)())&mat4::Invert)
			.def("ToString", &mat4::ToString)
			//.def(self * other<mat4>())
		];

		module(L)
		[
			class_<Quaternion>("Quaternion")
			.def(constructor<float, float, float, float>())
			.def_readwrite("x", &Quaternion::x)
			.def_readwrite("y", &Quaternion::y)
			.def_readwrite("z", &Quaternion::z)
			.def_readwrite("w", &Quaternion::w)
			.def("Conjugate", &Quaternion::Conjugate)
			.def("Dot", &Quaternion::Dot)
			//.def(self + other<Quaternion>())
			//.def(self - other<Quaternion>())
			//.def(self * other<Quaternion>())
			//.def(self / other<Quaternion>())
			//.def(self * float())
			//.def(self / float())
			//.def(self == other<Quaternion>())
			.scope
			[
				def("Identity", &Quaternion::Identity),
				def("FromEulerAngles", &Quaternion::FromEulerAngles),
				def("Rotate", &Quaternion::Rotate),
				def("Rotation", (const Quaternion(*)(const vec3&, const vec3&))&Quaternion::Rotation),
				def("Rotation", (const Quaternion(*)(float, const vec3&))&Quaternion::Rotation),
				def("RotationX", &Quaternion::RotationX),
				def("RotationY", &Quaternion::RotationY),
				def("RotationZ", &Quaternion::RotationZ)
			]
		];
	}

	void Load(lua_State* L)
	{
		RegisterEntity(L);
		RegisterGraphics(L);
		RegisterMaths(L);
		RegisterEvents(L);
		RegisterDebug(L);
		RegisterAudio(L);
	}

} } 