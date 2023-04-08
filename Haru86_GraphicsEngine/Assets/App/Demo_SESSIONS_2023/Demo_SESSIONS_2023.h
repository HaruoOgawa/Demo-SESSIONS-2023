#pragma once
#include <memory>

class MeshRendererComponent;

namespace app
{
    class Demo_SESSIONS_2023
    {
        unsigned int m_SceneIndex;
        float        m_SceneStartTime;
        float        m_SceneEndTime;
        float        m_LocalTime;

        std::shared_ptr<MeshRendererComponent> m_TestMeshRenderer;

    public:
        Demo_SESSIONS_2023();
        ~Demo_SESSIONS_2023() = default;

        void Start();
        void Update();
        void Draw(bool IsRaymarching);
        void UpdateTimeline();

        unsigned int GetSceneIndex()const { return m_SceneIndex; }
        float        GetSceneStartTime() const { return m_SceneStartTime; }
        float        GetSceneEndTime() const { return m_SceneEndTime; }
    };
}

