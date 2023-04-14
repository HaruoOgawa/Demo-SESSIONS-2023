#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace myapp {

	// ����ς莎���Ɂwchar�x�ł���Ă݂���
	/*enum class LWord
	{
		LG,  // L-Syetem Go Formard --> ��ɐi��Ő�������						--> F
		LPR, // L-Syetam Positive Rotate --> +����(���v���)�։�]����				--> +
		LNR, // L-System Negative Rotate --> -����(�����v����)�։�]����			--> -
		LDS, // L-System Devide Start --> ����̃X�^�[�g							--> [
		LDE, // L-Syetem Devide End --> ����̃S�[��(�X�^�[�g�ɖ߂��ĕʂ̋K����)	--> ]
	};*/

	struct LRule // ������̏��������K��
	{
		char rTarget_LWord; // ���������Ώە�����
		std::string rResult_LWord; // ������������e
		//bool m_UseProbability; // �m���v�Z���g�����ǂ���

		LRule(char rtw, const std::string& rule) :
			rTarget_LWord(rtw),
			rResult_LWord(rule)
		{
		}
	};

	class LTreeNode // Tree�̎}�������Node�ŕ\���������
	{
	public:
		std::string m_LAction; // L-System�̋L��
		std::vector<std::shared_ptr<LTreeNode>> m_LNodeList;
		int m_Indent; // �C���f���g(�q�v�f�̓��q�̓���)
		glm::vec3 m_LastVerticesData; // �q�v�f(m_LNodeList)�Ƃ̌q���ڂ̍��W
		glm::vec3 m_LastGrowDir; // �q�v�f�ɓn�������x�N�g��
		unsigned short m_LastParentIndices; // �q�v�f�ɓn��Indeceis
		std::shared_ptr<LTreeNode> m_NodeParent;

		LTreeNode(int inum);
		void BuildLNode(std::vector<glm::vec3>& LTree_Vertices, std::vector<glm::vec3>& LTree_Normals,
			std::vector<float>& LTreeRadiusList,std::vector<unsigned short>& LTree_Indices,const float LTreeRadius, float& LTreeLength);
	};

	class LTree
	{
		int m_LStep;
		std::string m_StartStructure;
		std::vector<LRule> m_LRule;
		std::string m_LStructure;
		std::shared_ptr<LTreeNode> m_LRootNode;
	public:
		LTree()=default;
		virtual ~LTree()=default;

		void Generate(std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices);
	
	private:
		void PrepareLSystem();
		void GenerateLStructure();

		void AnalyseLStructure();
		void RunLSystem(std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices);
		void BuildLTreeMesh(std::vector<glm::vec3>& LTree_Vertices,std::vector<glm::vec3>& LTree_Normals,
			const std::vector<float>& LTreeRadiusList,std::vector<unsigned short>& LTree_Indices,
			std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices);
	};
}