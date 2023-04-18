R"(
#version 430

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform float _MaxBoxHeight;
uniform float _SideCubeNum;

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;

struct SBoxData
{
    vec4 Pos;
    vec4 Rot;
    vec4 Scl;
    vec4 Data;
};

layout(std430,binding=3) buffer destBoxDataBuffer
{
	SBoxData data[];
} outBoxDataBuffer;

out vec3 worldPosition;
out vec3 worldNormal;
out float id;
out vec3 randColor;
out vec2 uv;

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(){
	SBoxData data = outBoxDataBuffer.data[gl_InstanceID];

	mat4x4 ModelMatrix= mat4x4(
		vec4(data.Scl.x,0.0,0.0,0.0),
		vec4(0.0,data.Scl.y,0.0,0.0),
		vec4(0.0,0.0,data.Scl.z,0.0),
		vec4(0.0,0.0,0.0,1.0)
	);

	vec3 pos=(ModelMatrix*vec4(vertex,1.0)).xyz;
	pos+=data.Pos.xyz;

	gl_Position=PMatrix*VMatrix*vec4(pos,1.0);
	worldPosition=pos;
	worldNormal=normalize((ModelMatrix*vec4(normal,1.0)).xyz);

	id=float(gl_InstanceID);

	float rate = data.Scl.y/_MaxBoxHeight;
	float l = 1.0 - clamp(length(pos.xz) / (_SideCubeNum * 0.5), 0.0, 1.0);
	randColor=hsv2rgb(vec3(mod(rate + _time*0.1, 1.0), 1.0 - rate, l)) * 2.0;

	uv=texcoord;
}

)"