#include "render/Buffer.hpp"

Buffer::Buffer(const std::shared_ptr<LogicalDevice> & logicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) :
    m_logicalDevice{logicalDevice}
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
    allocInfo.memoryTypeIndex = m_logicalDevice->getPhysicalDevice()->findMemoryType(memRequirements.memoryTypeBits, properties);

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
