#version 110

varying vec2 v_texCoord;
//varying vec3 v_normal;
uniform mat4 u_modelMatrix;
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

void main()
{
  float material = 256.0;
  
  vec3 color = pow(texture2D(u_textureColor, v_texCoord).rgb, vec3(1.0/u_gamma));
  vec3 normal = texture2D(u_textureNormal, v_texCoord).rgb * 2.0 - 1.0;
  
  vec3 v = normalize(vec3(normal.x, normal.z, normal.y));//vec3(0.0,1.0,0.0);
  
  v = vec3(u_modelMatrix * vec4(v,0.0));
  
  color = color * (max(dot(v, u_light),0.0) * 0.75 + 0.25);
  
  
  vec3 halfAngle = normalize(normalize(-u_cameraPos) + u_light);
  vec3 specularColor = vec3(1.0,1.0,1.0);
  float specularDot = clamp(dot(normalize(v), halfAngle), 0.0, 1.0);
  float p = texture2D(u_textureSpecular, v_texCoord).r;
  vec3 specular = specularColor * pow(specularDot, material) * p;
  
  color = min(color + specular, 1.0);
  
  
  gl_FragColor = vec4(pow(color, vec3(u_gamma)), 1.0);
}
