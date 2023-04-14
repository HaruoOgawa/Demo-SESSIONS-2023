#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class MeshRendererComponent;
class Material;
class ComputeBuffer;

namespace myapp
{
	class FlowerModel;
	class Stem;

	// ��(�Ԃт�̏W��)
	struct Multi_Flower_Data {
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<unsigned short> triangles;
	};

	// �Ԃт�
	struct BaseFlower_Data
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<unsigned short> triangles;
	};

	struct B_Spline_Data {
		glm::vec3 position;
		int index;

		B_Spline_Data(glm::vec3 p, int i) {
			this->position = p;
			this->index = i;
		}
	};

	class Flower
	{
		// ��b�p�����[�^�[
		int kernel_CalFlowerGrowth;
		FlowerModel* m_FlowerModel;
		float isFloweringTime;

		// �o�b�t�@�֘A
		std::shared_ptr<ComputeBuffer> stemDataFlower_buffer;
		int							   stemDataFlower_buffer_index;
		std::shared_ptr<ComputeBuffer> DEBUG_buffer;
		int							   DEBUG_buffer_index;

		std::shared_ptr<Material>	   cal_flower_cs;

		
	public:
		Flower(FlowerModel* model);
		~Flower() = default;
		void Start();
		void InitializeDispatch();
		void LinkBufferToResources(const std::shared_ptr<Stem>& stem);
		void Update();
		void Draw();

		// �I�u�W�F�N�g
		std::shared_ptr<MeshRendererComponent> m_FlowerRenderer;

	private:
		// ������
		void Init();
		void InitBuffer();
		void SetupFlowerdata();

		// �Ԃ̍\�z
		static std::shared_ptr<Multi_Flower_Data> RenderMultiFlower(const std::shared_ptr<BaseFlower_Data>& flower_data, glm::vec3 flowerPosition,
			glm::vec3 flowerTangent, glm::vec3 flowerBioNormal, float flowerTime = 1.0f, int N = 50);

		static void CalFibonacciPosition(std::vector<glm::vec3>& FibonacciPosition, std::vector<glm::quat>& FibonacciRotation, std::vector<glm::vec4>& FibonacciGrowthData, int N = 50);

		// �Ԃт�֘A
		static std::shared_ptr<BaseFlower_Data> Cal_BSpline_Surface(std::vector<glm::vec3> controlPoints, float knotMin, float knotMax, float tWidth = 0.01f);
		static std::vector<std::shared_ptr<B_Spline_Data>> Cal_BSplineCurve(std::vector<glm::vec3> controlPoints, float knotMin, float knotMax, float tWidth = 0.01f);
		static std::vector<float> GetKnotVector(int m, int n, float knotMin, float knotMax);
		static float GetBasisFunction(std::vector<float> u, int j, int k, float t);
		
		// �X�V����
		void Cal_flower_growth();
	};

}
