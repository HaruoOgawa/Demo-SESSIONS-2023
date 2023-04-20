#pragma once
#include <memory>

namespace app
{
    class CBoxInstancing;
    class CTrailObject;

    class Demo_SESSIONS_2023
    {
        unsigned int m_SceneIndex;
        float        m_SceneStartTime;
        float        m_SceneEndTime;

        std::shared_ptr<CBoxInstancing> m_BoxInstancing;
        std::shared_ptr<CTrailObject> m_TrailObject;

        bool m_DrawTrail;
    public:
        Demo_SESSIONS_2023();
        ~Demo_SESSIONS_2023() = default;

        void Start();
        void Update();
        void Draw();
        void UpdateTimeline(float SceneTime);

        unsigned int GetSceneIndex()const { return m_SceneIndex; }
        float        GetSceneStartTime() const { return m_SceneStartTime; }
        float        GetSceneEndTime() const { return m_SceneEndTime; }
    };
}

