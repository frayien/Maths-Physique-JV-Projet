#ifndef MPJVP_IMGUIVULKAN
#define MPJVP_IMGUIVULKAN

#include <vulkan/vulkan_raii.hpp>

#include <memory>
#include <vector>
#include <array>

#include "render/Window.hpp"
#include "render/IImGuiFrameGenerator.hpp"

class ImGuiVulkan
{
private:
    std::shared_ptr<vk::raii::Device>       m_device;

    uint32_t m_graphicsFamily;
    vk::Extent2D m_swapchainExtent;
    size_t m_swapchainSize;

    std::unique_ptr<vk::raii::DescriptorPool>            m_imguiDescriptorPool;
    std::unique_ptr<vk::raii::RenderPass>                m_imGuiRenderPass;
    std::unique_ptr<vk::raii::CommandPool>               m_imGuiCommandPool;
    std::unique_ptr<vk::raii::CommandBuffers>            m_imGuiCommandBuffers;
    std::vector<std::unique_ptr<vk::raii::Framebuffer> > m_imGuiFrameBuffers;

public:
    ImGuiVulkan(
        const std::shared_ptr<Window> & window, 
        const std::shared_ptr<vk::raii::Instance> & instance, 
        const std::shared_ptr<vk::raii::PhysicalDevice> & physicalDevice, 
        const std::shared_ptr<vk::raii::Device> & device,
        const std::shared_ptr<vk::raii::Queue> & graphicsQueue,
        const std::vector<std::shared_ptr<vk::raii::ImageView> > & swapchainImageViews,
        vk::Extent2D swapchainExtent,
        vk::Format imageFormat,
        uint32_t graphicsFamily);
    virtual ~ImGuiVulkan();

    void render(uint32_t m_imageIndex);
    void recreate(const std::vector<std::shared_ptr<vk::raii::ImageView> > & swapchainImageViews, vk::Extent2D swapchainExtent, vk::Format imageFormat);
    void createFrame(IImGuiFrameGenerator* generator);

    inline const vk::raii::CommandBuffer & getCommandBuffer(size_t i) { return (*m_imGuiCommandBuffers)[i]; }

private:
    std::shared_ptr<vk::raii::CommandBuffers> beginSingleTimeCommands();
    void endSingleTimeCommands(const std::shared_ptr<vk::raii::CommandBuffers> & commandBuffer, const std::shared_ptr<vk::raii::Queue> & graphicsQueue);
    void createImGuiCommandPool();
    void createImGuiCommandBuffers();
};

#endif // MPJVP_IMGUIVULKAN
