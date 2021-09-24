#version 450

layout(binding = 0) uniform UniformBufferObjectCamera {
    mat4 model;
    mat4 view;
    mat4 proj;
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

void main() {
    gl_Position = uboc.proj * uboc.view * uboc.model * ubot.transform * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragPos = vec3(uboc.model * ubot.transform * vec4(inPosition, 1.0));
    fragNormal = inNormal;
}