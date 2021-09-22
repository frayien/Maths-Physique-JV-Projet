#ifndef MPJVP_FRAMEBUFFER
#define MPJVP_FRAMEBUFFER

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

#include "render/LogicalDevice.hpp"
#include "render/ImageView.hpp"
#include "render/GraphicsPipeline.hpp"

class FrameBuffer
{
private:
    std::shared_ptr<ImageView> m_colorImageView;
    std::shared_ptr<ImageView> m_depthImageView;
    std::shared_ptr<ImageView> m_swapChainImageView;

    std::shared_ptr<LogicalDevice> m_logicalDevice;
    std::shared_ptr<GraphicsPipeline> m_graphicsPipeline;

    VkFramebuffer m_frameBuffer;

    VkExtent2D m_extent;
    
public:
    FrameBuffer(
        const std::shared_ptr<LogicalDevice> & logicalDevice, 
        const std::shared_ptr<GraphicsPipeline> & graphicsPipeline, 
        const std::shared_ptr<ImageView> & colorImageView, 
        const std::shared_ptr<ImageView> & depthImageView, 
        const std::shared_ptr<ImageView> & swapChainImageView, 
        VkExtent2D extent);
    virtual ~FrameBuffer();

    inline const std::shared_ptr<GraphicsPipeline> & getGraphicsPipeline() const { return m_graphicsPipeline; }
    inline VkExtent2D getExtent() const { return m_extent; }

    inline const VkFramebuffer & raw() const { return m_frameBuffer; }
};

#endif // MPJVP_FRAMEBUFFER
