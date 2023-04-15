#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>

class ComputeBuffer;
class MeshRendererComponent;

namespace app
{
    struct SBoxData
    {
        float Pos[4];
        float Rot[4];
        float Scl[4];
        float Pad[4];
    };

    class CBoxInstancing
    {
        int m_CubeNum;
        int m_SideCubeNum;
        float m_CubeWidth;
        float m_InitSY;
        glm::ivec3 m_CubeThreads;

        std::shared_ptr<MeshRendererComponent> m_CubeMountain;
        std::shared_ptr<MeshRendererComponent> m_GPGPU;
        std::shared_ptr<ComputeBuffer> m_cubeGroundBuffer;
    public:
        CBoxInstancing();
        ~CBoxInstancing();
        void Start();
        void Update(float SceneTime);
        void Draw();
        void UpdateTimeline(float SceneTime);

        std::shared_ptr<ComputeBuffer> GetCubeGroundBuffer() { return m_cubeGroundBuffer; }

        float rand(glm::vec2 seeds) {
            return glm::fract(glm::sin(glm::dot(seeds, glm::vec2(12.9898f, 78.233f))) * 43758.5453f);
        }
    };
}