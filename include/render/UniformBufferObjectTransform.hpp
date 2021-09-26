#ifndef MPJVP_UNIFORMBUFFEROBJECTTRANSFORM
#define MPJVP_UNIFORMBUFFEROBJECTTRANSFORM

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

struct UniformBufferObjectTransform
{
    // GPUs have specific alignment requirement for uniform buffers.
    // Indeal would be to read the specific requirement and work with offsets accordingly.
    // However, 256 bytes is the maximum require algnment,
    // thus, if this struct size is a multiple of 256, (ie aligned on 256) it will work with all GPUs.
    alignas(256) glm::mat4 transform;
};

#endif // MPJVP_UNIFORMBUFFEROBJECTTRANSFORM
