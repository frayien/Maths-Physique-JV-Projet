#ifndef MPJVP_RENDERPASS
#define MPJVP_RENDERPASS

#include <vulkan/vulkan_raii.hpp>

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

#endif // MPJVP_RENDERPASS
