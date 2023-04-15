R"(
#version 430

uniform vec3 _WorldCameraPos;
uniform vec3 _LightDir;
uniform float _Roughness;
uniform float _FresnelReflectance;

in vec3 worldPosition;
in vec3 worldNormal;
in float id;
in vec3 randColor;
in vec2 uv;

out vec4 outColor;

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))

float rand(vec2 seeds){
  return fract(sin(dot(seeds, vec2(12.9898,78.233))) * 43758.5453);
}

// D�iGGX�j�̍�
float D_GGX(vec3 H, vec3 N) {
    float NdotH = clamp(dot(H, N),0.0,1.0);
    float roughness = clamp(1.0-_Roughness,0.0,1.0);
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float t = ((NdotH * NdotH) * (alpha2 - 1.0) + 1.0);
    float PI = 3.1415926535897;
    return alpha2 / (PI * t * t);
}

// �t���l���̍�
float Flesnel(vec3 V, vec3 H) {
    float VdotH = clamp(dot(V, H),0.0,1.0);
    float F0 = clamp(_FresnelReflectance,0.0,1.0);
    float F = pow(1.0 - VdotH, 5.0);
    F *= (1.0 - F0);
    F += F0;
    return F;
}

// G - �􉽌����̍��i�N�b�N �g�����X���f���j
float G_CookTorrance(vec3 L, vec3 V, vec3 H, vec3 N) {
    float NdotH = clamp(dot(N, H),0.0,1.0);
    float NdotL = clamp(dot(N, L),0.0,1.0);
    float NdotV = clamp(dot(N, V),0.0,1.0);
    float VdotH = clamp(dot(V, H),0.0,1.0);

    float NH2 = 2.0 * NdotH;
    float g1 = (NH2 * NdotV) / VdotH;
    float g2 = (NH2 * NdotL) / VdotH;
    float G = min(1.0, min(g1, g2));
    return G;
}

void main(){
	

    //diffuse
	vec3 L=normalize(_LightDir);
	float diff=max(0.0,dot(worldNormal,L));
	vec3 diffuseColor = diff * randColor;;
	//diffuseColor+=0.01;

    //PBR///////////////////
    // ���[���h��ԏ�̃��C�g�ʒu�Ɩ@���Ƃ̓��ς��v�Z
    vec3 lightDirectionNormal = normalize(_LightDir);
    float NdotL = clamp(dot(worldNormal, lightDirectionNormal),0.0,1.0);

    // ���[���h��ԏ�̎��_�i�J�����j�ʒu�Ɩ@���Ƃ̓��ς��v�Z
    vec3 viewDirectionNormal = normalize((_WorldCameraPos - worldPosition).xyz);
    float NdotV = clamp(dot(worldNormal, viewDirectionNormal),0.0,1.0);

    // ���C�g�Ǝ��_�x�N�g���̃n�[�t�x�N�g�����v�Z
    vec3 halfVector = normalize(lightDirectionNormal + viewDirectionNormal);

    // D_GGX�̍�
    float D = D_GGX(halfVector,worldNormal);

    // F�̍�
    float F = Flesnel(viewDirectionNormal, halfVector);

    // G�̍�
    float G = G_CookTorrance(lightDirectionNormal, viewDirectionNormal, halfVector,worldNormal);

    // �X�y�L��������уf�B�t���[�Y���v�Z
    float specularReflection = (D * F * G) / (4.0 * NdotV * NdotL + 0.000001);
  
    vec3 pbrCol = vec3(1.0) * specularReflection;

    vec3 col = diffuseColor + pbrCol;
    
    vec2 st=uv*2.0-1.0;
    col=col*step(abs(st.x),0.9)*step(abs(st.y),0.9);

	outColor=vec4(col,1.0);
}
)"