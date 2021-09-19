#ifndef FNES_IMAGEVIEW
#define FNES_IMAGEVIEW

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <memory>

#include "render/LogicalDevice.hpp"

class ImageView
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;
    VkImageView m_imageView;
    
public:
    ImageView(const std::shared_ptr<LogicalDevice> & logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    virtual ~ImageView();

    inline VkImageView & raw() { return m_imageView; }
};

#endif // FNES_IMAGEVIEW
