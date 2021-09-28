#ifndef MPJVP_COMMANDBUFFER
#define MPJVP_COMMANDBUFFER

#include <vulkan/vulkan_raii.hpp>

#include <stdexcept>

#include "render/RenderPass.hpp"
#include "render/FrameBuffer.hpp"
#include "render/GraphicsPipeline.hpp"

#include "render/UniformBufferObjectTransform.hpp"

#include "render/World.hpp"
#include "render/BufferedShape.hpp"

class CommandBuffer
{
private:
    VkCommandBuffer m_commandBuffer;

    std::list<std::shared_ptr<BufferedShape>> m_shapes;

public:
    CommandBuffer(VkCommandBuffer commandBuffer);

    void record(const FrameBuffer & frameBuffer, VkDescriptorSet descriptorSet, const World & world);

    inline VkCommandBuffer & raw() { return m_commandBuffer; }
};

#endif // MPJVP_COMMANDBUFFER
