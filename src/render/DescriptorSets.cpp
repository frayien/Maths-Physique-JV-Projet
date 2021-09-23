#include "render/DescriptorSets.hpp"


DescriptorSets::DescriptorSets(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<DescriptorPool> & descriptorPool, const DescriptorSetLayout & descriptorSetLayout, VkDeviceSize bufferSize, size_t size) :
    m_logicalDevice{logicalDevice},
    m_descriptorPool{descriptorPool}
{   
    create(descriptorSetLayout, bufferSize, size);
}

DescriptorSets::~DescriptorSets()
{
}

void DescriptorSets::recreate(const DescriptorSetLayout & descriptorSetLayout, VkDeviceSize bufferSize, size_t size)
{
    m_descriptorSets.clear();
    m_uniformBuffers.clear();
    create(descriptorSetLayout, bufferSize, size);
}

void DescriptorSets::create(const DescriptorSetLayout & descriptorSetLayout, VkDeviceSize bufferSize, size_t size)
{
    std::vector<VkDescriptorSetLayout> layouts(size, descriptorSetLayout.raw());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool->raw();
    allocInfo.descriptorSetCount = static_cast<uint32_t>(size);
    allocInfo.pSetLayouts = layouts.data();

    m_descriptorSets.resize(size);
    if (vkAllocateDescriptorSets(m_logicalDevice->raw(), &allocInfo, m_descriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    m_uniformBuffers.resize(size);

    for (size_t i = 0; i < size; i++)
    {
        m_uniformBuffers[i] = std::make_unique<Buffer>(m_logicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = m_uniformBuffers[i]->raw();
        bufferInfo.offset = 0;
        bufferInfo.range = bufferSize;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = m_descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr; // Optional
        descriptorWrite.pTexelBufferView = nullptr; // Optional

        vkUpdateDescriptorSets(m_logicalDevice->raw(), 1, &descriptorWrite, 0, nullptr);
    }
}
