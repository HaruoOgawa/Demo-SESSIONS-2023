/* File generated with Shader Minifier 1.3.1
 * http://www.ctrl-alt-test.fr
 */

// E:\CppDev\Demo-SESSIONS-2023\Haru86_GraphicsEngine\GraphicsEngine\Graphics\Shader\BloomBlur_Frag.h
"#version 410\nprecision highp float;"
 "in vec2 uv;"
 "in float time;"
 "in vec2 resolution;"
 "uniform sampler2D _IlluminanceTexture;"
 "uniform float _frameResolusion;"
 "uniform vec2 _resolution;"
 "uniform float _weights[10],_IsHorizontal;"
 "void main()"
 "{"
   "vec3 u=vec3(0.);"
   "vec2 v=1./_resolution.xy;"
   "v*=(2.-_frameResolusion)*1.25;"
   "int f=10;"
   "if(_IsHorizontal==1.)"
     "{"
       "u+=texture(_IlluminanceTexture,uv*_frameResolusion).xyz*_weights[0];"
       "for(int n=1;n<f;n++)"
         "u+=texture(_IlluminanceTexture,uv*_frameResolusion+vec2(v.x*float(n),0.)*_frameResolusion).xyz*_weights[n],u+=texture(_IlluminanceTexture,uv*_frameResolusion-vec2(v.x*float(n),0.)*_frameResolusion).xyz*_weights[n];"
     "}"
   "else"
     " if(_IsHorizontal==0.)"
       "{"
         "u+=texture(_IlluminanceTexture,uv*_frameResolusion).xyz*_weights[0];"
         "for(int n=1;n<f;n++)"
           "u+=texture(_IlluminanceTexture,uv*_frameResolusion+vec2(0.,v.y*float(n))*_frameResolusion).xyz*_weights[n],u+=texture(_IlluminanceTexture,uv*_frameResolusion-vec2(0.,v.y*float(n))*_frameResolusion).xyz*_weights[n];"
       "}"
     "else"
       " u.xyz=texture(_IlluminanceTexture,uv*_frameResolusion).xyz;"
   "gl_FragColor=vec4(u,1.);"
 "}",

