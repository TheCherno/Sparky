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

#define BEGINCLASS(T) beginClass<T>(#T)
#define ENDCLASS endClass()
#define BEGINNAMESPACE(x) beginNamespace(x)
#define ENDNAMESPACE endNamespace()
#define ADDCONSTRUCTOR(...) addConstructor<void (*)(__VA_ARGS__)>()
#define ADDPROPERTY(N, T, g, s) addProperty(N, &T::g, &T::s)
#define ADDFUNCTION(T, x) addFunction(#x, &T::x)
#define ADDCFUNCTION(T, x) addCFunction(#x, &T::x)
#define ADDSTATICFUNCTION(T, x) addStaticFunction(#x, &T::x)

namespace sp { namespace scripting {

	void Load(lua_State* L)
	{
		getGlobalNamespace(L)
			.BEGINNAMESPACE("audio")
				.BEGINCLASS(Sound)
					.ADDCONSTRUCTOR(std::string, std::string)
					.ADDFUNCTION(Sound, Play)
					.ADDFUNCTION(Sound, Stop)
					.ADDFUNCTION(Sound, Loop)
					.ADDFUNCTION(Sound, Resume)
					.ADDFUNCTION(Sound, Pause)
					.ADDFUNCTION(Sound, IsPlaying)
					.ADDFUNCTION(Sound, GetFileName)
					.ADDFUNCTION(Sound, GetName)
					.ADDPROPERTY("Gain", Sound, GetGain, SetGain)
				.ENDCLASS
				.BEGINCLASS(SoundManager)
					.ADDSTATICFUNCTION(SoundManager, Add)
					.ADDSTATICFUNCTION(SoundManager, Get)
				.ENDCLASS
			.ENDNAMESPACE;
	}

} } 