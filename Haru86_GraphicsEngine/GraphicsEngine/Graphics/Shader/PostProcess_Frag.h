#version 410

in vec2 uv;

uniform float _time;
uniform float _deltaTime;
uniform vec2 _resolution;
uniform float _frameResolusion;
uniform sampler2D _SrcTexture;
uniform sampler2D _DepthMap;

void main(){
	vec2 st = uv * _frameResolusion;
	vec3 col=texture(_SrcTexture,st).rgb;
	// vec3 col=texture(_DepthMap,st).rgb;

	gl_FragColor=vec4(col,1.0);
}

