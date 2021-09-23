#ifndef MPJVP_IMGUIVULKAN
#define MPJVP_IMGUIVULKAN

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

#include "render/Instance.hpp"
#include "render/LogicalDevice.hpp"
#include "render/PhysicalDevice.hpp"
#include "render/SwapChain.hpp"
#include "render/Window.hpp"

class ImGuiVulkan
{
private:
    std::shared_ptr<LogicalDevice>  m_logicalDevice ;
    std::shared_ptr<SwapChain>      m_swapChain     ;

    VkDescriptorPool m_imguiDescriptorPool;
    VkRenderPass m_imGuiRenderPass;
    VkCommandPool m_imGuiCommandPool;
    std::vector<VkCommandBuffer> m_imGuiCommandBuffers;
    std::vector<VkFramebuffer> m_imGuiFrameBuffers;

public:
    ImGuiVulkan(const std::shared_ptr<Window> & window, const std::shared_ptr<Instance> & instance, const std::shared_ptr<PhysicalDevice> & physicalDevice, const std::shared_ptr<LogicalDevice> & logicalDevice, const std::shared_ptr<SwapChain> & swapChain);
    virtual ~ImGuiVulkan();

    void render(uint32_t m_imageIndex);
    void cleanup();
    void recreate();
    void createFrame();

    inline VkCommandBuffer getCommandBuffer(size_t i) { return m_imGuiCommandBuffers[i]; }

private:
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void createImGuiCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateFlags flags);
    void createImGuiCommandBuffers(VkCommandBuffer* commandBuffer, uint32_t commandBufferCount, VkCommandPool &commandPool);
};

#endif // MPJVP_IMGUIVULKAN
