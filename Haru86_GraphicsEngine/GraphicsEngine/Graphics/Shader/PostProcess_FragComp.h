/* File generated with Shader Minifier 1.3.1
 * http://www.ctrl-alt-test.fr
 */

// E:\CppDev\Demo-SESSIONS-2023\Haru86_GraphicsEngine\GraphicsEngine\Graphics\Shader\PostProcess_Frag.h
"#version 410\n"
 "in vec2 uv;"
 "uniform float _time,_deltaTime;"
 "uniform vec2 _resolution;"
 "uniform float _frameResolusion;"
 "uniform sampler2D _SrcTexture,_DepthMap;"
 "uniform float _Offset,_Power;"
 "void main()"
 "{"
   "vec2 f=uv*_frameResolusion;"
   "vec3 u=texture(_SrcTexture,f).xyz;"
   "float v=texture(_DepthMap,f).x,t=6.2831853,r=16.,n=3.,s=8.*v*_Power+_Offset;"
   "vec2 m=s/_resolution;"
   "for(float c=0.;c<t;c+=t/r)"
     "for(float g=1./n;g<=1.;g+=1./n)"
       "u+=texture(_SrcTexture,f+vec2(cos(c),sin(c))*m*g).xyz;"
   "u/=n*r-15.;"
   "gl_FragColor=vec4(u,1.);"
 "}",

