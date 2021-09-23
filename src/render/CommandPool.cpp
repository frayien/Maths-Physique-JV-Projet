#include "render/CommandPool.hpp"

CommandPool::CommandPool(const std::shared_ptr<LogicalDevice> & logicalDevice) :
    m_logicalDevice{logicalDevice}
{
    QueueFamilyIndices queueFamilyIndices = m_logicalDevice->getPhysicalDevice()->getQueueFamilies();

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(m_logicalDevice->raw(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
}

CommandPool::~CommandPool()
{
    vkDestroyCommandPool(m_logicalDevice->raw(), m_commandPool, nullptr);
}


void CommandPool::copyBuffer(Buffer & src, Buffer & dest, VkDeviceSize size)
{
    // Allocate a new command buffer
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_logicalDevice->raw(), &allocInfo, &commandBuffer);

    // Begin the copy command
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, src.raw(), dest.raw(), 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    // Submit the command to the queue
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_logicalDevice->getGraphicsQueue().raw(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_logicalDevice->getGraphicsQueue().raw());

    // Clean command buffer
    vkFreeCommandBuffers(m_logicalDevice->raw(), m_commandPool, 1, &commandBuffer);
}