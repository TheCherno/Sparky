#pragma once

#include "../SparkyCLI.h"
#include "../maths/Vector2.h"

#include <sp/app/Window.h>

namespace SparkyCLI {

    public ref class Window : public ManagedClass<sp::Window>
    {
    private:
        Window(sp::Window* instance);
    public:
        Window(System::String^ name, System::UInt32 width, System::UInt32 height);
        void Clear();
        void Update();
        bool Closed();

        inline uint GetWidth();
        inline uint GetHeight();

        void SetVsync(bool enabled);
        bool IsVsync();
    };

}