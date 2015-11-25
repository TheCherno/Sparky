#pragma once

#include <vector>
#include <sp/sparkygl.h>

#include "Font.h"
#include "Texture.h"
#include "Mask.h"
#include "../maths/maths.h"
#include "postfx/PostEffects.h"

namespace sp { namespace graphics {

	class Renderable2D;

	enum class RenderTarget
	{
		SCREEN = 0,
		BUFFER = 1
	};

	class Renderer2D
	{
	protected:
		std::vector<maths::mat4> m_TransformationStack;
		const maths::mat4* m_TransformationBack;
		const Mask* m_Mask;
		RenderTarget m_Target;
		PostEffects* m_PostEffects;
		bool m_PostEffectsEnabled;
	protected:
		Renderer2D()
			: m_Mask(nullptr), m_PostEffectsEnabled(true)
		{
			m_TransformationStack.push_back(maths::mat4::Identity());
			m_TransformationBack = &m_TransformationStack.back();
			m_Target = RenderTarget::SCREEN;
		}
	public:
		virtual ~Renderer2D() { }
		void Push(const maths::mat4& matrix, bool override = false)
		{
			if (override)
				m_TransformationStack.push_back(matrix);
			else
				m_TransformationStack.push_back(m_TransformationStack.back() * matrix);

			m_TransformationBack = &m_TransformationStack.back();
		}
		void Pop()
		{
			// TODO: Add to log!
			if (m_TransformationStack.size() > 1)
				m_TransformationStack.pop_back();

			m_TransformationBack = &m_TransformationStack.back();
		}

		inline void SetRenderTarget(RenderTarget target) { m_Target = target; }
		inline const RenderTarget GetRenderTarget() const { return m_Target; }

		inline void SetPostEffects(bool enabled) { m_PostEffectsEnabled = enabled; }
		inline bool GetPostEffects() const { return m_PostEffectsEnabled; }
		inline void AddPostEffectsPass(PostEffectsPass* pass) { m_PostEffects->Push(pass); }

		virtual void SetMask(const Mask* mask) { m_Mask = mask; }

		virtual void Begin() {}
		virtual void Submit(const Renderable2D* renderable) = 0;
		virtual void DrawString(const String& text, const maths::vec3& position, const Font& font, unsigned int color) { }
		virtual void End() {}
		virtual void Flush() = 0;
	};

} }