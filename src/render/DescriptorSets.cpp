#include "render/DescriptorSets.hpp"


DescriptorSets::DescriptorSets(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<DescriptorPool> & descriptorPool, const DescriptorSetLayout & descriptorSetLayout, size_t dynamicBufferSize, size_t size) :
    m_logicalDevice{logicalDevice},
    m_descriptorPool{descriptorPool}
{
    create(descriptorSetLayout, dynamicBufferSize, size);
}

DescriptorSets::~DescriptorSets()
{
}

void DescriptorSets::resizeDynamicBuffer(size_t bufferIndex, size_t dynamicBufferSize)
{
    m_uniformBufferDynamics[bufferIndex] = std::make_unique<Buffer>(m_logicalDevice, dynamicBufferSize * sizeof(UniformBufferObjectTransform), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = m_uniformBuffers[bufferIndex]->raw();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObjectCamera);

    std::array<VkWriteDescriptorSet, 2> descriptorWrites;
    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = m_descriptorSets[bufferIndex];
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;
    descriptorWrites[0].pNext = nullptr;

    VkDescriptorBufferInfo bufferInfoDynamic{};
    bufferInfoDynamic.buffer = m_uniformBufferDynamics[bufferIndex]->raw();
    bufferInfoDynamic.offset = 0;
    bufferInfoDynamic.range = sizeof(UniformBufferObjectTransform);

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = m_descriptorSets[bufferIndex];
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pBufferInfo = &bufferInfoDynamic;
    descriptorWrites[1].pNext = nullptr;

    vkUpdateDescriptorSets(m_logicalDevice->raw(), descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
}

void DescriptorSets::recreate(const DescriptorSetLayout & descriptorSetLayout, VkDeviceSize dynamicBufferSize, size_t size)
{
    m_descriptorSets.clear();
    m_uniformBuffers.clear();
    m_uniformBufferDynamics.clear();
    create(descriptorSetLayout, dynamicBufferSize, size);
}

#include <iostream>

void DescriptorSets::create(const DescriptorSetLayout & descriptorSetLayout, size_t dynamicBufferSize, size_t size)
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
    m_uniformBufferDynamics.resize(size);

    for (size_t i = 0; i < size; i++)
    {
        m_uniformBuffers[i] = std::make_unique<Buffer>(m_logicalDevice, sizeof(UniformBufferObjectCamera), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        resizeDynamicBuffer(i, dynamicBufferSize);
    }
}
