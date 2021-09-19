#include "render/FrameBuffer.hpp"

FrameBuffer::FrameBuffer(
    const std::shared_ptr<LogicalDevice> & logicalDevice, 
    const std::shared_ptr<RenderPass> & renderPass, 
    const std::shared_ptr<ImageView> & colorImageView, 
    const std::shared_ptr<ImageView> & depthImageView, 
    const std::shared_ptr<ImageView> & swapChainImageView, 
    VkExtent2D extent) :
    m_logicalDevice{logicalDevice},
    m_renderPass{renderPass},
    m_colorImageView{colorImageView},
    m_depthImageView{depthImageView},
    m_swapChainImageView{swapChainImageView}
{
    std::array<VkImageView, 3> attachments =
    {
        m_colorImageView->raw(),
        m_depthImageView->raw(),
        m_swapChainImageView->raw(),
    };

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = m_renderPass->raw();
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(m_logicalDevice->raw(), &framebufferInfo, nullptr, &m_frameBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create framebuffer!");
    }
}

FrameBuffer::~FrameBuffer()
{
    vkDestroyFramebuffer(m_logicalDevice->raw(), m_frameBuffer, nullptr);
}