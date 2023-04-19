/* File generated with Shader Minifier 1.3.1
 * http://www.ctrl-alt-test.fr
 */

// E:\CppDev\Demo-SESSIONS-2023\Haru86_GraphicsEngine\GraphicsEngine\Graphics\Shader\BloomResult_Frag.h
"#version 410\nprecision highp float;"
 "in vec2 uv;"
 "in float time;"
 "in vec2 resolution;"
 "uniform sampler2D _BlurTexture,_SrcTexture;"
 "uniform float _frameResolusion;"
 "void main()"
 "{"
   "vec3 e=vec3(0.);"
   "e.xyz=texture(_BlurTexture,uv*_frameResolusion).xyz+texture(_SrcTexture,uv*_frameResolusion).xyz;"
   "gl_FragColor=vec4(e,1.);"
 "}",

