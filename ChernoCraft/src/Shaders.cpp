#include "Shaders.h"

#include <maths/maths.h>

sparky::graphics::Shader* Shaders::BLOCK = nullptr;
sparky::graphics::Shader* Shaders::HUD = nullptr;

void Shaders::Init()
{
	using namespace sparky::maths;

	mat4 pr_matrix = sparky::maths::mat4::Perspective(90.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	BLOCK = new sparky::graphics::Shader("res/shaders/shader.vert", "res/shaders/shader.frag");
	BLOCK->Bind();
	BLOCK->SetUniformMat4("pr_matrix", pr_matrix);
	BLOCK->SetUniformMat4("vw_matrix", sparky::maths::mat4::Translate(vec3(0.0f, 0.0f, -500.0f)));
	BLOCK->SetUniform1f("tex_id", 0);
	BLOCK->Unbind();

	mat4 ortho = mat4::Orthographic(-16.0f * 2.0f, 16.0f * 2.0f, 9.0f * 2.0f, -9.0f * 2.0f, -1.0f, 1.0f);
	HUD = new sparky::graphics::Shader("res/shaders/hud.vert", "res/shaders/hud.frag");
	HUD->Bind();
	HUD->SetUniformMat4("pr_matrix", ortho);
	HUD->Unbind();
}