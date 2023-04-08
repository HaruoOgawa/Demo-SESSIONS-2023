#include "Demo_SESSIONS_2023.h"

#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Sound/SoundPlayer.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"

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
        m_LocalTime(0.0f),
        m_TestMeshRenderer(nullptr)
    {
    }

    void Demo_SESSIONS_2023::Start()
    {
        m_TestMeshRenderer = std::make_shared<MeshRendererComponent>(
            std::make_shared<TransformComponent>(),
            PrimitiveType::BOARD,
            RenderingSurfaceType::RAYMARCHING,
            shaderlib::StandardRenderBoard_vert,
            std::string({
#include "shader/ray01.frag"
            })
        );

        PostProcess::GetInstance()->m_UseBloom = true;
        PostProcess::GetInstance()->m_BloomIntensity = 1.5f;
        PostProcess::GetInstance()->m_BloomThreshold = 0.25f;

#ifdef _DEBUG
        //GraphicsMain::GetInstance()->m_ShowDebugLog = true;
#endif // _DEBUG

    }

    void Demo_SESSIONS_2023::Update()
    {
        if (GraphicsMain::GetInstance()->m_SecondsTime >= 90.0f)
        {
            GraphicsMain::GetInstance()->isRunning = false;
            return;
        }
    }

    void Demo_SESSIONS_2023::Draw(bool IsRaymarching)
    {
        if (IsRaymarching)
        {
            m_TestMeshRenderer->Draw([&]() {
                m_TestMeshRenderer->m_material->SetIntUniform("_UseColor", 1);
                m_TestMeshRenderer->m_material->SetVec4Uniform("_Color", glm::vec4(glm::vec3(2.0f, 0.0f, 0.0f), 1.0f));
                });
        }
        else
        {
            
        }
    }

    void Demo_SESSIONS_2023::UpdateTimeline()
    {
    }
}