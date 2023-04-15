#include "CBoxInstancing.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"

CBoxInstancing::CBoxInstancing():
	m_InstanceNum(128*128),
	m_SideCubeCount(128),
	m_CubeMountain(nullptr),
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
	
	m_cubeGroundBuffer = std::make_shared<ComputeBuffer>(sizeof(SCubeFieldObj)*m_InstanceNum);
	std::vector<SCubeFieldObj> init_CubeFieldObj;
	for (int y = 0; y < m_SideCubeCount;y++) {
		for (int x = 0; x < m_SideCubeCount; x++) {
			init_CubeFieldObj.push_back(SCubeFieldObj(x, y, x + y * m_SideCubeCount));
		}
	}

	m_cubeGroundBuffer->SetData<std::vector<SCubeFieldObj>>(init_CubeFieldObj);

	//set buffer to material
	m_CubeMountain->m_material->SetBufferToMat(m_cubeGroundBuffer, 0);
}

void CBoxInstancing::Update() {

}

void CBoxInstancing::Draw() {
	m_CubeMountain->Draw([&]() {
		m_CubeMountain->m_material->SetIntUniform("_instancedCound", m_InstanceNum);
		m_CubeMountain->m_material->SetIntUniform("_sideCubeCount", m_SideCubeCount);
		m_CubeMountain->m_material->SetFloatUniform("_cubeWidth", 1.0);
		m_CubeMountain->m_material->SetVec3Uniform("_lightDir", glm::vec3(-2.0, 1.0, 1.0));
		m_CubeMountain->m_material->SetFloatUniform("_Roughness", 0.5);
		m_CubeMountain->m_material->SetFloatUniform("_FresnelReflectance", 1.0);
	}, GL_TRIANGLES, true, m_InstanceNum);
}

void CBoxInstancing::UpdateTimeline(float SceneTime)
{
}
