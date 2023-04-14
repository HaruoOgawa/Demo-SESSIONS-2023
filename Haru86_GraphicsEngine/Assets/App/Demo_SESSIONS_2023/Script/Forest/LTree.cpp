#include "LTree.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Math/mymath_withGLM.h"
#include <glm/gtc/random.hpp>

namespace myapp {
	LTreeNode::LTreeNode(int inum) :
		m_LAction(""),
		m_Indent(inum),
		m_LastVerticesData(glm::vec3(0.0f)),
		m_LastParentIndices(0),
		m_NodeParent(nullptr),
		m_LastGrowDir(glm::vec3(0.0f, 1.0f, 0.0f))
	{
	}

	void LTree::Generate(std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices)
	{
		//
		m_LStep = -1;
		m_StartStructure = "";
		m_LStructure = "";
		m_LRootNode = nullptr;

		// L-System
		PrepareLSystem();
		GenerateLStructure();
		AnalyseLStructure();
		RunLSystem(VertexData, Dimention, Indices);
	}

	void LTree::PrepareLSystem() 
	{
		// ��b���
		m_LStep = 6;
		m_StartStructure = "X";
		
		// �����������[��
		m_LRule.push_back(LRule('F', "FF"));
		m_LRule.push_back(LRule('X', "F[+X]F[-X]+X"));
	}

	void LTree::GenerateLStructure()
	{
		// �������������
		m_LStructure += m_StartStructure;
		
		// �����������[���ɏ]������������
		for (int nLStep=0; nLStep<m_LStep; nLStep++)
		{
			// ���[�����`�F�b�N����
			for (const auto& Rule : m_LRule) {
				// ���̃X�e�b�v�ł̍X�V���e
				std::string nowStep_LStructure = "";

				//
				for (char& now_word : m_LStructure)
				{
					// �^�[�Q�b�g�ł���Ώ���������(nowStep_LStructure�ɂ�rResult_LWord�𑫂�)
					if (now_word == Rule.rTarget_LWord)
					{
						nowStep_LStructure += Rule.rResult_LWord;
					}
					else // ���v���Ȃ���΂��̂܂�(rTarget_LWord�𑫂�-->���Ƃ̕���)
					{
						nowStep_LStructure += now_word;
					}
				}

				// �X�V���ꂽ���e�𔽉f����
				m_LStructure = nowStep_LStructure;
			}
		}
	}

	void LTree::AnalyseLStructure()
	{
		//
		std::shared_ptr<LTreeNode> CurrentParentLNode = nullptr;
		std::shared_ptr<LTreeNode> CurrentLNode = nullptr;
		
		// ���[�g�m�[�h���쐬
		int CurrentIndent = 0;
		m_LRootNode = std::make_shared<LTreeNode>(CurrentIndent);
		CurrentLNode = m_LRootNode;

		// �ЂƂO�ɖ߂邽�߂̃y�A�����g�m�[�h��m�[�h���ꎞ�ۑ����Ă������߂̃x�N�^�[
		std::vector<std::shared_ptr<LTreeNode>> PreviousParentLNodeList;
		std::vector<std::shared_ptr<LTreeNode>> PreviousLNodeList;

		// ���肤��p�^�[��
		// ***[***[***]***]***
		// ***[***[***]***[***]***]***
		// ***[***[***[***]***]***[***[***]***[***]***]***]***
		
		//Console::Log("m_LStructure: %s\n", m_LStructure.c_str());

		//
		for (const auto& LWord : m_LStructure)
		{
			if (LWord == '[')
			{
				// ����(�q�v�f�ւ̈ړ��̖ڈ�Ƃ��ăA�X�^���X�N��z�u)
				CurrentLNode->m_LAction += '*';

				//
				if (CurrentParentLNode) PreviousParentLNodeList.push_back(CurrentParentLNode);
				PreviousLNodeList.push_back(CurrentLNode);

				//
				CurrentIndent++;
				CurrentParentLNode = CurrentLNode;
				CurrentLNode = std::make_shared<LTreeNode>(CurrentIndent);
				CurrentLNode->m_NodeParent = CurrentParentLNode;
				if (CurrentParentLNode) CurrentParentLNode->m_LNodeList.push_back(CurrentLNode);
			}
			else if (LWord == ']')
			{
				CurrentIndent--;
				
				if (PreviousParentLNodeList.size() > 0 && PreviousLNodeList.size() > 0)
				{
					//
					CurrentParentLNode = PreviousParentLNodeList[PreviousParentLNodeList.size() - 1];
					PreviousParentLNodeList.pop_back();
					PreviousParentLNodeList.shrink_to_fit();

					//
					CurrentLNode = PreviousLNodeList[PreviousLNodeList.size() - 1];
					PreviousLNodeList.pop_back();
					PreviousLNodeList.shrink_to_fit();
				}
				else // �߂����̂��Ȃ��Ƃ������Ƃ̓��[�g�m�[�h�ł���Ƃ�������
				{
					CurrentLNode = m_LRootNode;
					CurrentParentLNode = nullptr;
				}
			}
			else if (LWord == 'X')
			{
				// A,B�͖����ȕ�����Ȃ̂Ŗ�������
			}
			else
			{
				// CurrentLNode�̃A�N�V�����ɒǉ�����
				CurrentLNode->m_LAction += LWord;
			}
		}
	}

