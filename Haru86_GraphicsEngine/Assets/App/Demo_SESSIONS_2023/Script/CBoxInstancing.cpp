#include "CBoxInstancing.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"

CBoxInstancing::CBoxInstancing():
	m_SideCubeNum(512),
	m_CubeNum(512 * 512),
	m_CubeWidth(1.0f),
	m_CubeThreads(512, 1, 1),
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
		std::string(
			#include "Assets/App/Demo_SESSIONS_2023/Shader/InstancedBox.vert"
		),
		std::string(
			#include "Assets/App/Demo_SESSIONS_2023/Shader/InstancedBox.frag"
		)
	);
	
	m_GPGPU = std::make_shared<MeshRendererComponent>(
		std::make_shared<TransformComponent>(),
		PrimitiveType::POINT,
		RenderingSurfaceType::RASTERIZER,
		"","","","","",
		std::string(
			#include "Assets/App/Demo_SESSIONS_2023/Shader/InstancedBox.comp"
		)
	);

	m_cubeGroundBuffer = std::make_shared<ComputeBuffer>(sizeof(SFieldData) * m_CubeNum);
	
	std::vector<SFieldData> init_FieldData;
	for (int y = 0; y < m_SideCubeNum; y++) {
		for (int x = 0; x < m_SideCubeNum; x++) {
			glm::vec3 Pos = glm::vec3(
				float(x - m_SideCubeNum * 0.5f) * m_CubeWidth,
				0.0f,
				float(y - m_SideCubeNum * 0.5f) * m_CubeWidth
			);
			SFieldData Data = {
				{Pos.x,Pos.y,Pos.z,1.0f},
				{0.0f,0.0f,0.0f,0.0f},
				{m_CubeWidth,m_CubeWidth,m_CubeWidth,rand(100.0f * glm::vec2(float(x), float(y)))},
				{0.0f,0.0f,0.0f,0.0f}
			};
			init_FieldData.push_back(Data);
		}
	}

	m_cubeGroundBuffer->SetData<std::vector<SFieldData>>(init_FieldData);

	//set buffer
	m_GPGPU->m_material->SetBufferToCS(m_cubeGroundBuffer, 0);
	m_CubeMountain->m_material->SetBufferToMat(m_cubeGroundBuffer, 0);
}

void CBoxInstancing::Update(float SceneTime) {
	//
	auto& mat = m_GPGPU->m_material;
	mat->SetActive();
	mat->SetFloatUniform("_time", SceneTime);
	mat->Dispatch(m_CubeNum / m_CubeThreads.x, 1, 1);
}

void CBoxInstancing::Draw() {
	m_CubeMountain->Draw([&]() {
		m_CubeMountain->m_material->SetFloatUniform("_Roughness", 0.5);
		m_CubeMountain->m_material->SetFloatUniform("_FresnelReflectance", 1.0);
	}, GL_TRIANGLES, true, m_CubeNum);
}

void CBoxInstancing::UpdateTimeline(float SceneTime)
{
}
