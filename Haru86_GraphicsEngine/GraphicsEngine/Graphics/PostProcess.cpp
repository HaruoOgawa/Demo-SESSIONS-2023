#include "PostProcess.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include <glew.h>
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"

//instance‚ð’è‹`‚·‚é
PostProcess* PostProcess::instance = nullptr;

void PostProcess::CreateInstance() {
	if (!instance)instance = new PostProcess();
}

PostProcess* PostProcess::GetInstance() {
	if (instance) return instance;
}

void PostProcess::DestroyInstance() {
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

PostProcess::PostProcess():
	m_UsePostProcess(false),
	m_PostProcesCallBack([]() {})
{
	m_MeshRenderer = std::make_shared<MeshRendererComponent>(
		std::make_shared<TransformComponent>(),
		PrimitiveType::BOARD,
		RenderingSurfaceType::RASTERIZER,
		shaderlib::StandardRenderBoard_vert,
		shaderlib::PostProcess_frag
	);
	m_MeshRenderer->useZTest = false;
}

void PostProcess::DrawPostProcess(const std::shared_ptr<Texture>& SrcTexture, const unsigned int& DestBuffer)const {
	glBindFramebuffer(GL_FRAMEBUFFER, DestBuffer);
	glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x), static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_MeshRenderer->Draw([&]() {
		m_PostProcesCallBack();
		SrcTexture->SetActive(GL_TEXTURE0);
		m_MeshRenderer->m_material->SetTexUniform("_SrcTexture", 0);
	
	}, GL_TRIANGLES, false, 0);
	SrcTexture->SetEnactive(GL_TEXTURE0);
}
