#version 410

in vec2 uv;

uniform float _time;
uniform float _deltaTime;
uniform vec2 _resolution;
uniform float _frameResolusion;
uniform sampler2D _SrcTexture;
uniform sampler2D _DepthMap;
uniform float _Offset;
uniform float _Power;

void main(){
	//
	vec2 st = uv * _frameResolusion;
	vec3 col=texture(_SrcTexture,st).rgb;
	float depth=texture(_DepthMap,st).r;

	// Blur
	// https://www.shadertoy.com/view/Xltfzj
	float PI = 2.0 * 3.14159265;
	float Directions = 16.0;
	float Quality = 3.0;
	float Size = 8.0 * depth * _Power + _Offset;
	vec2 Radius = Size / _resolution;

	for (float d = 0.0; d < PI; d += PI / Directions)
	{
		for (float i = 1.0 / Quality; i <= 1.0; i += 1.0 / Quality)
		{
			col += texture(_SrcTexture, st + vec2(cos(d), sin(d)) * Radius * i).rgb;
		}
	}

	col /= Quality * Directions - 15.0;

	//col = vec3(depth);

	gl_FragColor=vec4(col,1.0);
}

