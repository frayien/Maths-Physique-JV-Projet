#include "render/ImageView.hpp"


ImageView::ImageView(const std::shared_ptr<LogicalDevice> & logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) :
    m_logicalDevice{logicalDevice}
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(m_logicalDevice->raw(), &viewInfo, nullptr, &m_imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }
}

ImageView::~ImageView()
{
    vkDestroyImageView(m_logicalDevice->raw(), m_imageView, nullptr);
}
