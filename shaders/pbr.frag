#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPosWorld;
layout (location = 2) in vec3 fragNormalWorld;
layout(location = 3) in vec2 fragUV;

layout (location = 0) out vec4 outColor;

struct PointLight {
  vec4 position; // ignore w
  vec4 color; // w is intensity
};

struct DirectionalLight {
  vec4 direction; // ignore w
  vec4 color; // w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projection;
  mat4 view;
  mat4 invView;
  vec4 ambientLightColor; // w is intensity
  DirectionalLight directionalLights[10];
  PointLight pointLights[10];
  int numDirectionalLights;
  int numPointLights;
} ubo;

layout(set = 1, binding = 0) uniform sampler2D albedoTexture;
layout(set = 1, binding = 1) uniform sampler2D normalTexture;
layout(set = 1, binding = 2) uniform sampler2D metallicTexture;
layout(set = 1, binding = 3) uniform sampler2D roughnessTexture;
//layout(set = 1, binding = 4) uniform sampler2D aoTexture;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

const float PI = 3.14159265359;
//const vec3 albedo = vec3(1, 0, 0);
//const float metallic = 1;
//const float roughness = 0.4;
//const float ao = 1;

//---------------------------------------------------

vec3 getNormalFromMap()
{
  vec3 tangentNormal = texture(normalTexture, fragUV).xyz * 2.0 - 1.0;

  vec3 Q1  = dFdx(fragPosWorld);
  vec3 Q2  = dFdy(fragPosWorld);
  vec2 st1 = dFdx(fragUV);
  vec2 st2 = dFdy(fragUV);

  vec3 N   = normalize(fragNormalWorld);
  vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
  vec3 B  = -normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);

  return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
  float a = roughness*roughness;
  float a2 = a*a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH*NdotH;

  float nom   = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
  float r = (roughness + 1.0);
  float k = (r*r) / 8.0;

  float nom   = NdotV;
  float denom = NdotV * (1.0 - k) + k;

  return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2 = GeometrySchlickGGX(NdotV, roughness);
  float ggx1 = GeometrySchlickGGX(NdotL, roughness);

  return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {
//  vec3 diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
//  vec3 specularLight = vec3(0.0);
  vec3 albedo     = texture(albedoTexture, fragUV).rgb;
  float metallic  = texture(metallicTexture, fragUV).r;
  float roughness = texture(roughnessTexture, fragUV).r;
  float ao        = 1;

//  vec3 N = normalize(fragNormalWorld);
  vec3 N = getNormalFromMap();
  vec3 cameraPosWorld = ubo.invView[3].xyz;
  vec3 V = normalize(cameraPosWorld - fragPosWorld);

  vec3 F0 = vec3(0.04);
  F0 = mix(F0, albedo, metallic);

  // reflectance equation
  vec3 Lo = vec3(0.0);

  for (int i = 0; i < ubo.numPointLights; i++) {
    PointLight light = ubo.pointLights[i];

    // calculate per-light radiance
    vec3 L = normalize(light.position.xyz - fragPosWorld);
    vec3 H = normalize(V + L);
    float distance = length(light.position.xyz - fragPosWorld);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light.color.xyz * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;

    //

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
  }


  vec3 ambient = vec3(0.03) * albedo * ao;

  vec3 color = ambient + Lo;

  // HDR tonemapping
  color = color / (color + vec3(1.0));
  // gamma correct
  color = pow(color, vec3(1.0/2.2));

  outColor = vec4(color, 1.0);
}
