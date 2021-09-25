#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragLightPos;
layout(location = 4) in vec3 fragLightColor;
layout(location = 5) in float fragAmbientLightStrength;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 ambient = fragAmbientLightStrength * fragLightColor;

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(fragLightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * fragLightColor;

    vec3 result = (ambient + diffuse) * fragColor;

    outColor = vec4(result, 1.0);
}