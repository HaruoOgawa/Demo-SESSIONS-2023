#pragma once
#include <memory>
#include <functional>

class Texture;
class MeshRendererComponent;

class PostProcess
{
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

protected:
	static PostProcess* instance;
};

