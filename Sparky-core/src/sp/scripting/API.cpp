#include "sp/sp.h"
#include "API.h"

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

#define ADDFUNCTION(x) addFunction(#x, &x)
#define ADDCFUNCTION(x) addCFunction(#x, &x)
#define ADDSTATICFUNCTION(x) addStaticFunction(#x, &x)

namespace sp { namespace scripting {

	template <typename Enumeration>
	auto as_integer(Enumeration const value)
		-> typename std::underlying_type<Enumeration>::type
	{
		return static_cast<typename std::underlying_type<Enumeration>::type>(value);
	}
	
	void RegisterAudio(lua_State* L) {
		getGlobalNamespace(L)
			.beginClass<Sound>("Sound")
				.addConstructor<void (*)(std::string, std::string)>()
				.addFunction("Play", &Sound::Play)
				.addFunction("Stop", &Sound::Stop)
				.addFunction("Loop", &Sound::Loop)
				.addFunction("Resume", &Sound::Resume)
				.addFunction("Pause", &Sound::Pause)
				.addProperty("Gain", &Sound::GetGain, &Sound::SetGain)
				.addFunction("IsPlaying", &Sound::IsPlaying)
				.addFunction("GetFileName", &Sound::GetFileName)
				.addFunction("GetName", &Sound::GetName)
			.endClass();

		getGlobalNamespace(L)
			.beginClass<SoundManager>("SoundManager")
				.addStaticFunction("Add", SoundManager::Add)
				.addStaticFunction("Get", SoundManager::Get)
			.endClass();
	}

	void Load(lua_State* L)
	{
		RegisterAudio(L);
	}

} } 