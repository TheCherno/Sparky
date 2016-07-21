#pragma once

//
// Sparky Engine header file
//

#include <sp/app/Application.h>
#include <sp/app/Window.h>

#include <sp/audio/Sound.h>
#include <sp/audio/SoundManager.h>

#include <sp/debug/DebugLayer.h>
#include <sp/debug/DebugMenu.h>
#include <sp/debug/DebugMenuAction.h>
#include <sp/debug/DebugMenuItem.h>

#include <sp/entity/Entity.h>
#include <sp/entity/component/Component.h>
#include <sp/entity/component/MeshComponent.h>
#include <sp/entity/component/TransformComponent.h>

#include <sp/events/Event.h>
#include <sp/events/KeyEvent.h>
#include <sp/events/MouseEvent.h>

#include <sp/graphics/Renderer.h>
#include <sp/graphics/Renderer2D.h>
#include <sp/graphics/Batchrenderer2D.h>
#include <sp/graphics/Renderer3D.h>
#include <sp/graphics/ForwardRenderer.h>
#include <sp/graphics/DeferredRenderer.h>

#include <sp/graphics/Font.h>
#include <sp/graphics/FontManager.h>
#include <sp/graphics/GBuffer.h>
#include <sp/graphics/IRenderable.h>
#include <sp/graphics/Label.h>
#include <sp/graphics/Mask.h>
#include <sp/graphics/Material.h>
#include <sp/graphics/PBRMaterial.h>
#include <sp/graphics/Mesh.h>
#include <sp/graphics/MeshFactory.h>
#include <sp/graphics/Model.h>
#include <sp/graphics/Renderable2D.h>
#include <sp/graphics/RenderCommand.h>
#include <sp/graphics/Scene.h>
#include <sp/graphics/Sprite.h>
#include <sp/graphics/TextureManager.h>

#include <sp/graphics/camera/Camera.h>
#include <sp/graphics/camera/MayaCamera.h>
#include <sp/graphics/camera/FPSCamera.h>

#include <sp/graphics/layers/Group.h>
#include <sp/graphics/layers/Layer.h>
#include <sp/graphics/layers/Layer2D.h>
#include <sp/graphics/layers/Layer3D.h>

#include <sp/graphics/postfx/PostEffects.h>
#include <sp/graphics/postfx/PostEffectsPass.h>

#include <sp/graphics/shaders/Shader.h>
#include <sp/graphics/shaders/ShaderFactory.h>
#include <sp/graphics/shaders/ShaderManager.h>
#include <sp/graphics/shaders/ShaderUniform.h>

#include <sp/graphics/API/Texture.h>
#include <sp/graphics/API/Texture2D.h>
#include <sp/graphics/API/TextureCube.h>
#include <sp/graphics/API/TextureDepth.h>
#include <sp/graphics/API/Framebuffer.h>
#include <sp/graphics/API/Framebuffer2D.h>
#include <sp/graphics/API/FramebufferDepth.h>

#include <sp/maths/AABB.h>
#include <sp/maths/mat4.h>
#include <sp/maths/maths_func.h>
#include <sp/maths/Quaternion.h>
#include <sp/maths/tvec2.h>
#include <sp/maths/vec2.h>
#include <sp/maths/vec3.h>
#include <sp/maths/vec4.h>

#include <sp/utils/ImageLoad.h>
#include <sp/utils/Log.h>
#include <sp/utils/Timer.h>
#include <sp/utils/Timestep.h>

#include <sp/system/FileSystem.h>
#include <sp/system/Memory.h>
#include <sp/system/MemoryManager.h>
#include <sp/system/VFS.h>

#include <sp/String.h>
#include <sp/Types.h>
