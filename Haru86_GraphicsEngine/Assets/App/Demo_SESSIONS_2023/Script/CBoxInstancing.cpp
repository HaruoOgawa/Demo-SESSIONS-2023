#include "CBoxInstancing.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"

namespace app
{
	CBoxInstancing::CBoxInstancing() :
		m_SideCubeNum(512),
		m_CubeNum(512 * 512),
		m_CubeWidth(1.0f),
		m_CubeThreads(512, 1, 1),

		m_InitSY(1.0f),
		m_MaxBoxHeight(80.0f),
		m_AddedBoxHeight(1.0f),
		m_Atten(0.5f),
		m_CommonYOffset(-25.0f),
		m_Brightness(2.0f),
		
		m_CubeMountain(nullptr),
		m_GPGPU(nullptr),
		m_cubeGroundBuffer(nullptr)
	{
		Start();
	}

	CBoxInstancing::~CBoxInstancing() {
	}

	void CBoxInstancing::Start() {
		m_CubeMountain = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::CUBE,
			RenderingSurfaceType::RASTERIZER,
			std::string({
				#include "Assets/App/Demo_SESSIONS_2023/shader/InstancedBox_VertComp_my.h"	
			}),
			std::string({
				#include "Assets/App/Demo_SESSIONS_2023/shader/InstancedBox_FragComp.h"	
			})
		);

		m_GPGPU = std::make_shared<MeshRendererComponent>(
			std::make_shared<TransformComponent>(),
			PrimitiveType::POINT,
			RenderingSurfaceType::RASTERIZER,
			"", "", "", "", "",
			std::string(
#include "Assets/App/Demo_SESSIONS_2023/shader/InstancedBox.comp"
			)
			);

		m_cubeGroundBuffer = std::make_shared<ComputeBuffer>(sizeof(SBoxData) * m_CubeNum);

		std::vector<SBoxData> init_BoxData;
		for (int y = 0; y < m_SideCubeNum; y++) {
			for (int x = 0; x < m_SideCubeNum; x++) {
				glm::vec3 Pos = glm::vec3(
					float(x - m_SideCubeNum * 0.5f) * m_CubeWidth,
					0.0f,
					float(y - m_SideCubeNum * 0.5f) * m_CubeWidth
				);
				SBoxData Data = {
					{Pos.x,Pos.y,Pos.z,1.0f},
					{0.0f,0.0f,0.0f,0.0f},
					{m_CubeWidth,m_CubeWidth,m_CubeWidth,rand(100.0f * glm::vec2(float(x), float(y)))},
					{0.0f,0.0f,0.0f,0.0f}
				};
				init_BoxData.push_back(Data);
			}
		}

		m_cubeGroundBuffer->SetData<std::vector<SBoxData>>(init_BoxData);

		//set buffer
		m_GPGPU->m_material->SetBufferToCS(m_cubeGroundBuffer, 3);
		m_CubeMountain->m_material->SetBufferToMat(m_cubeGroundBuffer, 3);
	}

	void CBoxInstancing::Update() {
		float time = GraphicsMain::GetInstance()->m_SecondsTime;

		auto& mat = m_GPGPU->m_material;
		mat->SetActive();
		mat->SetFloatUniform("_time", time);
		mat->SetFloatUniform("_CommonYO", m_CommonYOffset);
		mat->Dispatch(m_CubeNum / m_CubeThreads.x, 1, 1);
	}

	void CBoxInstancing::Draw() {
		m_CubeMountain->Draw([&]() {
			m_CubeMountain->m_material->SetFloatUniform("_MaxBoxHeight", m_MaxBoxHeight);
			m_CubeMountain->m_material->SetFloatUniform("_SideCubeNum", static_cast<float>(m_SideCubeNum));
			m_CubeMountain->m_material->SetFloatUniform("_Brightness", m_Brightness);
			}, GL_TRIANGLES, true, m_CubeNum);
	}

	void CBoxInstancing::UpdateTimeline(float SceneTime)
	{
	}
}