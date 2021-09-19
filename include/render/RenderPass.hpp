#ifndef FNES_RENDERPASS
#define FNES_RENDERPASS

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <stdexcept>
#include <memory>

#include "render/LogicalDevice.hpp"

class RenderPass
{
private:
    std::shared_ptr<LogicalDevice> m_logicalDevice;

    VkRenderPass m_renderPass;
    
public:
    RenderPass(const std::shared_ptr<LogicalDevice> & logicalDevice, VkFormat imageFormat, VkFormat depthFormat, VkSampleCountFlagBits msaaSampleCount);
    virtual ~RenderPass();

    inline VkRenderPass & raw() { return m_renderPass; }
};

#endif // FNES_RENDERPASS
