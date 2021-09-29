#include "render/SwapChain.hpp"

SwapChain::SwapChain(const std::shared_ptr<Window> & window, const std::shared_ptr<Surface> & surface, const std::shared_ptr<PhysicalDevice> & physicalDevice, const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool, const std::shared_ptr<World> & world) :
    m_window{window},
    m_surface{surface},
    m_physicalDevice{physicalDevice},
    m_logicalDevice{logicalDevice},
    m_commandPool{commandPool},
    m_world{world}
{

    m_descriptorSets = std::make_shared<DescriptorSets>(m_logicalDevice, m_descriptorPool, *m_descriptorSetLayout, m_world->getShapes().size(), size());

    m_commandBuffers = std::make_shared<CommandBuffers>(m_logicalDevice, m_commandPool, size());
    for(size_t i = 0; i < size(); ++i)
    {
        recordCommandBuffer(i);
    }
}

SwapChain::~SwapChain()
{
    cleanup();
}

void SwapChain::recreate()
{
    m_descriptorSets->recreate(*m_descriptorSetLayout, m_world->getShapes().size(), size());


    m_commandBuffers->recreate(size());
    for(size_t i = 0; i < size(); ++i)
    {
        recordCommandBuffer(i);
    }
}

void SwapChain::recordCommandBuffer(size_t i)
{
    m_descriptorSets->resizeDynamicBuffer(i, m_world->getShapes().size() * sizeof(UniformBufferObjectTransform));

    const auto & commandBuffer = (*m_commandBuffers)[i];
    const FrameBuffer & frameBuffer = (*m_frameBuffers)[i];
    commandBuffer->record(frameBuffer, m_descriptorSets->getRaw(i), *m_world);
}

