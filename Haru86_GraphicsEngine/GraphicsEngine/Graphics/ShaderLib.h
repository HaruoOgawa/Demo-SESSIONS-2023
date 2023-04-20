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

	const std::string PostProcess_frag = std::string({
		#include "GraphicsEngine/Graphics/Shader/PostProcess_FragComp.h"
	});

	const std::string BloomBlur_frag = {
		#include "GraphicsEngine/Graphics/Shader/BloomBlur_FragComp.h"
	};

	const std::string BloomIlluminance_frag = {
		#include "GraphicsEngine/Graphics/Shader/BloomIlluminance_FragComp.h"
	};

	const std::string BloomResult_frag = {
		#include "GraphicsEngine/Graphics/Shader/BloomResult_FragComp.h"
	};

	const std::string Depth_Frag = {
		#include "GraphicsEngine/Graphics/Shader/Depth_FragComp.h"
	};
}