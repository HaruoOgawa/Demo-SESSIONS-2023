#include "Forest.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "FlowerModel.h"
#include "LTreeModel.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "Flower.h"
#include "Stem.h"

namespace myapp
{
	Forest::Forest():
		m_FlowerModel(nullptr),
		m_LTreeModel(nullptr),
		m_EnergyBall(nullptr),
		m_Ground(nullptr),
		IsMountainRotate(true)
	{
		Start();
	}

	void Forest::Start() {
		// ��
		m_FlowerModel = std::make_shared<FlowerModel>();
		
		// ��
		m_LTreeModel = std::make_shared<LTreeModel>();

		// �G�i�W�[�{�[��
		m_EnergyBall = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RAYMARCHING,
			shaderlib::StandardRenderBoard_vert, 
			std::string(
				#include "../../Shader/Forest/EnergyBall.frag"
			)
		);
		m_EnergyBall->useZTest = false;
		m_EnergyBall->useAlphaTest = true;

		// �n��
		std::string MountainGround_frag = {
			#include "../../Shader/Forest/MountainGround.frag"
		};

		m_Ground = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::BOARD,
			RenderingSurfaceType::RASTERIZER, shaderlib::Standard_vert, MountainGround_frag);

		m_Ground->m_transform->m_rotation = glm::vec3(-3.14f / 2.0f, 0.0f, 0.0f);
		m_Ground->m_transform->m_scale = glm::vec3(500.0f);
		m_Ground->m_calllback = [this]() 
		{
			m_Ground->m_material->SetIntUniform("_UseColor", 1);
			m_Ground->m_material->SetVec4Uniform("_Color", glm::vec4(0.5f*glm::vec3(227.0 / 255.0, 206.0 / 255.0, 164.0 / 255.0), 1.0f));
		};

		// Not Use SSR
		PostProcess::GetInstance()->m_UseSSR = false;
	}

	void Forest::Update() {
		m_FlowerModel->Update();
		m_LTreeModel->Update();
	}

	void Forest::Draw(bool IsRaymarching, int SceneIndex, int LinearInstanceRate, bool IsReverseTime) {
		m_LTreeModel->SetIsStreetLineTree(false);
		if (IsRaymarching)
		{
			m_EnergyBall->Draw();
		}
		else
		{
			m_LTreeModel->m_TreeRenderer->m_transform->m_scale = glm::vec3(1.0f);
			//m_Ground->Draw();
			m_FlowerModel->Draw();
			int numOfTree = (IsReverseTime) ? 256 : 1024;
			m_LTreeModel->Draw(numOfTree);
		}
	}

	void Forest::UpdateTimeline(float LocalTime, bool IsReverseTime)
	{
		PostProcess::GetInstance()->m_LatePostProcesCallBack = [=]() {
			PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseVignette", 0);
			PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetIntUniform("_UseWhiteFade", 0);
			PostProcess::GetInstance()->m_LateMeshRenderer->m_material->SetFloatUniform("_WhiteFadeVal", 0.0f);
		};

		// �J�������[�N
		float CameraTimeModeRate = 3.75f, NumOfCamera = 4.0f;
		float AdjustTimeOffset = 3.75f * 0.5f;
		float CameraworkTime = glm::mod(LocalTime + AdjustTimeOffset, CameraTimeModeRate * NumOfCamera);
		
		IsMountainRotate = true;

		if (CameraworkTime >= 0.0f && CameraworkTime < CameraTimeModeRate * 1.0f)
		{
			IsMountainRotate = false;

			GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(
				LocalTime * 0.1f, 0.5f, LocalTime * 0.1f
			) * 5.0f;
			GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.5f, 0.0f) * 5.0f;
		}
		else if (CameraworkTime >= CameraTimeModeRate * 1.0f && CameraworkTime < CameraTimeModeRate * 2.0f)
		{
			GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(
				glm::cos(LocalTime * 0.1f), 225.0f, glm::sin(LocalTime * 0.1f)
			);
			GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else if (CameraworkTime >= CameraTimeModeRate * 2.0f && CameraworkTime < CameraTimeModeRate * 3.0f)
		{
			IsMountainRotate = false;

			GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(2.5f, 0.5f, 2.5f) * 5.0f;
			GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 2.5f, 0.0f) * 5.0f;
		}
		else if (CameraworkTime >= CameraTimeModeRate * 3.0f && CameraworkTime < CameraTimeModeRate * 4.0f)
		{
			GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(
				glm::cos(LocalTime * 0.1f) * 10.0f, 10.0f, glm::sin(LocalTime * 0.1f) * 10.0f
			);

			GraphicsMain::GetInstance()->m_MainCamera->m_center = glm::vec3(0.0f, 10.0f, 0.0f);
		}
	}
}