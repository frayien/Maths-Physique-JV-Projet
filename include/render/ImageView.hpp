#ifndef MPJVP_IMAGEVIEW
#define MPJVP_IMAGEVIEW

#include <vulkan/vulkan_raii.hpp>

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

#endif // MPJVP_IMAGEVIEW
