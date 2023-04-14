#include "CBloom.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

CBloom::CBloom():
	m_IlluminanceMap(std::make_shared<Texture>()),
	m_BlurMapX(std::make_shared<Texture>()),
	m_BlurMapY(std::make_shared<Texture>()),
	m_IlluminanceBuffer(0),
	m_BlurBufferX(0),
	m_BlurBufferY(0)
{
	Initialize();
}

void CBloom::Initialize() {
	m_IlluminanceMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER,shaderlib::StandardRenderBoard_vert, shaderlib::BloomIlluminance_frag, "", "", "", "");
	m_BlurMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, shaderlib::StandardRenderBoard_vert, shaderlib::BloomBlur_frag, "", "", "", "");
	m_BloomResultMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, shaderlib::StandardRenderBoard_vert, shaderlib::BloomResult_frag, "", "", "", "");
	m_mesh = std::make_shared<Mesh>(PrimitiveType::BOARD);

	// Create FrameBuffer  ��ق�HDRTexture�ɂ���
	if(!GraphicsRenderer::GetInstance()->CreateFrameBuffer(GraphicsRenderer::GetInstance()->GetScreenSize().x, GraphicsRenderer::GetInstance()->GetScreenSize().y,m_IlluminanceMap, m_IlluminanceBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT))printf("Cannot Create FrameBuffer\n");
	if(!GraphicsRenderer::GetInstance()->CreateFrameBuffer(GraphicsRenderer::GetInstance()->GetScreenSize().x, GraphicsRenderer::GetInstance()->GetScreenSize().y, m_BlurMapX, m_BlurBufferX, GL_RGBA16F, GL_RGBA, GL_FLOAT))printf("Cannot Create FrameBuffer\n");
	if(!GraphicsRenderer::GetInstance()->CreateFrameBuffer(GraphicsRenderer::GetInstance()->GetScreenSize().x, GraphicsRenderer::GetInstance()->GetScreenSize().y, m_BlurMapY, m_BlurBufferY, GL_RGBA16F, GL_RGBA, GL_FLOAT))printf("Cannot Create FrameBuffer\n");

	// weights�̌v�Z
	float blurPower = 1000.0; // from 10.0 to 1000.0
	float t = 0.0;
	float d = blurPower * blurPower *0.001;
	
	for (int i = 0; i < 10;i++) {
		float r = 2.0 * static_cast<float>(i);
		float w = glm::exp(-0.5 * (r * r) / d);
		weights.push_back(w);
		if (i > 0) { w *= 2.0; }
		t += w;
	}
	
	for (int i = 0; i < 10; i++) {
		weights[i] /= t;
	}
}

void CBloom::Draw(const std::shared_ptr<Texture> SrcTexture, const unsigned int& DestBuffer) {
	// Illuminance
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, m_IlluminanceBuffer);
	glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion), static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	m_IlluminanceMaterial->SetActive();
	m_IlluminanceMaterial->SetFloatUniform("_time", GraphicsMain::GetInstance()->m_SecondsTime);
	m_IlluminanceMaterial->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	m_IlluminanceMaterial->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
	SrcTexture->SetActive(GL_TEXTURE0);
	m_IlluminanceMaterial->SetTexUniform("_SrcTexture", 0);
	m_IlluminanceMaterial->SetFloatUniform("_BloomThreshold", PostProcess::GetInstance()->m_BloomThreshold);
	m_IlluminanceMaterial->SetFloatUniform("_BloomIntensity", PostProcess::GetInstance()->m_BloomIntensity);
	
	m_mesh->Draw();
	SrcTexture->SetEnactive(GL_TEXTURE0);


	// BlurX
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, m_BlurBufferX);
	glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion), static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	m_BlurMaterial->SetActive();
	m_BlurMaterial->SetFloatUniform("_time", GraphicsMain::GetInstance()->m_SecondsTime);
	m_BlurMaterial->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	m_BlurMaterial->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
	if (m_IlluminanceMap) {
		m_IlluminanceMap->SetActive(GL_TEXTURE0);
		m_BlurMaterial->SetTexUniform("_IlluminanceTexture", 0);
	}
	m_BlurMaterial->SetFloatVectorUniform("_weights", weights);
	m_BlurMaterial->SetFloatUniform("_IsHorizontal", 1.0);
	m_mesh->Draw();
	m_IlluminanceMap->SetEnactive(GL_TEXTURE0);
	
	// BlurY
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, m_BlurBufferY);
	glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion), static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	m_BlurMaterial->SetActive();
	m_BlurMaterial->SetFloatUniform("_time", GraphicsMain::GetInstance()->m_SecondsTime);
	m_BlurMaterial->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	m_BlurMaterial->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
	if (m_BlurMapX) {
		m_BlurMapX->SetActive(GL_TEXTURE0);
		m_BlurMaterial->SetTexUniform("_IlluminanceTexture", 0);
		
	}
	m_BlurMaterial->SetFloatVectorUniform("_weights", weights);
	m_BlurMaterial->SetFloatUniform("_IsHorizontal", 0.0);
	m_mesh->Draw();
	m_BlurMapX->SetEnactive(GL_TEXTURE0);

	// Result
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, DestBuffer);
	glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion), static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	m_BloomResultMaterial->SetActive();
	m_BloomResultMaterial->SetFloatUniform("_time", GraphicsMain::GetInstance()->m_SecondsTime);
	m_BloomResultMaterial->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
	m_BloomResultMaterial->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
	if (m_BlurMapY) {
		m_BlurMapY->SetActive(GL_TEXTURE0);
		m_BloomResultMaterial->SetTexUniform("_BlurTexture", 0);
		
	}
	SrcTexture->SetActive(GL_TEXTURE1);
	m_BloomResultMaterial->SetTexUniform("_SrcTexture", 1);
	
	m_mesh->Draw();
	m_BlurMapY->SetEnactive(GL_TEXTURE0);
	SrcTexture->SetEnactive(GL_TEXTURE1);
}