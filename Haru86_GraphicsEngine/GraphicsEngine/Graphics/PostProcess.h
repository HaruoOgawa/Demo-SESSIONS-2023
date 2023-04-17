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
	
	bool m_UsePostProcess;
	std::shared_ptr<MeshRendererComponent> m_MeshRenderer;
	std::function<void(void)> m_PostProcesCallBack;

	//bloom
	bool m_UseBloom;
	float m_BloomIntensity;
	float m_BloomThreshold;

protected:
	static PostProcess* instance;
};

