#ifndef FNES_FRAMEBUFFER
#define FNES_FRAMEBUFFER

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

#include "render/LogicalDevice.hpp"
#include "render/ImageView.hpp"
#include "render/RenderPass.hpp"

class FrameBuffer
{
private:
    std::shared_ptr<ImageView> m_colorImageView;
    std::shared_ptr<ImageView> m_depthImageView;
    std::shared_ptr<ImageView> m_swapChainImageView;

    std::shared_ptr<LogicalDevice> m_logicalDevice;
    std::shared_ptr<RenderPass> m_renderPass;

    VkFramebuffer m_frameBuffer;
    
public:
    FrameBuffer(
        const std::shared_ptr<LogicalDevice> & logicalDevice, 
        const std::shared_ptr<RenderPass> & renderPass, 
        const std::shared_ptr<ImageView> & colorImageView, 
        const std::shared_ptr<ImageView> & depthImageView, 
        const std::shared_ptr<ImageView> & swapChainImageView, 
        VkExtent2D extent);
    virtual ~FrameBuffer();
};

#endif // FNES_FRAMEBUFFER
