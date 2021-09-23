#include "render/DescriptorPool.hpp"

DescriptorPool::DescriptorPool(const std::shared_ptr<LogicalDevice> & logicalDevice, size_t size) :
    m_logicalDevice{logicalDevice}
{
    createPool(size);
}

DescriptorPool::~DescriptorPool()
{
    cleanup();
}

void DescriptorPool::cleanup()
{
    vkDestroyDescriptorPool(m_logicalDevice->raw(), m_descriptorPool, nullptr);
}

void DescriptorPool::recreate(size_t size)
{
    cleanup();
    createPool(size);
}

void DescriptorPool::createPool(size_t size)
{
    std::array<VkDescriptorPoolSize, 2> poolSizes;
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(size);

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(size);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(size * poolSizes.size());

    if (vkCreateDescriptorPool(m_logicalDevice->raw(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}
