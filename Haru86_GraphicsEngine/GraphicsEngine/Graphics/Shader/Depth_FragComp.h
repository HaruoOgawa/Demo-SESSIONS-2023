/* File generated with Shader Minifier 1.3.1
 * http://www.ctrl-alt-test.fr
 */

// E:\CppDev\Demo-SESSIONS-2023\Haru86_GraphicsEngine\GraphicsEngine\Graphics\Shader\Depth_Frag.h
"#version 430\n"
 "#extension GL_ARB_separate_shader_objects:enable\n"
 "layout(location=0) in vec2 in_uv;"
 "layout(location=1) in vec4 in_WorldVertexPos;"
 "layout(location=2) in vec4 in_WorldNormal;"
 "layout(location=3) flat in int in_gl_InstanceID;"
 "layout(location=4) in vec4 in_Color;"
 "out vec4 FragColor;"
 "float i=.1,l=100.;"
 "float v(float v)"
 "{"
   "float n=v*2.-1.;"
   "return 2.*i*l/(l+i-n*(l-i));"
 "}"
 "void main()"
 "{"
   "float i=v(gl_FragCoord.z)/l;"
   "FragColor=vec4(vec3(i),1.);"
 "}",

