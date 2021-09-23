#include "render/DescriptorSetLayout.hpp"

DescriptorSetLayout::DescriptorSetLayout(const std::shared_ptr<LogicalDevice> & logicalDevice) :
    m_logicalDevice{logicalDevice}
{
    std::array<VkDescriptorSetLayoutBinding, 2> uboLayoutBindings;

    uboLayoutBindings[0].binding = 0;
    uboLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; 
    uboLayoutBindings[0].descriptorCount = 1;
    uboLayoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBindings[0].pImmutableSamplers = nullptr;

    uboLayoutBindings[1].binding = 1;
    uboLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC; 
    uboLayoutBindings[1].descriptorCount = 1;
    uboLayoutBindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBindings[1].pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = uboLayoutBindings.size();
    layoutInfo.pBindings = uboLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(m_logicalDevice->raw(), &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(m_logicalDevice->raw(), m_descriptorSetLayout, nullptr);
}
