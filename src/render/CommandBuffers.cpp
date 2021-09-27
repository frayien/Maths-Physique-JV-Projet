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
    std::vector<VkCommandBuffer> vkCommandBuffers;
    for(auto & cmdBuf : m_commandBuffers)
    {
        vkCommandBuffers.push_back(cmdBuf->raw());
    }

    vkFreeCommandBuffers(m_logicalDevice->raw(), m_commandPool->raw(), static_cast<uint32_t>(vkCommandBuffers.size()), vkCommandBuffers.data());
}

void CommandBuffers::create(size_t size)
{
    std::vector<VkCommandBuffer> vkCommandBuffers;
    vkCommandBuffers.resize(size);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool->raw();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(size);

    if (vkAllocateCommandBuffers(m_logicalDevice->raw(), &allocInfo, vkCommandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for(auto vkCmdBuf : vkCommandBuffers)
    {
        m_commandBuffers.push_back(std::make_unique<CommandBuffer>(vkCmdBuf));
    }
}