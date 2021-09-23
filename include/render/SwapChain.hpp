#ifndef MPJVP_SWAPCHAIN
#define MPJVP_SWAPCHAIN

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <algorithm>

#include "render/Window.hpp"
#include "render/Surface.hpp"
#include "render/PhysicalDevice.hpp"
#include "render/LogicalDevice.hpp"
#include "render/GraphicsPipeline.hpp"
#include "render/CommandPool.hpp"
#include "render/FrameBuffers.hpp"
#include "render/DescriptorPool.hpp"
#include "render/DescriptorSets.hpp"
#include "render/CommandBuffers.hpp"

#include "render/UniformBufferObjectCamera.hpp"
#include "render/UniformBufferObjectTransform.hpp"
#include "render/World.hpp"
#include "render/Entity.hpp"

class SwapChain
{
private:
    std::shared_ptr<Window> m_window;
    std::shared_ptr<Surface> m_surface;
    std::shared_ptr<PhysicalDevice> m_physicalDevice;
    std::shared_ptr<LogicalDevice> m_logicalDevice;
    std::shared_ptr<CommandPool> m_commandPool;
    std::shared_ptr<World> m_world;

    VkSwapchainKHR m_swapChain;

    std::vector<std::shared_ptr<ImageView> > m_imageViews;
    
    std::shared_ptr<DescriptorSetLayout> m_descriptorSetLayout;

    std::shared_ptr<GraphicsPipeline> m_graphicsPipeline;
    std::shared_ptr<FrameBuffers> m_frameBuffers;

    std::shared_ptr<DescriptorPool> m_descriptorPool;

    std::shared_ptr<DescriptorSets> m_descriptorSets;

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
    inline CommandBuffer getCommandBuffer(size_t i) { return (*m_commandBuffers)[i]; }

    inline VkFormat getImageFormat() const { return m_imageFormat; }
    inline VkExtent2D getExtent() const { return m_extent; }
    inline size_t size() const { return m_imageViews.size(); }

    inline VkSwapchainKHR & raw() { return m_swapChain; }

    inline std::vector<std::shared_ptr<ImageView> > getImageViews () const { return m_imageViews; }

private:
    void create();
    void cleanup();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};

#endif // MPJVP_SWAPCHAIN
