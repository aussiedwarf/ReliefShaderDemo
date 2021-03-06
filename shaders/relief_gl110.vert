#version 110

uniform mat4 u_mvp;

attribute vec3 a_pos;
attribute vec2 a_texCoord;

//varying vec3 v_normal;
varying vec2 v_texCoord;

void main()
{
  
  gl_Position = u_mvp * vec4(a_pos, 1.0);
  //v_normal = vec3(u_modelMatrix * vec4(0.0,1.0,0.0,0.0));
  v_texCoord = a_texCoord;
}
