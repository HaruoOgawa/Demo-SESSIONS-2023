/* File generated with Shader Minifier 1.3.1
 * http://www.ctrl-alt-test.fr
 */

// E:\CppDev\Demo-SESSIONS-2023\Haru86_GraphicsEngine\GraphicsEngine\Graphics\Shader\BloomIlluminance_Frag.h
"#version 410\nprecision highp float;"
 "in vec2 uv;"
 "in float time;"
 "in vec2 resolution;"
 "uniform sampler2D _SrcTexture;"
 "uniform float _frameResolusion,_BloomThreshold,_BloomIntensity;"
 "void main()"
 "{"
   "vec3 i=vec3(0.);"
   "i=texture(_SrcTexture,uv*_frameResolusion).xyz;"
   "i.xyz=max(i.xyz-_BloomThreshold,0.)*_BloomIntensity;"
   "gl_FragColor=vec4(i,1.);"
 "}",

