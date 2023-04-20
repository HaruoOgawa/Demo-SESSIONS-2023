#version 430
#extension GL_ARB_separate_shader_objects : enable

layout(location=0) in vec2 in_uv;
layout(location=1) in vec4 in_WorldVertexPos;
layout(location=2) in vec4 in_WorldNormal;
layout(location=3) flat in int  in_gl_InstanceID;
layout(location = 4) in vec4 in_Color;

out vec4 FragColor;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{             
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    FragColor = vec4(vec3(depth), 1.0);
}
