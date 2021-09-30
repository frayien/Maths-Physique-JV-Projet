#ifndef MPJVP_SWAPCHAIN
#define MPJVP_SWAPCHAIN

#include <vulkan/vulkan_raii.hpp>

#include <memory>
#include <algorithm>

#include "render/Window.hpp"
#include "render/Surface.hpp"
#include "render/DescriptorSets.hpp"
#include "render/CommandBuffers.hpp"

#include "render/UniformBufferObjectCamera.hpp"
#include "render/UniformBufferObjectTransform.hpp"
#include "render/World.hpp"
#include "render/BufferedShape.hpp"

class SwapChain
{
private:

    std::shared_ptr<CommandBuffers> m_commandBuffers;

    VkFormat m_imageFormat;
    VkExtent2D m_extent;

public:
    SwapChain(const std::shared_ptr<Window> & window, const std::shared_ptr<Surface> & surface, const std::shared_ptr<PhysicalDevice> & physicalDevice, const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<CommandPool> & commandPool, const std::shared_ptr<World> & world);
    virtual ~SwapChain();

    void recreate();
    void recordCommandBuffer(size_t i);

    inline const std::unique_ptr<Buffer> & getCameraUniformBuffer(size_t i) const { return m_descriptorSets->getUniformBuffer(i); }
    inline const std::unique_ptr<Buffer> & getTransformsUniformBuffer(size_t i) const { return m_descriptorSets->getUniformBufferDynamic(i); }
    inline const std::unique_ptr<CommandBuffer> & getCommandBuffer(size_t i) const { return (*m_commandBuffers)[i]; }

    inline VkFormat getImageFormat() const { return m_imageFormat; }
    inline VkExtent2D getExtent() const { return m_extent; }
    inline size_t size() const { return m_imageViews.size(); }

    inline VkSwapchainKHR & raw() { return m_swapChain; }

    inline std::vector<std::shared_ptr<ImageView> > getImageViews () const { return m_imageViews; }

private:
    void create();
    void cleanup();

    
};

#endif // MPJVP_SWAPCHAIN
