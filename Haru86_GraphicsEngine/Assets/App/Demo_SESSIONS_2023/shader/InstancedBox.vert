R"(
#version 430

uniform mat4 MVPMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform float _time;
uniform float _deltaTime;

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in vec2 texcoord;

struct SFieldData
{
    vec4 Pos;
    vec4 Rot;
    vec4 Scl;
    vec4 Pad;
};

layout(std430,binding=0) buffer destFieldDataBuffer
{
	SFieldData data[];
} outFieldDataBuffer;

out vec3 worldPosition;
out vec3 worldNormal;
out float id;
out vec3 randColor;
out vec2 uv;

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

void main(){
	SFieldData data = outFieldDataBuffer.data[gl_InstanceID];

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

	randColor=vec3(
		rand(vec2(float(gl_InstanceID),0.159)),
        rand(vec2(float(gl_InstanceID),7.957)),
        rand(vec2(float(gl_InstanceID),0.681))
	);

	uv=texcoord;
}

)"