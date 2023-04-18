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
 "void main()"
 "{"
   "vec2 v=uv*_frameResolusion;"
   "vec3 e=texture(_SrcTexture,v).xyz;"
   "gl_FragColor=vec4(e,1.);"
 "}",

