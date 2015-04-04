#pragma once

#include <GL/glew.h>
#include "../maths/maths.h"
#include "renderable2d.h"

namespace sparky { namespace graphics {

	class Renderer2D
	{
	protected:
		std::vector<const maths::mat4> m_TransformationStack;
		const maths::mat4* m_TransformationBack;
	protected:
		Renderer2D()
		{
			m_TransformationStack.push_back(maths::mat4::identity());
			m_TransformationBack = &m_TransformationStack.back();
		}
	public:
		void push(const maths::mat4& matrix, bool override = false)
		{
			if (override)
				m_TransformationStack.push_back(matrix);
			else
				m_TransformationStack.push_back(m_TransformationStack.back() * matrix);

			m_TransformationBack = &m_TransformationStack.back();
		}
		void pop()
		{
			// TODO: Add to log!
			if (m_TransformationStack.size() > 1)
				m_TransformationStack.pop_back();

			m_TransformationBack = &m_TransformationStack.back();
		}

		virtual void begin() {}
		virtual void submit(const Renderable2D* renderable) = 0;
		virtual void end() {}
		virtual void flush() = 0;
	};

} }