#ifndef MPJVP_SURFACE
#define MPJVP_SURFACE

#include <vulkan/vulkan_raii.hpp>

#include <memory>
#include <string>

#include "render/Window.hpp"
#include "render/Instance.hpp"

//namespace
//{
    struct SwapChainSupportDetails 
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
//}

// TODO remove

class Surface
{
private:
    std::shared_ptr<Window>   m_window;
    std::shared_ptr<Instance> m_instance;

    VkSurfaceKHR m_surface;
    
public:
    Surface(const std::shared_ptr<Window> & window, const std::shared_ptr<Instance> & instance);
    virtual ~Surface();

    bool supportPhysicalDevice(VkPhysicalDevice device, uint32_t queueFamilyIndex); 
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    inline VkSurfaceKHR & raw() { return m_surface; }
};

#endif // MPJVP_SURFACE
