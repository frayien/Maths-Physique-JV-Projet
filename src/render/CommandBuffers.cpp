#include "render/CommandBuffers.hpp"

CommandBuffers::CommandBuffers(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool, size_t size) :
    m_logicalDevice{logicalDevice},
    m_commandPool{commandPool}
{
    create(size);
}

CommandBuffers::~CommandBuffers()
{
    cleanup();
}

void CommandBuffers::recreate(size_t size)
{
    cleanup();
    m_commandBuffers.clear();
    create(size);
}

void CommandBuffers::cleanup()
{
    vkFreeCommandBuffers(m_logicalDevice->raw(), m_commandPool->raw(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
}

void CommandBuffers::create(size_t size)
{
    m_commandBuffers.resize(size);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool->raw();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(size);

    if (vkAllocateCommandBuffers(m_logicalDevice->raw(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}