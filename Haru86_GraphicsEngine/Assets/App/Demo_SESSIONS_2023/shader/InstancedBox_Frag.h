#version 430

uniform vec3 _WorldCameraPos;
uniform vec3 _LightDir;

in vec3 worldPosition;
in vec3 worldNormal;
in float id;
in vec3 randColor;
in vec2 uv;

out vec4 outColor;

void main(){
    //diffuse
	vec3 L=normalize(_LightDir);
	float diff=max(0.0,dot(worldNormal,L));
	vec3 diffuseColor = diff * randColor;;

    vec3 col = diffuseColor;
    
    vec2 st=uv*2.0-1.0;
    float w = 0.05;
    col=col*step(abs(st.x),1.0-w)*step(abs(st.y),1.0-w);

	outColor=vec4(col,1.0);
}
