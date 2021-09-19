#ifndef FNES_SWAPCHAIN
#define FNES_SWAPCHAIN

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

#include "render/Window.hpp"
#include "render/Surface.hpp"
#include "render/PhysicalDevice.hpp"
#include "render/LogicalDevice.hpp"
#include "render/RenderPass.hpp"
#include "render/DescriptorSetLayout.hpp"
#include "render/GraphicsPipeline.hpp"
#include "render/CommandPool.hpp"
#include "render/Image.hpp"
#include "render/ImageView.hpp"
#include "render/FrameBuffer.hpp"
#include "render/DescriptorPool.hpp"

#include "render/UniformBufferObject.hpp"
#include "render/Entity.hpp"

class SwapChain
{
private:
    std::shared_ptr<Window> m_window;
    std::shared_ptr<Surface> m_surface;
    std::shared_ptr<PhysicalDevice> m_physicalDevice;
    std::shared_ptr<LogicalDevice> m_logicalDevice;

    VkSwapchainKHR m_swapChain;

    std::vector<std::shared_ptr<ImageView> > m_imageViews;
    std::shared_ptr<RenderPass> m_renderPass;
    std::shared_ptr<DescriptorSetLayout> m_descriptorSetLayout;
    std::shared_ptr<GraphicsPipeline> m_graphicsPipeline;
    std::shared_ptr<CommandPool> m_commandPool;
    std::shared_ptr<Image> m_colorImage;
    std::shared_ptr<ImageView> m_colorImageView;
    std::shared_ptr<Image> m_depthImage;
    std::shared_ptr<ImageView> m_depthImageView;
    std::vector<std::shared_ptr<FrameBuffer> > m_frameBuffers;

    std::vector<Entity> m_entities;

    std::vector<std::shared_ptr<Buffer> > m_uniformBuffers;
    std::shared_ptr<DescriptorPool> m_descriptorPool;

    VkFormat m_imageFormat;
    VkExtent2D m_extent;

public:
    SwapChain(const std::shared_ptr<Window> & window, const std::shared_ptr<Surface> & surface, const std::shared_ptr<PhysicalDevice> & physicalDevice, const std::shared_ptr<LogicalDevice> & logicalDevice);
    virtual ~SwapChain();

    void recreate();

    inline VkFormat getImageFormat() const { return m_imageFormat; }
    inline VkExtent2D getExtent() const { return m_extent; }

    inline VkSwapchainKHR & raw() { return m_swapChain; }

private:
    void create();
    void cleanup();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};

#endif // FNES_SWAPCHAIN
