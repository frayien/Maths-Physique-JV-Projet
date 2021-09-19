#include "render/Buffer.hpp"

Buffer::Buffer(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool, PhysicalDevice & physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) :
    m_logicalDevice{logicalDevice},
    m_commandPool{commandPool}
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_logicalDevice->raw(), &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_logicalDevice->raw(), m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = physicalDevice.findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_logicalDevice->raw(), &allocInfo, nullptr, &m_bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(m_logicalDevice->raw(), m_buffer, m_bufferMemory, 0);
}

Buffer::~Buffer()
{
    vkDestroyBuffer(m_logicalDevice->raw(), m_buffer, nullptr);
    vkFreeMemory(m_logicalDevice->raw(), m_bufferMemory, nullptr);
}

void Buffer::loadData(const void* data, VkDeviceSize size)
{
    void* data_dst;
    vkMapMemory(m_logicalDevice->raw(), m_bufferMemory, 0, size, 0, &data_dst);
        memcpy(data_dst, data, (size_t) size);
    vkUnmapMemory(m_logicalDevice->raw(), m_bufferMemory);
}

void Buffer::copyFrom(Buffer & src, VkDeviceSize size)
{
    // Allocate a new command buffer
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPool->raw();
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
    vkCmdCopyBuffer(commandBuffer, src.m_buffer, m_buffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    // Submit the command to the queue
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_logicalDevice->getGraphicsQueue().raw(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_logicalDevice->getGraphicsQueue().raw());

    // Clean command buffer
    vkFreeCommandBuffers(m_logicalDevice->raw(), m_commandPool->raw(), 1, &commandBuffer);
}