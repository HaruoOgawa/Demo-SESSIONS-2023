#include "Demo_SESSIONS_2023.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
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
        m_DrawTrail(false),
        m_BoxInstancing(nullptr),
        m_TrailObject(nullptr)
    {
    }

    void Demo_SESSIONS_2023::Start()
    {
        //
        m_BoxInstancing = std::make_shared<CBoxInstancing>();
        m_TrailObject = std::make_shared<CTrailObject>();
        
        //
        GraphicsMain::GetInstance()->m_DirectionalLightDir = glm::normalize(glm::vec3(1.0f, 0.5f, 1.0f));
        PostProcess::GetInstance()->m_BloomIntensity = 2.0f;
        PostProcess::GetInstance()->m_BloomThreshold = 0.5f;

#ifdef _DEBUG
        GraphicsMain::GetInstance()->m_ShowDebugLog = true;

        // 時間のオフセット
        //GraphicsMain::GetInstance()->m_SecondsTimeOffset = 60.0f;// シーンを飛ばすためのオフセット

        // 音楽のミュート
        //GraphicsMain::GetInstance()->m_SoundPlayer->Mute(true);

        // デバッグ用
        /*{
            m_DebugTimeLock = true;
            if (m_DebugTimeLock)m_LocalTime = GraphicsMain::GetInstance()->m_SecondsTimeOffset;
        }*/
#endif // _DEBUG

    }

    void Demo_SESSIONS_2023::Update()
    {
        m_BoxInstancing->Update();
        if(m_DrawTrail) m_TrailObject->Update();
    }

    void Demo_SESSIONS_2023::Draw()
    {
        m_BoxInstancing->Draw();
        if (m_DrawTrail) m_TrailObject->Draw();
    }

    void Demo_SESSIONS_2023::UpdateTimeline(float SceneTime)
    {
        // Camera
        float time = GraphicsMain::GetInstance()->m_SecondsTime * 0.1f, r = 60.0f;
        GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(r * glm::cos(time), r, r * glm::sin(time));
        
        if (SceneTime >= 0.0 && SceneTime < 5.0f)
        {

        }
        else if (SceneTime >= 65.0f && SceneTime < 66.0f)
        {
            float rate = (SceneTime - 65.0f) / (66.0f - 65.0f);
            PostProcess::GetInstance()->m_BloomThreshold = 0.5f - rate;
            PostProcess::GetInstance()->m_DoFPower = 1.0f;
            PostProcess::GetInstance()->m_DoFOffset = 12.0f * rate;
        }
        else if (SceneTime >= 66.0f)
        {
            GraphicsMain::GetInstance()->isRunning = false;
            return;
        }

        if (SceneTime >= 22.0f)
        {
            m_DrawTrail = true;
        }
    }
}