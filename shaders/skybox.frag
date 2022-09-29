#version 450

layout (location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragUVW;

layout (location = 0) out vec4 outColor;

struct PointLight {
  vec4 position; // ignore w
  vec4 color; // w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projection;
  mat4 view;
  mat4 invView;
  vec4 ambientLightColor; // w is intensity
  PointLight pointLights[10];
  int numLights;
} ubo;

layout(set = 1, binding = 0) uniform samplerCube samplerCubeMap;

layout(push_constant) uniform Push {
  mat4 modelMatrix;
} push;

void main() {
  outColor = texture(samplerCubeMap, fragUVW);
}
