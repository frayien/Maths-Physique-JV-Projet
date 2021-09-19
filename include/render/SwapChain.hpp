#ifndef FNES_SWAPCHAIN
#define FNES_SWAPCHAIN

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

#include "render/Window.hpp"
#include "render/Surface.hpp"
#include "render/PhysicalDevice.hpp"
#include "render/LogicalDevice.hpp"

class SwapChain
{
private:
    std::shared_ptr<Window> m_window;
    std::shared_ptr<Surface> m_surface;
    std::shared_ptr<PhysicalDevice> m_physicalDevice;
    std::shared_ptr<LogicalDevice> m_logicalDevice;

    VkSwapchainKHR m_swapChain;

    std::vector<VkImage> m_images;
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
