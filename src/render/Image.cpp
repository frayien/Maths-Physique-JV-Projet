#include "render/Image.hpp"

Image::Image(const std::shared_ptr<LogicalDevice> & logicalDevice, VkExtent2D extent, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties) :
    m_logicalDevice{logicalDevice}
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = extent.width;
    imageInfo.extent.height = extent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = numSamples;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(m_logicalDevice->raw(), &imageInfo, nullptr, &m_image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(m_logicalDevice->raw(), m_image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = m_logicalDevice->getPhysicalDevice()->findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_logicalDevice->raw(), &allocInfo, nullptr, &m_imageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(m_logicalDevice->raw(), m_image, m_imageMemory, 0);
}

Image::~Image()
{
    vkDestroyImage(m_logicalDevice->raw(), m_image, nullptr);
    vkFreeMemory(m_logicalDevice->raw(), m_imageMemory, nullptr);
}