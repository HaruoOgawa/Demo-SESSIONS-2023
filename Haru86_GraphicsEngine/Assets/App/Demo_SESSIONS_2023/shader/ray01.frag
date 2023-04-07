// Veda --> OpenGL/GLSL
#define DRAW_ON_VEDA

#ifdef DRAW_ON_VEDA
  precision mediump float;
  uniform float time;
  uniform vec2 resolution;
  const int _RenderingTarget = 1;
#else
  #version 330

  uniform float _time;
  uniform vec2 _resolution;
  uniform float _RenderingTarget;
  uniform vec3 _WorldCameraPos;
  uniform vec3 _WorldCameraCenter;

  in vec2 uv;
#endif

float map(vec3 p)
{
  return length(p) - 0.5;
}

void main(){
  if(_RenderingTarget==2) // ZTest
  {
      gl_FragColor = vec4(vec3(0.0),1.0);
  }
  else
  {
#ifdef DRAW_ON_VEDA
    vec2 st=(gl_FragCoord.xy*2.0-resolution.xy)/min(resolution.x,resolution.y);
#else
    vec2 st=uv*2.0-1.0;
    st.x*=(_resolution.x/_resolution.y);
#endif
    vec3 col=vec3(st, 0.0);
    vec3 ro=vec3(0.,0.,1.), rd=normalize(vec3(st,-1.0));
    float d,t,l=0.0;

    for(float i=0.0;i<64.0;i++)
    {
      d=map(ro+rd*t);
      if(d<0.001){l=i; break;}
      t+=d;
    }

    col=vec3(exp(-1.0*t));

    gl_FragColor = vec4(col,1.0);
  }
}
