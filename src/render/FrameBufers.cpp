#include "render/FrameBuffers.hpp"

FrameBuffers::FrameBuffers(const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<GraphicsPipeline> & graphicsPipeline, const std::vector<std::shared_ptr<ImageView> > & imageViews, VkExtent2D extent, VkFormat colorFormat, VkFormat depthFormat) :
    m_logicalDevice{logicalDevice}
{
    createImages(extent, colorFormat, depthFormat);
    createFrameBuffers(graphicsPipeline, imageViews, extent);
}

FrameBuffers::~FrameBuffers()
{
}

void FrameBuffers::recreate(const std::shared_ptr<GraphicsPipeline> & graphicsPipeline, const std::vector<std::shared_ptr<ImageView> > & imageViews, VkExtent2D extent, VkFormat colorFormat, VkFormat depthFormat)
{
    m_frameBuffers.clear();

    createImages(extent, colorFormat, depthFormat);
    createFrameBuffers(graphicsPipeline, imageViews, extent);
}

void FrameBuffers::createFrameBuffers(const std::shared_ptr<GraphicsPipeline> & graphicsPipeline, const std::vector<std::shared_ptr<ImageView> > & imageViews, VkExtent2D extent)
{
    for (const std::shared_ptr<ImageView> & imageView : imageViews)
    {
        m_frameBuffers.push_back(std::make_unique<FrameBuffer>(m_logicalDevice, graphicsPipeline, m_colorImageView, m_depthImageView, imageView, extent));
    }
}

void FrameBuffers::createImages(VkExtent2D extent, VkFormat colorFormat, VkFormat depthFormat)
{
    VkSampleCountFlagBits msaaSampleCount = m_logicalDevice->getPhysicalDevice()->getMsaaSampleCount();

    m_colorImage = std::make_shared<Image>(m_logicalDevice, extent, msaaSampleCount, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    m_colorImageView = std::make_shared<ImageView>(m_logicalDevice, m_colorImage->raw(), colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);

    m_depthImage = std::make_shared<Image>(m_logicalDevice, extent, msaaSampleCount, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    m_depthImageView = std::make_shared<ImageView>(m_logicalDevice, m_depthImage->raw(), depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    
}