#include "Demo_SESSIONS_2023.h"

#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Sound/SoundPlayer.h"

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

namespace app
{
    Demo_SESSIONS_2023::Demo_SESSIONS_2023() :
        m_SceneIndex(0),
        m_SceneStartTime(0.0f),
        m_SceneEndTime(0.0f),
        m_LocalTime(0.0f)
    {
    }

    void Demo_SESSIONS_2023::Start()
    {
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
    }

    void Demo_SESSIONS_2023::UpdateTimeline()
    {
    }
}