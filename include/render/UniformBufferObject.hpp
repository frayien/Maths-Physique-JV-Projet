#ifndef FNES_UNIFORMBUFFEROBJECT
#define FNES_UNIFORMBUFFEROBJECT

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

struct UniformBufferObject
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

#endif // FNES_UNIFORMBUFFEROBJECT
