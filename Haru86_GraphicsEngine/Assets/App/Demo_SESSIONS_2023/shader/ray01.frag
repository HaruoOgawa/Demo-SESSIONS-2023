precision mediump float;
uniform float time;
uniform vec2 resolution;

const float pi = 3.1415;
mat2 rot(float a){return mat2(cos(a),-sin(a),sin(a),cos(a));}
float cube(vec3 p,vec3 s) {return length(max(vec3(0.0), abs(p)-s));}
float rand(vec2 p) {return fract( sin(dot(p,vec2(12.9898,78.233)))*43758.5453123 );}

float map(vec3 pos)
{
  float d0=1000.0,d1=1000.0,k=7.5,sp=0.5,ftf=pow(fract(time*sp),-0.25),ft=floor(time*sp)+ftf;

  pos.z -= time * 5.0;
  pos = mod(pos,k)-k*.5;
  pos.xy*=rot(pi/4.0 * step(0.5,fract(time * 0.5)));
  pos.yz*=rot(pi/4.0 * step(fract(time  * 0.5),0.5));

  {
      vec3 p = pos;
      p = abs(p) - 0.1;

      if(p.x<p.y)p.xy = p.yx;
      if(p.x<p.z)p.xz = p.zx;
      if(p.y<p.z)p.yz = p.zy;

      for(int i = 0; i < 7; i++)
      {
        p = abs(p) -0.5;
        p.xy *= rot(1.5);
        p.x *= 1.25;
        p.yz *= rot(ft);

        p = abs(p);

        if(p.x<p.y)p.xy = p.yx;
        if(p.x<p.z)p.xz = p.zx;
        if(p.y<p.z)p.yz = p.zy;

        p.xz*=rot(0.5 -0.25);
      }
      d0 = cube(p,vec3(0.25));
  }

  {
    vec3 p = pos;
    d1 = cube(p,vec3(3.0));
  }

  return max(d0,d1);
}

vec3 ldir = normalize(vec3(1.0,-1.0,-1.0));

vec3 gn(vec3 p)
{
  vec2 e = vec2(0.1,0.0);
  return normalize(vec3(
      map(p+e.xyy)-map(p-e.xyy),
      map(p+e.yxy)-map(p-e.yxy),
      map(p+e.yyx)-map(p-e.yyx)
  ));
}
vec3 hsv2rgb2(vec3 c, float k) {
    return smoothstep(0. + k, 1. - k,
        .5 + .5 * cos((vec3(c.x, c.x, c.x) + vec3(3., 2., 1.) / 3.) * radians(360.)));
}

vec3 dColor(vec3 ro, vec3 rd, float i,float t,float d,float acc)
{
  vec3 col = vec3(1.0),p=ro+rd*t;
  if(d<0.001 && t < 1000.0)
  {
    vec3 n = gn(p);
    float diff = max(0.0, dot(ldir, n));
    vec3 hdir = normalize(rd + ldir);
    float spec = pow(max(0.0, dot(hdir,n)), 60.0);
    col = vec3(0.1) * diff + vec3(1.0) * spec;
  }

  float flash = sin(p.z*0.1 + time * 10.0) * 0.5 + 0.5;
  col += vec3(1.0) * acc * 0.1 * hsv2rgb2(vec3(mod(time * 2.0, 1.0), 1.0,1.0), 2.2) * flash;

  vec3 fog = exp2(-0.025*t*vec3(1.0));
  col=mix(vec3(1.0),col,fog);

  return col;
}

void main(){
      vec2 st=(gl_FragCoord.xy*2.0-resolution.xy)/min(resolution.x,resolution.y);
      st*=rot(-time*0.1);
      float d,t,l=0.0,r=5.0,s=2.0,acc=0.0,zfactor=1.0-0.5*length(st),blurPower = 0.05,l_time = time + (rand(st)*blurPower-blurPower),c0=0.5,c1=1.5,c2=2.8;
      vec3 ro=vec3(0.05*r*cos(s*l_time),0.05*r*sin(s*l_time),r*sin(s*l_time)*cos(s*l_time));

      vec3 col=vec3(0.0),ta=vec3(0.0),cdir=normalize(ta-ro),cside=normalize(cross(vec3(0.0,1.0,0.0),cdir)),cup=normalize(cross(cdir,cside)),rd=normalize(st.x*cside+st.y*cup+zfactor*cdir);

      for(float i=0.0;i<256.0;i++)
      {
        d=map(ro+rd*t);
        if(d<0.001 || t>1000.0){l=i; break;}
        t+=max(0.0001,d*0.75);
        acc+=exp(-3.0*d);
      }

      col = dColor(ro, rd, l, t, d,acc);
      gl_FragColor = vec4(col,1.0);
}
