#ifndef MPJVP_UNIFORMBUFFEROBJECTCAMERA
#define MPJVP_UNIFORMBUFFEROBJECTCAMERA

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

struct UniformBufferObjectCamera
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

#endif // MPJVP_UNIFORMBUFFEROBJECTCAMERA
