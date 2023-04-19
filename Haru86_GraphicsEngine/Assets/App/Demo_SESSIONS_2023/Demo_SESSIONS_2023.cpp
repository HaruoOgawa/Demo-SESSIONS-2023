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
        //GraphicsMain::GetInstance()->m_ShowDebugLog = true;

        // 時間のオフセット
        //GraphicsMain::GetInstance()->m_SecondsTimeOffset = 32.0f;// シーンを飛ばすためのオフセット

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
        auto& Camera = GraphicsMain::GetInstance()->m_MainCamera->m_position;
        auto& Center = GraphicsMain::GetInstance()->m_MainCamera->m_center;

        if (SceneTime >= 0.0f && SceneTime < 22.0f)
        {
            float w = 3.333f;
            float lTime = glm::mod(SceneTime, w * 5.0f);
            float CamID = glm::floor(lTime / w);
            float lrate = glm::clamp((lTime - CamID * w) / w, 0.0f, 1.0f);

            {
                float rate = glm::clamp((SceneTime - 0.0f) / (10.0f - 0.0f), 0.0f, 1.0f);
                PostProcess::GetInstance()->m_BloomThreshold = 0.5f + (1.0f - rate) * 0.5f;
                PostProcess::GetInstance()->m_DoFOffset = (1.0f - abs(rate * 2.0f - 1.0f)) * 10.0f;
                m_BoxInstancing->m_Brightness = rate * 2.0f + (1.0f - abs(rate * 2.0f - 1.0f)) * 3.0f;
            }

            if (CamID == 0.0f)
            {
                float rate = glm::clamp((lTime - 0.0f) / (10.0f - 0.0f), 0.0f, 1.0f);
                Camera = glm::vec3(0.0f, 15.0f, 30.0f + rate * 20.0f);
            }
            else if (CamID == 1.0f)
            {   
                Camera = glm::vec3(1.0f, 60.0f + lrate * 10.0f, 1.0f);
            }
            else if (CamID == 2.0f)
            {
                Camera = glm::vec3(60.0f * glm::cos(SceneTime * 0.1f), 60.0f, 60.0f * glm::sin(SceneTime * 0.1f));
            }
            else if (CamID == 3.0f)
            {
                Camera = glm::vec3(0.0f, 15.0f, 60.0f + lrate * 20.0f);
            }
            else if(CamID == 4.0f)
            {
                Camera = glm::vec3(100.0f * glm::cos(SceneTime * 0.05f), 100.0f, 100.0f * glm::sin(SceneTime * 0.05f));
            }
        }
        else if (SceneTime >= 22.0f && SceneTime < 59.0f)
        {
            m_DrawTrail = true;
            PostProcess::GetInstance()->m_BloomThreshold = 0.5f;
            PostProcess::GetInstance()->m_DoFOffset = 0.0f;
            m_BoxInstancing->m_Brightness = 2.0f;

            if (SceneTime >= 22.0f && SceneTime < 33.0f)
            {
                Camera = glm::vec3(60.0f * glm::cos(SceneTime * 0.1f), 60.0f, 60.0f * glm::sin(SceneTime * 0.1f));
            }
            else if (SceneTime >= 33.0f && SceneTime < 43.0f)
            {
                float rate = glm::clamp((SceneTime - 33.0f) / (43.0f - 33.0f), 0.0f, 1.0f);
                Camera = glm::vec3(90.0f - rate * 20.0f, 20.0f, 90.0f - rate * 20.0f);
            }
            else if (SceneTime >= 43.0f && SceneTime < 54.0)
            {
                Camera = glm::vec3(100.0f * glm::cos(-SceneTime * 0.05f), 100.0f, 100.0f * glm::sin(-SceneTime * 0.05f));
            }
            else if (SceneTime >= 54.0f && SceneTime < 59.0f)
            {
                float h = 15.0f, rate = glm::clamp((SceneTime - 54.0f) / (59.0f - 54.0f), 0.0f, 1.0f);
                Camera = glm::vec3(rate * 15.0f, h + 5.0f, 1.0f);
                Center = glm::vec3(rate * 15.0f, h, 0.0f);
            }
        }
        else if (SceneTime >= 59.0f && SceneTime < 66.0f)
        {
            float rate = glm::clamp((SceneTime - 65.0f) / (66.0f - 65.0f), 0.0f, 1.0f);
            PostProcess::GetInstance()->m_BloomThreshold = 0.5f - rate;
            PostProcess::GetInstance()->m_DoFPower = 1.0f;
            PostProcess::GetInstance()->m_DoFOffset = 12.0f * rate;

            Camera = glm::vec3(60.0f * glm::cos(SceneTime * 0.1f), 60.0f, 60.0f * glm::sin(SceneTime * 0.1f));
            Center = glm::vec3(0.0f);
        }
        else if (SceneTime >= 66.0f)
        {
            GraphicsMain::GetInstance()->isRunning = false;
            return;
        }
    }
}