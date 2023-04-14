#pragma once
#include <string>

namespace shaderlib {
	const std::string Standard_vert = std::string({
		#include "GraphicsEngine/Graphics/Shader/Standard_VertComp.h"
	});

	const std::string Standard_frag = std::string({
		#include "GraphicsEngine/Graphics/Shader/Standard_FragComp.h"
	});

	const std::string StandardRenderBoard_vert = std::string({
		#include "GraphicsEngine/Graphics/Shader/StandardRenderBoard_VertComp.h"
	});

	const std::string LatePostProcess_frag = std::string({
		#include "GraphicsEngine/Graphics/Shader/LatePostProcess_FragComp.h"
	});

	const std::string BloomBlur_frag = {
		#include "GraphicsEngine/Graphics/Shader/BloomBlur.frag"
	};

	const std::string BloomDemo_frag = {
		#include "GraphicsEngine/Graphics/Shader/BloomDemo.frag"
	};

	const std::string BloomIlluminance_frag = {
		#include "GraphicsEngine/Graphics/Shader/BloomIlluminance.frag"
	};

	const std::string BloomResult_frag = {
		#include "GraphicsEngine/Graphics/Shader/BloomResult.frag"
	};

}