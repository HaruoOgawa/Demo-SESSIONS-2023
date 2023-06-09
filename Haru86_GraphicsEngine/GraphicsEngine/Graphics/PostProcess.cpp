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
#include "CBloom.h"

//instanceを定義する
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
	m_BloomIntensity(0.0f),
	m_DoFOffset(0.0f),
	m_DoFPower(1.0f),
	m_Bloom(std::make_unique<CBloom>()),
	m_BloomTexture(std::make_shared<Texture>()),
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

	if (!GraphicsRenderer::GetInstance()->CreateFrameBuffer(GraphicsRenderer::GetInstance()->GetScreenSize().x, GraphicsRenderer::GetInstance()->GetScreenSize().y, m_BloomTexture, m_BloomFrameBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT)) {
		printf("Cannot Create FrameBuffer\n");
	}
}

void PostProcess::DrawPostProcess(const std::shared_ptr<Texture>& SrcTexture, const unsigned int& DestBuffer)const {
	// Draw Bloom
	m_Bloom->Draw(SrcTexture, m_BloomFrameBuffer);
	
	glBindFramebuffer(GL_FRAMEBUFFER, DestBuffer);
	glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x), static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_MeshRenderer->Draw([&]() {
		m_PostProcesCallBack();

		m_BloomTexture->SetActive(GL_TEXTURE0);
		m_MeshRenderer->m_material->SetTexUniform("_SrcTexture", 0);

		GraphicsRenderer::GetInstance()->p_r_DepthBlendingTexture->SetActive(GL_TEXTURE1);
		m_MeshRenderer->m_material->SetTexUniform("_DepthMap", 1);
	
		m_MeshRenderer->m_material->SetFloatUniform("_Offset", m_DoFOffset);
		m_MeshRenderer->m_material->SetFloatUniform("_Power", m_DoFPower);
	}, GL_TRIANGLES, false, 0);

	m_BloomTexture->SetEnactive(GL_TEXTURE0);
	GraphicsRenderer::GetInstance()->p_r_DepthBlendingTexture->SetEnactive(GL_TEXTURE1);
}
