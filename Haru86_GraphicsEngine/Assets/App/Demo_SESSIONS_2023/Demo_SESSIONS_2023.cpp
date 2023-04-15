#include "Demo_SESSIONS_2023.h"

#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Sound/SoundPlayer.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
//#include "./Script/Forest/Forest.h"
#include "./Script/CBoxInstancing.h"

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

#include "GraphicsEngine/Graphics/PostProcess.h"

namespace app
{
    Demo_SESSIONS_2023::Demo_SESSIONS_2023() :
        m_SceneIndex(0),
        m_SceneStartTime(0.0f),
        m_SceneEndTime(0.0f),
        m_CubeMountain(nullptr),
        m_CBoxInstancing(nullptr)
        //m_Forest(nullptr)
    {
    }

    void Demo_SESSIONS_2023::Start()
    {
        //m_Forest = std::make_shared<myapp::Forest>();
        m_CubeMountain = std::make_shared<MeshRendererComponent>(
            std::make_shared<TransformComponent>(),
            PrimitiveType::CUBE,    
            RenderingSurfaceType::RASTERIZER,
            shaderlib::Standard_vert,
            shaderlib::Standard_frag
        );

        m_CBoxInstancing = std::make_shared<CBoxInstancing>();
        
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
        GraphicsMain::GetInstance()->m_GroabalLightPosition->m_position = glm::vec3(
            0.0f,
            5.0f,
            0.0f
        );

        float time = GraphicsMain::GetInstance()->m_SecondsTime, r = 2.0f;
        GraphicsMain::GetInstance()->m_MainCamera->m_position = glm::vec3(r * glm::cos(time), r * 1.0f, r * glm::sin(time));

        //m_Forest->Update();

        if (GraphicsMain::GetInstance()->m_SecondsTime >= 30.0f)
        {
            GraphicsMain::GetInstance()->isRunning = false;
            return;
        }
    }

    void Demo_SESSIONS_2023::Draw(bool IsRaymarching)
    {
        //m_Forest->Draw(IsRaymarching, m_SceneIndex, 10, false);
        if (!IsRaymarching)
        {
            //m_CubeMountain->Draw();
            m_CBoxInstancing->Draw();
        }
    }

    void Demo_SESSIONS_2023::UpdateTimeline(float SceneTime)
    {
        //m_Forest->UpdateTimeline(SceneTime, false);
    }
}