#include "render/CommandBuffer.hpp"

CommandBuffer::CommandBuffer(VkCommandBuffer commandBuffer) :
    m_commandBuffer{commandBuffer}
{
}

void CommandBuffer::record(const FrameBuffer & frameBuffer, VkDescriptorSet descriptorSet, const World & world)
{
    // store internaly a refenrence to the current entites.
    // This way, when an entity is removed from the world
    // it is not deleted before this buffer gets re-recorded.
    m_shapes = world.getShapes();

    // starting command buffer recording
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    // starting a render pass
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = frameBuffer.getGraphicsPipeline()->getRenderPass()->raw();
    renderPassInfo.framebuffer = frameBuffer.raw();
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = frameBuffer.getExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, frameBuffer.getGraphicsPipeline()->raw());

    size_t i = 0;
    for(const auto & shape : m_shapes)
    {
        std::array<uint32_t, 1> dynamicOffsets = {sizeof(UniformBufferObjectTransform) * static_cast<uint32_t>(i)};

        vkCmdBindDescriptorSets(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, frameBuffer.getGraphicsPipeline()->rawLayout(), 0, 1, &descriptorSet, dynamicOffsets.size(), dynamicOffsets.data());

        VkBuffer vertexBuffers[] = {shape->getVertexBuffer()->raw()};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(m_commandBuffer, 0, 1, vertexBuffers, offsets);

        VkBuffer indexBuffer  = shape->getIndexBuffer()->raw();

        vkCmdBindIndexBuffer(m_commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(m_commandBuffer, static_cast<uint32_t>(shape->getIndexBufferSize()), 1, 0, 0, 0);

        ++i;
    }
    

    vkCmdEndRenderPass(m_commandBuffer);

    if (vkEndCommandBuffer(m_commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }
}