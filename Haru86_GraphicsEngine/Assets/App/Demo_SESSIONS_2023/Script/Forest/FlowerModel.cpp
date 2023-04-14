#include "FlowerModel.h"
#include "Flower.h"
#include "Stem.h"

namespace myapp {
	FlowerModel::FlowerModel() :
		count(1024),
		m_Flower(nullptr),
		m_Stem(nullptr),
		flowersIsDone(false),
		stemIsDone(false),
		leafIsDone(false)
	{
		Start();
	}

	void FlowerModel::Start() {
		// ������
		m_Stem = std::make_shared<Stem>(this);
		stemIsDone = true;
		leafIsDone = true;
		m_Flower = std::make_shared<Flower>(this);
		flowersIsDone = true;

		// �o�b�t�@�ƃR���s���[�g�V�F�[�_�[ / �o�b�t�@�ƃ}�e���A�� �����ꂼ�ꌋ�ѕt����(BufferIndex���K�v�Ȃ��ߏ�������Ɋ��蓖�Ă�)
		m_Stem->LinkBufferToResources();
		m_Flower->LinkBufferToResources(m_Stem);

		// �����Dispatch(�������R���s���[�g�V�F�[�_�[�̌v�Z)
		// ��L�̃o�C���h��ɍs��Ȃ���Buffer���Ȃ��ĕ`�悪���������Ȃ�(�G���[�͂łȂ��������Ȃ�)
		m_Stem->InitializeDispatch();
		m_Flower->InitializeDispatch();
	}

	void FlowerModel::Update() {
		m_Stem->Update();
		m_Flower->Update();
	}

	void FlowerModel::Draw() {
		m_Stem->Draw();
		m_Flower->Draw();
	}
}