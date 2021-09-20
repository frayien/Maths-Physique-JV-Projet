#include "render/CommandPool.hpp"

CommandPool::CommandPool(const std::shared_ptr<LogicalDevice> & logicalDevice, QueueFamilyIndices queueFamilyIndices) :
    m_logicalDevice{logicalDevice}
{
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
