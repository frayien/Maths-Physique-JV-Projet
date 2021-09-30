#ifndef MPJVP_VERTEX
#define MPJVP_VERTEX

#include <vulkan/vulkan_raii.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <array>

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 normal;

    static vk::VertexInputBindingDescription getBindingDescription();
    static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions();
};

#endif // MPJVP_VERTEX
