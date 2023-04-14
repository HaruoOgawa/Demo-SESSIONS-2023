R"(
#version 330

// Veda --> OpenGL/GLSL
//#define DRAW_ON_VEDA

#ifdef DRAW_ON_VEDA
  precision mediump float;
  uniform float time;
  uniform vec2 resolution;
  const int _RenderingTarget = 1;

  #define _time time
  #define _resolution resolution
#else
  

  uniform float _time;
  uniform vec2 _resolution;
  uniform float _RenderingTarget;
  uniform vec3 _WorldCameraPos;
  uniform vec3 _WorldCameraCenter;

  in vec2 uv;
#endif

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define cube(p,s) length(max(vec3(0.0), abs(p)-s))

float map(vec3 pos)
{
  float d0=1000.0,d1=1000.0,k=0.5,s=0.25;

  {
    vec3 p=pos;
    p.xy = abs(p.xy);
    p.xy*=rot(0.5);


    d0 = cube(p , vec3(s, s*0.25,s*0.25));
  }

  //return d0;
  return length(pos)-0.5;
}

void main(){
  if(_RenderingTarget==2) // ZTest
  {
      gl_FragColor = vec4(vec3(0.0),1.0);
  }
  else
  {
#ifdef DRAW_ON_VEDA
    vec2 st=(gl_FragCoord.xy*2.0-_resolution.xy)/min(_resolution.x,_resolution.y);
#else
    vec2 st=uv*2.0-1.0;
    st.x*=(_resolution.x/_resolution.y);
#endif
    vec3 col=vec3(0.0),ro=vec3(cos(_time),0.,sin(_time)),ta=vec3(0.0),cdir=normalize(ta-ro),cside=normalize(cross(vec3(0.0,1.0,0.0),cdir)),cup=normalize(cross(cdir,cside)),rd=normalize(st.x*cside+st.y*cup+1.0*cdir);
    float d,t,l=0.0;

    for(float i=0.0;i<64.0;i++)
    {
      d=map(ro+rd*t);
      if(d<0.001){l=i; break;}
      t+=d;
    }

    col=vec3(exp(-0.1*t)) * 2.0;

    gl_FragColor = vec4(col,1.0);
  }
}
)"