	void LTreeNode::BuildLNode(std::vector<glm::vec3>& LTree_Vertices, std::vector<glm::vec3>& LTree_Normals,
		std::vector<float>& LTreeRadiusList, std::vector<unsigned short>& LTree_Indices,const float LTreeRadius, float& LTreeLength)
	{
		// Debug /////////////////////////////
		std::string DebugStr = m_LAction;
		for (int n = -1; n < m_Indent; n++)
		{
			DebugStr = ">" + DebugStr;
		}

		//Console::Log("%d (p)%p (my)%p %s\n", m_LNodeList.size(),m_NodeParent.get(),this, DebugStr.c_str());
		/////////////////////////////////////

		// �X�^�[�g�n�_���`�e�v�f�Ƃ̌q����(Node�̒��ł�����g���܂킷)
		glm::vec3 StartPosInCNode = glm::vec3(0.0f);
		if (m_NodeParent) StartPosInCNode = m_NodeParent->m_LastVerticesData;

		// �����x�N�g��(�ǂ̕����ɐL�т邩)�B��]�ł́A���̃x�N�g�������H����
		glm::vec3 StartGrowDir = glm::vec3(0.0f, 1.0f, 0.0f);
		if (m_NodeParent) StartGrowDir = m_NodeParent->m_LastGrowDir;

		// �C���f�b�N�X�f�[�^
		unsigned short FirstIndices = 0;
		if (m_NodeParent) FirstIndices = m_NodeParent->m_LastParentIndices;
		unsigned short SecondIndices = 1;
		if(LTree_Indices.size()>0) SecondIndices = LTree_Indices[LTree_Indices.size() - 1] + 1;

		// F�̐������O�Ɍv�Z����0�ȉ��Ȃ炻�̃m�[�h�͎q�v�f�������ׂĂ��Ƃ͖�������
		int FNum = 0;
		for (const auto& LWord : m_LAction)
		{
			if (LWord == 'F') FNum++;
		}

		if (FNum <= 0)
		{
			// �q�v�f��Build
			for (auto& Node : m_LNodeList)
			{
				Node->BuildLNode(LTree_Vertices, LTree_Normals, LTreeRadiusList, LTree_Indices, LTreeRadius, LTreeLength);
			}

			return;
		}

		// ���݂̃m�[�h�̎��q�v�f�̃��X�g --> * ��������ċA�I�Ɏq�v�f�̒��ɓ����Ă���
		std::vector<std::shared_ptr<LTreeNode>> ChildNodeList;
		ChildNodeList.resize(m_LNodeList.size());
		for (int n=0;n< m_LNodeList.size();n++)
		{
			ChildNodeList.push_back(m_LNodeList[m_LNodeList.size() - 1 - n]);
		}

		// LTreeNode�����Ƃɖ؂̃��b�V�����쐬
		for (int n=0;n< m_LAction.size();n++)
		{
			const auto& LWord = m_LAction[n];

			// LWord�����Ƃ�L-System�A�N�V���������肷��
			if (LWord == 'F') // ��ɐi��Ő�������
			{
				// ���_
				StartPosInCNode += StartGrowDir * LTreeLength;
				LTree_Vertices.push_back(StartPosInCNode);

				// �@��(�����x�N�g����vec3(1.0,0.0,0.0)�Ƃ̊O��)
				LTree_Normals.push_back(glm::normalize(glm::cross(StartGrowDir, glm::vec3(1.0f, 0.0f, 0.0f))));

				// ���a�̌v�Z
				float NodeRadius = LTreeRadius;

				// �C���f���g���[���قǍׂ��Ȃ�
				NodeRadius *= glm::pow(0.65f, float(m_Indent));

				// ���a(���݂̃m�[�h�̒���m_Action�̍Ō�̕��قǍׂ��Ȃ�)
				NodeRadius *= glm::exp(-2.0f * ( float((n)) / float(m_LAction.size())) );
				
				// ���a���
				LTreeRadiusList.push_back(NodeRadius);

				// Debug
				//Console::Log("%d FirstIndices: %d / SecondIndices: %d\n", (LTree_Vertices.size() - 1), FirstIndices, SecondIndices);

				// �C���f�b�N�X�f�[�^
				LTree_Indices.push_back(FirstIndices);
				LTree_Indices.push_back(SecondIndices);
				FirstIndices= SecondIndices;
				SecondIndices++;
			}
			else if (LWord == '+') // ���v���ɉ�]
			{
				// �����w+�x�w-�x�������Ȃ̂ł���΂Ȃɂ����Ȃ�
				if (((n + 1) < m_LAction.size()) && (m_LAction[n + 1] == 'X'))
				{
					continue;
				}

				//
				float angle =  20.0f * (3.14f/180.0f);
				float rotAxisZ = glm::linearRand(0.0f, 1.0f);
				//glm::vec4 rotV = glm::mat4_cast(glm::quat(glm::vec3(0.0f, 0.0f, angle))) * glm::vec4(glm::vec3(0.0f,1.0f,0.0f), 0.0);
				glm::vec4 rotV = glm::mat4_cast(glm::quat(glm::vec3( (rotAxisZ<0.5f)? angle :0.0f, 0.0f, (rotAxisZ >= 0.5f) ? angle : 0.0f))) * glm::vec4(StartGrowDir, 0.0);
				StartGrowDir = glm::normalize(glm::vec3(rotV.x, rotV.y, rotV.z));
			}
			else if (LWord == '-') // �����v����ɉ�]
			{
				// �����w+�x�w-�x�������Ȃ̂ł���΂Ȃɂ����Ȃ�
				if (((n + 1) < m_LAction.size()) && (m_LAction[n + 1] == 'X'))
				{
					continue;
				}

				//
				float angle = - 20.0f * (3.14f / 180.0f);
				float rotAxisZ = glm::linearRand(0.0f, 1.0f);
				//glm::vec4 rotV = glm::mat4_cast(glm::quat(glm::vec3(0.0f, 0.0f, angle))) * glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), 0.0);
				glm::vec4 rotV = glm::mat4_cast(glm::quat(glm::vec3((rotAxisZ < 0.5f) ? angle : 0.0f, 0.0f, (rotAxisZ >= 0.5f) ? angle : 0.0f))) * glm::vec4(StartGrowDir, 0.0);
				StartGrowDir = glm::normalize(glm::vec3(rotV.x, rotV.y, rotV.z));
			}
			else if (LWord=='*') // �q�v�f�ֈړ�
			{
				// �Ō�̒��_�f�[�^�����݂̃m�[�h�̎q�v�f�Ƃ̌q���ڂɂ���
				m_LastVerticesData = LTree_Vertices[LTree_Vertices.size() - 1];

				// ��L�Ɠ��l�Ɍ��݂̃m�[�h�v���Z�X�̒��ł��낢��Ɖ��H���������x�N�g�����q�v�f�Ɉ����p��
				m_LastGrowDir = StartGrowDir;

				// Indices���Ō�̂��̂�Parent�̂��̂Ƃ��ēo�^���Ă���
				m_LastParentIndices = LTree_Indices[LTree_Indices.size() - 1];

				// �q�v�f��Build
				auto Child = ChildNodeList.back();
				ChildNodeList.pop_back();
				Child->BuildLNode(LTree_Vertices, LTree_Normals, LTreeRadiusList, LTree_Indices, LTreeRadius, LTreeLength);

				// �q�v�f��Indices���X�V�����̂ł�����l�����čĔz�u
				SecondIndices = LTree_Indices[LTree_Indices.size() - 1] + 1; // �����_��Indices�̍ő�l+1
			}
		}

		// �Ō�̒��_�f�[�^�����݂̃m�[�h�̎q�v�f�Ƃ̌q���ڂɂ���
		m_LastVerticesData = LTree_Vertices[LTree_Vertices.size() - 1];

		// ��L�Ɠ��l�Ɍ��݂̃m�[�h�v���Z�X�̒��ł��낢��Ɖ��H���������x�N�g�����q�v�f�Ɉ����p��
		m_LastGrowDir = StartGrowDir;

		// Indices���Ō�̂��̂�Parent�̂��̂Ƃ��ēo�^���Ă���
		m_LastParentIndices = LTree_Indices[LTree_Indices.size() - 1];
	}

