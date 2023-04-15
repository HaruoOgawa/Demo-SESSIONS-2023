#pragma once
#include <memory>
#include <string>
#include <vector>

class ComputeBuffer;
class MeshRendererComponent;

struct SCubeFieldObj
{
public:
    int xIndex;
    int yIndex;
    int instancedIndex;
    SCubeFieldObj(int x, int y, int ii)
        :
        xIndex(x),
        yIndex(y),
        instancedIndex(ii)
    {
    }
};

class CBoxInstancing
{
    int m_InstanceNum;
    int m_SideCubeCount;

    std::shared_ptr<MeshRendererComponent> m_CubeMountain;
    std::shared_ptr<ComputeBuffer> m_cubeGroundBuffer;
public:
    CBoxInstancing();
    ~CBoxInstancing();
    void Start();
    void Update();
    void Draw();
    void UpdateTimeline(float SceneTime);
};

