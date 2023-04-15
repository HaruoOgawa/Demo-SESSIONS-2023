#include "Demo_SESSIONS_2023.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Sound/SoundPlayer.h"
#include "./Script/CBoxInstancing.h"
#include "./Script/CTrailObject.h"
#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

namespace app
{
    Demo_SESSIONS_2023::Demo_SESSIONS_2023() :
        m_SceneIndex(0),
        m_SceneStartTime(0.0f),
        m_SceneEndTime(0.0f),
        m_BoxInstancing(nullptr),
        m_TrailObject(nullptr)
    {
    }

    void Demo_SESSIONS_2023::Start()
    {
        //m_BoxInstancing = std::make_shared<CBoxInstancing>();
        m_TrailObject = std::make_shared<CTrailObject>();
        
#ifdef _DEBUG
        //GraphicsMain::GetInstance()->m_ShowDebugLog = true;

        // 時間のオフセット
        //GraphicsMain::GetInstance()->m_SecondsTimeOffset = 0.0f;// シーンを飛ばすためのオフセット

        // 音楽のミュート
        GraphicsMain::GetInstance()->m_SoundPlayer->Mute(true);

        // デバッグ用
        /*{
            m_DebugTimeLock = true;
            if (m_DebugTimeLock)m_LocalTime = GraphicsMain::GetInstance()->m_SecondsTimeOffset;
        }*/
#endif // _DEBUG

    }

    void Demo_SESSIONS_2023::Update()
    {
        // ライトポジション
        GraphicsMain::GetInstance()->m_DirectionalLightDir = glm::normalize(glm::vec3(-2.0f, 1.0f, 1.0f));

        // Camera
        float time = GraphicsMain::GetInstance()->m_SecondsTime, r = 50.0f;
        GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(r * glm::cos(time), r * 0.5f, r * glm::sin(time));

        // Obj
        //m_BoxInstancing->Update(time);
        m_TrailObject->Update();

        //
        if (GraphicsMain::GetInstance()->m_SecondsTime >= 30.0f)
        {
            GraphicsMain::GetInstance()->isRunning = false;
            return;
        }
    }

    void Demo_SESSIONS_2023::Draw()
    {
        //m_BoxInstancing->Draw();
        m_TrailObject->Draw();
    }

    void Demo_SESSIONS_2023::UpdateTimeline(float SceneTime)
    {
        m_BoxInstancing->UpdateTimeline(SceneTime);
    }
}