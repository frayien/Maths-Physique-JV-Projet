#include "render/DescriptorPool.hpp"

DescriptorPool::DescriptorPool(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::vector<std::shared_ptr<DescriptorSetLayout> > & descriptorSetLayouts, size_t size) :
    m_logicalDevice{logicalDevice}
{
    createPool(size * descriptorSetLayouts.size());
}

DescriptorPool::~DescriptorPool()
{
    cleanup();
}

void DescriptorPool::cleanup()
{
    vkDestroyDescriptorPool(m_logicalDevice->raw(), m_descriptorPool, nullptr);
}

void DescriptorPool::recreate(const std::vector<std::shared_ptr<DescriptorSetLayout> > & descriptorSetLayouts, size_t size)
{
    cleanup();
    createPool(size * descriptorSetLayouts.size());
}

void DescriptorPool::createPool(size_t size)
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(size);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(size);

    if (vkCreateDescriptorPool(m_logicalDevice->raw(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}
