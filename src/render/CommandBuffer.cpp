#include "render/CommandBuffer.hpp"

CommandBuffer::CommandBuffer(VkCommandBuffer commandBuffer) :
    m_commandBuffer{commandBuffer}
{
}

CommandBuffer::~CommandBuffer()
{
}

void CommandBuffer::record(RenderPass & renderPass, FrameBuffer & frameBuffer, GraphicsPipeline & graphicsPipeline, VkExtent2D extent, VkDescriptorSet descriptorSet, const World & world)
{    
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
    renderPassInfo.renderPass = renderPass.raw();
    renderPassInfo.framebuffer = frameBuffer.raw();
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.raw());

    vkCmdBindDescriptorSets(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.rawLayout(), 0, 1, &descriptorSet, 0, nullptr);


    for(const Entity & entity : world.getEntities())
    {
        VkBuffer vertexBuffers[] = {entity.getVertexBuffer()->raw()};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(m_commandBuffer, 0, 1, vertexBuffers, offsets);

        VkBuffer indexBuffer  = entity.getIndexBuffer()->raw();

        vkCmdBindIndexBuffer(m_commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(m_commandBuffer, static_cast<uint32_t>(entity.getIndexBufferSize()), 1, 0, 0, 0);
    }
    

    vkCmdEndRenderPass(m_commandBuffer);

    if (vkEndCommandBuffer(m_commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }
}