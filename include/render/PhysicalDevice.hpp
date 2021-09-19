#ifndef FNES_PHYSICALDEVICE
#define FNES_PHYSICALDEVICE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>
#include <set>
#include <string>

#include "render/Instance.hpp"
#include "render/Surface.hpp"

namespace
{
    struct QueueFamilyIndices 
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() 
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };
}

class PhysicalDevice
{
private:
    const std::vector<const char*> m_deviceExtensions = 
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

private:
    std::shared_ptr<Instance> m_instance;
    std::shared_ptr<Surface> m_surface;

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

    VkSampleCountFlagBits m_msaaSampleCount = VK_SAMPLE_COUNT_1_BIT;
    
public:
    PhysicalDevice(const std::shared_ptr<Instance> & instance, const std::shared_ptr<Surface> & surface);
    virtual ~PhysicalDevice();

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkFormat findDepthFormat();

    inline std::vector<const char*> getDeviceExtensions() const { return m_deviceExtensions; }
    inline VkSampleCountFlagBits getMsaaSampleCount() const { return m_msaaSampleCount; }
    inline QueueFamilyIndices getQueueFamilies() { return findQueueFamilies(m_physicalDevice); }
    inline SwapChainSupportDetails getSwapChainSupport() { return m_surface->querySwapChainSupport(m_physicalDevice); }

    inline VkPhysicalDevice & raw() { return m_physicalDevice; }

private:
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    VkSampleCountFlagBits getMaxUsableSampleCount();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};



#endif // FNES_PHYSICALDEVICE
