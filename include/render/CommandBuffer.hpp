#ifndef MPJVP_COMMANDBUFFER
#define MPJVP_COMMANDBUFFER

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

#include "render/RenderPass.hpp"
#include "render/FrameBuffer.hpp"
#include "render/GraphicsPipeline.hpp"

#include "render/World.hpp"
#include "render/Entity.hpp"

class CommandBuffer
{
private:
    VkCommandBuffer m_commandBuffer;

public:
    CommandBuffer(VkCommandBuffer commandBuffer);
    virtual ~CommandBuffer();

    void record(RenderPass & renderPass, FrameBuffer & frameBuffer, GraphicsPipeline & graphicsPipeline, VkExtent2D extent, VkDescriptorSet descriptorSet, const World & world);

    inline VkCommandBuffer & raw() { return m_commandBuffer; }
};

#endif // MPJVP_COMMANDBUFFER
