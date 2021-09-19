#include "render/Surface.hpp"

Surface::Surface(const std::shared_ptr<Window> & window, const std::shared_ptr<Instance> & instance) :
    m_window  {window},
    m_instance{instance}
{
    VkResult result = glfwCreateWindowSurface(m_instance->raw(), m_window->raw(), nullptr, &m_surface);
    if (result != VK_SUCCESS) 
    {
        const char* description;
        glfwGetError(&description);
        throw std::runtime_error("failed to create window surface! " + std::to_string(result) + " " + description);
    }
}

Surface::~Surface()
{
    vkDestroySurfaceKHR(m_instance->raw(), m_surface, nullptr);
}

bool Surface::supportPhysicalDevice(VkPhysicalDevice device, uint32_t queueFamilyIndex)
{
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, queueFamilyIndex, m_surface, &presentSupport);

    return presentSupport;
}

SwapChainSupportDetails Surface::querySwapChainSupport(VkPhysicalDevice device) 
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);

    if (formatCount != 0) 
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) 
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

