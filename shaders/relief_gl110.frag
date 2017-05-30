#version 110

varying vec2 v_texCoord;
//varying vec3 v_normal;
uniform mat4 u_modelMatrix;
uniform mat4 u_inverseMvp;
uniform vec3 u_light;
uniform vec3 u_cameraPos;           //relative camera position
uniform float u_gamma;
uniform sampler2D u_textureColor;
uniform sampler2D u_textureNormal;
uniform sampler2D u_textureSpecular;
uniform sampler2D u_textureDepth;

mat3 cotangent_frame( vec3 N, vec3 p, vec2 uv )
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );

    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 GetSpecular(vec3 normal, vec3 camera, vec3 light, float spec)
{
  float material = 32.0;
  vec3 specularColor = vec3(1.0,1.0,0.95);
  
  vec3 halfAngle = normalize(camera + light);
  
  float specularDot = clamp(dot(normal, halfAngle), 0.0, 1.0);
  vec3 specular = specularColor * pow(specularDot, material) * spec;

  return specular;
}

/*
vec3 GetColor(vec3 normal, vec3 light)
{
  
}

vec3 GetSurfaceNormal()
{
  
}

vec2 GetCoord(vec3 pos)
{
  vec2 coord = pos
}
*/

vec3 GetPos()
{
  vec4 pos = gl_FragCoord / gl_FragCoord.w;
  return vec3(u_inverseMvp * pos);
}

vec2 Parallax(vec3 view, vec2 texCoord, float heightScale)
{
  const float numLayers = 10.0;
  float layerDepth = 1.0 / numLayers;
  float currentLayerDepth = 0.0;
  
  vec2 P = view.xy * heightScale; 
  vec2 deltaTexCoords = P / numLayers;
  
  vec2  currentTexCoords     = texCoord;
  float currentDepthMapValue = texture2D(u_textureDepth, currentTexCoords).r;
  
  while(currentLayerDepth < currentDepthMapValue)
  {
    currentTexCoords -= deltaTexCoords;
    currentDepthMapValue = texture2D(u_textureDepth, currentTexCoords).r;  
    currentLayerDepth += layerDepth;  
  }
  

  vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

  float afterDepth  = currentDepthMapValue - currentLayerDepth;
  float beforeDepth = texture2D(u_textureDepth, prevTexCoords).r - currentLayerDepth + layerDepth;
 
  float weight = afterDepth / (afterDepth - beforeDepth);
  vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

  return finalTexCoords; 
}


void main()
{
  
  float ambient = 0.125;
  
  vec3 T = vec3(1.0,0.0,0.0);
  vec3 B = vec3(0.0,0.0,1.0);
  vec3 N = vec3(0.0,1.0,0.0);
  mat3 TBN = mat3(T, B, N);
  
  //vec3 v = vec3(0.0,1.0,0.0);
  //vec3 v = normalize(vec3(normal.x, normal.z, normal.y));
  
  
  vec3 pos = GetPos();
  
  vec3 view = -normalize(u_cameraPos - pos);
  
  vec3 tangentView = TBN * view;
  
  vec2 coord = Parallax(tangentView, v_texCoord, 0.1);
  
  if(coord.x > 1.0 || coord.y > 1.0 || coord.x < 0.0 || coord.y < 0.0)
    discard;
  
  
  vec3 color = pow(texture2D(u_textureColor, coord).rgb, vec3(1.0/u_gamma));
  vec3 normal = texture2D(u_textureNormal, coord).rgb * 2.0 - 1.0;
  float spec = texture2D(u_textureSpecular, coord).r;
  
  
  vec3 v = TBN * normal;
  
  
  
  
  //v = vec3(u_modelMatrix * vec4(v,0.0));
  
  color = color * (max(dot(v, -u_light),0.0) * (1.0 - ambient) + ambient);
  
  vec3 specular = GetSpecular(v, view, -u_light, spec);
  
  
  color = min(color + specular, 1.0);
  
  vec4 final = vec4(pow(color, vec3(u_gamma)), 1.0);
  gl_FragColor = final;
}
