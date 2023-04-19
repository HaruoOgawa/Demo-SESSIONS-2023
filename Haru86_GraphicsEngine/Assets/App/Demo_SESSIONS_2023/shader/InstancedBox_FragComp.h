/* File generated with Shader Minifier 1.3.1
 * http://www.ctrl-alt-test.fr
 */

// E:\CppDev\Demo-SESSIONS-2023\Haru86_GraphicsEngine\Assets\App\Demo_SESSIONS_2023\shader\InstancedBox_Frag.h
"#version 430\n"
 "uniform vec3 _WorldCameraPos,_LightDir;"
 "in vec3 worldPosition,worldNormal;"
 "in float id;"
 "in vec3 randColor;"
 "in vec2 uv;"
 "out vec4 outColor;"
 "void main()"
 "{"
   "vec3 v=normalize(_LightDir);"
   "float r=max(0.,dot(worldNormal,v));"
   "vec3 m=r*randColor,i=m;"
   "vec2 a=uv*2.-1.;"
   "float o=.05;"
   "i=i*step(abs(a.x),1.-o)*step(abs(a.y),1.-o);"
   "outColor=vec4(i,1.);"
 "}",

