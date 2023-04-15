#pragma once
#include <memory>

class MeshRendererComponent;
class CBoxInstancing;

namespace myapp {
    class Forest;
}

namespace app
{
    class Demo_SESSIONS_2023
    {
        unsigned int m_SceneIndex;
        float        m_SceneStartTime;
        float        m_SceneEndTime;

        //std::shared_ptr<myapp::Forest> m_Forest;

        std::shared_ptr<MeshRendererComponent> m_CubeMountain;
        std::shared_ptr<CBoxInstancing> m_CBoxInstancing;
    public:
        Demo_SESSIONS_2023();
        ~Demo_SESSIONS_2023() = default;

        void Start();
        void Update();
        void Draw(bool IsRaymarching);
        void UpdateTimeline(float SceneTime);

        unsigned int GetSceneIndex()const { return m_SceneIndex; }
        float        GetSceneStartTime() const { return m_SceneStartTime; }
        float        GetSceneEndTime() const { return m_SceneEndTime; }
    };
}

