#pragma once
#include <memory>
#include <functional>

class Texture;
class CBloom;
class MeshRendererComponent;

class PostProcess
{
	//bloom
	std::unique_ptr<CBloom> m_Bloom;
	std::shared_ptr<Texture> m_BloomTexture;
	unsigned int m_BloomFrameBuffer;

	
public:
	static void CreateInstance();
	static PostProcess* GetInstance();
	static void DestroyInstance();
	
	PostProcess();
	virtual ~PostProcess()=default;
	void DrawPostProcess(const std::shared_ptr<Texture>& SrcTexture, const unsigned int& DestBuffer)const;
	
	std::shared_ptr<MeshRendererComponent> m_MeshRenderer;
	std::function<void(void)> m_PostProcesCallBack;

	//Bloom
	float m_BloomIntensity;
	float m_BloomThreshold;

	// DoF
	float m_DoFOffset;
	float m_DoFPower;

protected:
	static PostProcess* instance;
};

