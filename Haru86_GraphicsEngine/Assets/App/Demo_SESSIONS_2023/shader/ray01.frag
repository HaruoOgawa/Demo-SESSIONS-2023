//#version 330

// Veda --> OpenGL/GLSL
#define DRAW_ON_VEDA

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

float cp;

///////////////////////////////////////////////
float map(vec3 pos)
{
  cp = 3.0;
  float d0=1000.0,d1=1000.0,d2=1000.0,k=7.5;
  {
    d2 = cube(pos,vec3(0.5,0.5,5.0));
  }

  pos.z -= _time;
  pos = mod(pos,k)-k*.5;

  {
      vec3 p = pos;
      //

      if(p.x<p.y)p.xy = p.yx;
      if(p.x<p.z)p.xz = p.zx;
      if(p.y<p.z)p.yz = p.zy;

      for(int i = 0; i < 8; i++)
      {
        p = abs(p) -0.5;
        p.xy *= rot(1.5);
        p.x *= 1.25;
        p.yz *= rot(0.5);

        if(p.x<p.y)p.xy = p.yx;
        if(p.x<p.z)p.xz = p.zx;
        if(p.y<p.z)p.yz = p.zy;

        p.xz*=rot(0.5);
      }
      d0 = cube(p,vec3(0.5));
  }

  {
    vec3 p = pos;
    d1 = cube(p,vec3(3.0));
  }

  return max(-d2, max(d0,d1));
  // return min(d2, max(d0,d1));
  // return d2;
}

///////////////////////////////////////////////

#define lvec normalize(vec3(1.0,-1.0,-1.0))

vec3 gn(vec3 p)
{
  vec2 e = vec2(0.001,0.0);
  return normalize(vec3(
      map(p+e.xyy)-map(p-e.xyy),
      map(p+e.yxy)-map(p-e.yxy),
      map(p+e.yyx)-map(p-e.yyx)
  ));
}

vec3 dColor(vec3 ro, vec3 rd, float i,float t,float d)
{
  vec3 col = vec3(0.0),p=ro+rd*t;
  if(d<0.001 && t < 1000.0)
  {
    vec3 n = gn(p);
    // col=vec3(1.0) * max(0.0,dot(abs(n),lvec));
    // col += vec3(exp(-0.1*t));
    col = vec3(1.0) * 5.0/i;
  }
  return col;
}
//////////////////////////////////////////////
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
    float d,t,l=0.0,r=10.0;
    vec3 col=vec3(0.0),ro=vec3(0.05*r*cos(_time),0.05*r*sin(_time),r),ta=vec3(0.0),cdir=normalize(ta-ro),cside=normalize(cross(vec3(0.0,1.0,0.0),cdir)),cup=normalize(cross(cdir,cside)),rd=normalize(st.x*cside+st.y*cup+1.0*cdir);

    for(float i=0.0;i<128.0;i++)
    {
      d=map(ro+rd*t);
      if(d<0.001 || t>1000.0){l=i; break;}
      t+=max(0.0001,d*0.5);
    }

    col = dColor(ro, rd, l, t, d);
    gl_FragColor = vec4(col,1.0);
  }
}