	void LTree::RunLSystem(std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices)
	{
		// LStructure���璸�_�f�[�^���쐬
		std::vector<glm::vec3> LTree_Vertices;
		std::vector<glm::vec3> LTree_Normals;
		std::vector<float> LTreeRadiusList; // �i�X�k�����Ă������a
		std::vector<unsigned short> LTree_Indices;

		// ��{�p�����[�^�[(�����l)
		//float LTreeRadius = 1.0f;
		float LTreeRadius = 1.0f;
		float LTreeLength = 1.0f;
		//float LTreeLength = 1.0f*0.1f;

		// �����l��ݒ�(���_)
		LTree_Vertices.push_back(glm::vec3(0.0f));
		LTree_Normals.push_back(glm::vec3(0.0f));
		LTreeRadiusList.push_back(LTreeRadius);

		// LTreeNode�����Ƃɖ؂̃��b�V�����쐬
		m_LRootNode->BuildLNode(LTree_Vertices, LTree_Normals, LTreeRadiusList, LTree_Indices, LTreeRadius, LTreeLength);

		// ���b�V���I�u�W�F�N�g���\�z
		BuildLTreeMesh(LTree_Vertices, LTree_Normals, LTreeRadiusList, LTree_Indices, VertexData, Dimention, Indices);
	}

	void LTree::BuildLTreeMesh(std::vector<glm::vec3>& LTree_Vertices, std::vector<glm::vec3>& LTree_Normals,
		const std::vector<float>& LTree_Radius, std::vector<unsigned short>& LTree_Indices,
		std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices)
	{
		VertexData.push_back(mymath::CastVec3ToLine_float(LTree_Vertices));
		VertexData.push_back(mymath::CastVec3ToLine_float(LTree_Normals));
		VertexData.push_back(LTree_Radius);
		Dimention.push_back(3);
		Dimention.push_back(3);
		Dimention.push_back(1);
		Indices = LTree_Indices;
	}
}