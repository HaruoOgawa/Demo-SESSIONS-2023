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
        m_BoxInstancing(nullptr),
        m_TrailObject(nullptr)
    {
    }

    void Demo_SESSIONS_2023::Start()
    {
        m_BoxInstancing = std::make_shared<CBoxInstancing>();
        m_TrailObject = std::make_shared<CTrailObject>();

#ifdef _DEBUG
        //GraphicsMain::GetInstance()->m_ShowDebugLog = true;

        // ���Ԃ̃I�t�Z�b�g
        //GraphicsMain::GetInstance()->m_SecondsTimeOffset = 0.0f;// �V�[�����΂����߂̃I�t�Z�b�g

        // ���y�̃~���[�g
        //GraphicsMain::GetInstance()->m_SoundPlayer->Mute(true);

        // �f�o�b�O�p
        /*{
            m_DebugTimeLock = true;
            if (m_DebugTimeLock)m_LocalTime = GraphicsMain::GetInstance()->m_SecondsTimeOffset;
        }*/
#endif // _DEBUG

    }

    void Demo_SESSIONS_2023::Update()
    {
        PostProcess::GetInstance()->m_UseBloom = true;
        PostProcess::GetInstance()->m_BloomIntensity = 2.0f;
        PostProcess::GetInstance()->m_BloomThreshold = 0.5f;

        GraphicsMain::GetInstance()->m_DirectionalLightDir = glm::normalize(glm::vec3(1.0f, 0.5f, 1.0f));

        // Camera
        float time = GraphicsMain::GetInstance()->m_SecondsTime * 0.1f, r = 60.0f;
        //time = 0.0f;
        GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(r * glm::cos(time), r, r * glm::sin(time));

        // Obj
        m_BoxInstancing->Update(time);
        m_TrailObject->Update();

        //
#ifndef _DEBUG
        if (GraphicsMain::GetInstance()->m_SecondsTime >= 60.0f)
        {
            GraphicsMain::GetInstance()->isRunning = false;
            return;
        }
#endif // !_DEBUG
    }

    void Demo_SESSIONS_2023::Draw()
    {
        m_BoxInstancing->Draw();
        m_TrailObject->Draw();
    }

    void Demo_SESSIONS_2023::UpdateTimeline(float SceneTime)
    {
        m_BoxInstancing->UpdateTimeline(SceneTime);
    }
}