#version 110

varying vec2 v_texCoord;
//varying vec3 v_normal;
uniform vec3 u_light;
uniform sampler2D u_textureColor;
uniform sampler2D u_textureNormal;

void main()
{
  vec3 color = texture2D(u_textureColor, v_texCoord).rgb;
  vec3 normal = texture2D(u_textureNormal, v_texCoord).rgb * 2.0 - 1.0;
  
  vec3 v = normalize(vec3(normal.x, normal.z, normal.y));//vec3(0.0,1.0,0.0);
  
  color = color * (max(dot(v, u_light),0.0) * 0.75 + 0.25);
  
  
  gl_FragColor = vec4(color, 1.0);
}
