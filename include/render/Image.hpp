#ifndef MPJVP_IMAGE
#define MPJVP_IMAGE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render/PhysicalDevice.hpp"
#include "render/LogicalDevice.hpp"

class Image
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;

    VkImage m_image;
    VkDeviceMemory m_imageMemory;
    
public:
    Image(const std::shared_ptr<LogicalDevice> & logicalDevice, VkExtent2D extent, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
    virtual ~Image();
    
    inline VkImage & raw() { return m_image; }
};

#endif // MPJVP_IMAGE
