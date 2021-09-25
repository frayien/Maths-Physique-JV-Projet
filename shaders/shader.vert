#version 450

layout(binding = 0) uniform UniformBufferObjectCamera {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightPos;
    vec3 lightColor;
    float ambientLightStrength;
} uboc;

layout(binding = 1) uniform UniformBufferObjectTransform {
    mat4 transform;
} ubot;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPos;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragLightPos;
layout(location = 4) out vec3 fragLightColor;
layout(location = 5) out float fragAmbientLightStrength;

void main() {
    gl_Position = uboc.proj * uboc.view * uboc.model * ubot.transform * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragPos = vec3(uboc.model * ubot.transform * vec4(inPosition, 1.0));
    fragNormal = vec3(ubot.transform * vec4(inNormal, 0.0));
    fragLightPos = uboc.lightPos;
    fragLightColor = uboc.lightColor;
    fragAmbientLightStrength = uboc.ambientLightStrength;
}