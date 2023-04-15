R"(
#version 430

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;
uniform float _MaxBoxHeight;

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;

struct SBoxData
{
    vec4 Pos;
    vec4 Rot;
    vec4 Scl;
    vec4 Pad;
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

vec3 hsv2rgb2(vec3 c, float k) {
    return smoothstep(0. + k, 1. - k,
        .5 + .5 * cos((vec3(c.x, c.x, c.x) + vec3(3., 2., 1.) / 3.) * radians(360.)));
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
	rate = mod(rate, 1.0);
	randColor=hsv2rgb2(vec3(rate, 1.0, 1.0), 0.6);

	uv=texcoord;
}

)"