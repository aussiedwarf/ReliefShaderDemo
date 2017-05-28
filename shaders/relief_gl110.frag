#version 110

varying vec2 v_texCoord;

uniform sampler2D u_textureColor;

void main()
{
  gl_FragColor = texture2D(u_textureColor, v_texCoord);
}
