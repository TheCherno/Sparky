#pragma once

#include <sp/graphics/layers/Layer.h>

#include "sp/cli/SparkyCLI.h"
#include "sp/cli/events/IEventListener.h"

namespace sp { namespace cli { namespace graphics {

	public ref class Layer : public sp::cli::events::IEventListener
	{
	internal:
		Layer(sp::graphics::Layer* instance);
	public:
		Layer();

		bool IsVisible();
		void SetVisible(bool visible);

		virtual void Init();
		virtual void OnEvent(sp::cli::events::Event^ e) override;
		virtual void OnTick();
		virtual void OnUpdate();
		virtual void OnRender();
	protected:
		virtual bool OnResize(uint width, uint height);
	};

} } }