R"(

#version 410

layout(location=0)in vec3 vertex;
layout(location=1)in vec3 normal;
layout(location=2)in float treeRadius; // ���a�����񂾂�ׂ����邽�߂̒��_�f�[�^�B���_��Tree�̂ǂ̂ւ�ɂ��邩  

out vec3 LocalNormal;
out float v2g_treeRadius;
flat out float v2g_id;

uniform int _IsStreetLine;
uniform float _time;

uniform vec3 XSideWarkVec;
uniform float StreetRadius;
uniform float ToSideWarkDist;
uniform vec3 _ZCenterVec; // Z�͖�����ł���

#define rot(a) mat2(cos(a),sin(a),-sin(a),cos(a))
#define PI 3.14159265

// hash --> https://www.shadertoy.com/view/4dffRH
vec3 hash( vec3 p ) // replace this by something better. really. do
{
	p = vec3( dot(p,vec3(127.1,311.7, 74.7)),
			  dot(p,vec3(269.5,183.3,246.1)),
			  dot(p,vec3(113.5,271.9,124.6)));

	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float rand(vec2 st){
    return fract(
        sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123
    );
}

void main()
{
	//
	vec4 pos=vec4(vertex,1.0);
	float id = float(gl_InstanceID);

	// �����_�����[�e�[�V����
	pos.xz*=rot(rand(vec2(id+449.212))*4.0*PI-2.0*PI);

	// �����_���|�W�V����
	vec3 randPos = vec3(0.0);
	
	if(_IsStreetLine == 1) // ������ɕ��ׂ�
	{
		float randPosRadius=400.0;
		randPos=hash(vec3(id+7.22,id+id,id-88.21))*randPosRadius;
		randPos.y=1.5;

		randPos.z-=_time*10.0;
		randPos.z=mod(randPos.z,randPosRadius)-randPosRadius*0.5;

		// ��ʂ�̂Ԃ񂾂������J����
		{
			vec3 OffsetVectorZStreet = randPos-_ZCenterVec;

			if(abs(OffsetVectorZStreet.x) <= (StreetRadius + ToSideWarkDist))
			{
				vec3 StreetOffVec = exp(-0.3*abs(OffsetVectorZStreet.x))*ToSideWarkDist * StreetRadius * normalize(vec3( OffsetVectorZStreet.x ,0.0,0.0));
				randPos+=StreetOffVec;
			}
		}
	}
	else // �~��ɕ��ׂ�
	{
		randPos = 200.0 * vec3(
			rand(vec2(id,0.697))*2.0-1.0,
			0.0,
			rand(vec2(1.294+id,0.412+id))*2.0-1.0
		);
	
		/*// �����_���|�W�V���� --> �ЂƂ܂����~��ɍL���ăV�[�����떂����
		vec3 randPos = 200.0 * vec3(
			-1.0*rand(vec2(id,0.697)),
			0.0,
			rand(vec2(1.294+id,0.412+id))*2.0-1.0
		);*/

		// ���S����̃I�t�Z�b�g
		float offradius=50.0;
		vec2 offdir= offradius * normalize(vec2(randPos.x,randPos.z));
		randPos.xz+=offdir;
	}
	
	// �|�W�V�������X�V
	pos.xyz+=randPos;

	//
	gl_Position = pos;
	LocalNormal = normal;
	v2g_treeRadius = treeRadius;
	v2g_id = id;
}

)"