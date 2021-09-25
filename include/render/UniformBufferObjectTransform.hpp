#ifndef MPJVP_UNIFORMBUFFEROBJECTTRANSFORM
#define MPJVP_UNIFORMBUFFEROBJECTTRANSFORM

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

struct UniformBufferObjectTransform
{
    alignas(256) glm::mat4 transform;
};

#endif // MPJVP_UNIFORMBUFFEROBJECTTRANSFORM
