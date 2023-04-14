#include "LTreeModel.h"
#include "LTree.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

namespace myapp {
	LTreeModel::LTreeModel() :
		m_TreeRenderer(nullptr),
		IsStreetLineTree(false)
	{
		Start();
	}

	void LTreeModel::Start()
	{
		// �؂̃��f���𐶐�
		{
			//
			std::vector<std::vector<float>> VertexData; std::vector<int> Dimention; std::vector<unsigned short> Indices;
			LTree Tree;
			Tree.Generate(VertexData, Dimention, Indices);

			//
			m_TreeRenderer = std::make_shared<MeshRendererComponent>(
				std::make_shared<TransformComponent>(),
				RenderingSurfaceType::RASTERIZER,
				VertexData, Dimention, Indices,
				std::string(
					#include "../../Shader/Forest/LTree.vert"
				),
				std::string(
					#include "../../Shader/Forest/LTree.frag"
				),
				std::string(
					#include "../../Shader/Forest/LTree.geom"
				)
			);

			m_TreeRenderer->useAlphaTest = true;
		}
	}

	void LTreeModel::Update()
	{
	}

	void LTreeModel::Draw(int numOfTree, int LinearInstanceRate, bool UseFade)
	{
		m_TreeRenderer->Draw([&]() {
			m_TreeRenderer->m_material->SetFloatUniform("_TreeMaxRadius", 0.05f);
			m_TreeRenderer->m_material->SetIntUniform("_TreeSegment", 3);

			m_TreeRenderer->m_material->SetIntUniform("_IsStreetLine", (IsStreetLineTree) ? 1 : 0 );
			m_TreeRenderer->m_material->SetVec3Uniform("_ZCenterVec", glm::vec3(2.0f * glm::cos(-3.14f / 2.0), 2.0f, 2.0f * glm::sin(-3.14f / 2.0)));
			m_TreeRenderer->m_material->SetVec3Uniform("XSideWarkVec", glm::vec3(0.0f)); // �ЂƂ܂��K���Ȓl��n���Ă���
			m_TreeRenderer->m_material->SetFloatUniform("StreetRadius", 2.5f);
			m_TreeRenderer->m_material->SetFloatUniform("ToSideWarkDist", 1.5f);
			m_TreeRenderer->m_material->SetIntUniform("_UseFade", (UseFade) ? 1 : 0);
			m_TreeRenderer->m_material->SetIntUniform("_LinearInstanceRate", LinearInstanceRate);
		}, GL_LINES, true, numOfTree);
	}
}